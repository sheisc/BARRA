/*****************************************************************

******************************************************************/
#include <unistd.h>
#include <dlfcn.h> 
#include <stdio.h>



#include "randomize.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

char *gets(char *s){
    typedef char * (* GETS_FUNC)(char *s);
    static GETS_FUNC _gets;
    if(!_gets){
        _gets = (GETS_FUNC) dlsym(RTLD_NEXT, "gets");
	
    }    
    au_edu_unsw_randomize();
    return _gets(s);
}













#ifndef __COMMON_H
#define __COMMON_H

#define DEBUG

#ifdef DEBUG
#define debug(fmt, args...)   printf(fmt, ##args) 
#else
#define debug(fmt, args...) 
#endif

#define SAVE_FREE(p) \
    do {             \
        free(p);     \
        p = NULL;    \
    } while(0)       

#endif

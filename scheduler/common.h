#ifndef __COMMON_H
#define __COMMON_H
/* Platform specific configurations */
#define USE_MAVLINK
#define USE_FPGA_LDR
#define STATIC_BUFFER

/* Error Code Group */

#define CM_OFFSET  1
#define CM_ERR_CODE(e)  ((e) < CM_OFFSET)
#define SBG_OFFSET 2
#define SBG_ERR_CODE(e) ((e) < SBG_OFFSET)

#define PARSER_OFFSET 4
#define PARSER_ERR_CODE(e) ((e) < PARSER_OFFSET)

typedef int16_t status_t;


/* Debug Level*/
#define I	4
#define W	3
#define E	2
/* DEBUG */
#ifdef  DEBUG   
#define debug(fmt,args...)  printf (fmt ,##args)   
#define vdebug(level,fmt,args...) if (DEBUG>=level) printf(fmt,##args);   
#else   
#define debug(fmt,args...)   
#define vdebug(level,fmt,args...)   
#endif  /* DEBUG */   

/* NULL Pointer Dereference Guard */
#define _RETURN_IF_FAIL(p)											\
   do{													\
	if ((p) == NULL)												\
	{												\
		vdebug(W, "[W] Null pointer use in function: %s!\n", __func__);				\
		return;										\
	}												\
   }													\
   while(0)

#define _RETURN_MINUS_IF_FAIL(p)											\
   do{													\
	if ((p) == NULL)												\
	{												\
		vdebug(W, "[W] Null pointer use in function: %s!\n", __func__);				\
		return NULL_P;										\
	}												\
   }													\
   while(0)
#define _RETURN_P_IF_FAIL(p)											\
   do{													\
	if ((p) == NULL)												\
	{												\
		vdebug(W, "[W] Null pointer use in function: %s!\n", __func__);				\
		return &NULL_P;										\
	}												\
   }													\
   while(0)
#define _RETURN_VAL_IF_FAIL(p, val)											\
   do{													\
	if ((p) == NULL)												\
	{												\
		vdebug(W, "[W] Null pointer use in function: %s!\n", __func__);				\
		return val;										\
	}												\
   }													\
   while(0)

/*
 *
 * \brief safe free macros to avoid wild pointer 
 * 
 * \note _SAFE_FREE_REF_POINTER can only be used with pointer which has
 * 	 a reference count
 *
 */

#define _SAFE_FREE_REF_POINTER(p)	\
	do {			        \
		_RETURN_P_IF_FAIL((p))  \
		if((p)->_refcnt > 1){	\
		    (p)->_refcnt--;	\
		}else{			\
		    free((p));	\
	            p = NULL;	\
                }		\
	}			\
	while(0)	

#define _SAFE_FREE(p)	        \
	do {			\
		_RETURN_P_IF_FAIL((p)) \
		free((p));	\
	        p = NULL;	\
	}			\
	while(0)	

#endif //__COMMON_H

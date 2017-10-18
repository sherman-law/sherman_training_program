#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

array_t *array_create (int length)
{
    array_t *p_array = (array_t *)malloc(sizeof(array_t) + sizeof(void *) * length);

    int i = 0;

    if (NULL == p_array) {
        debug("create array fail!\r\n");   
	return NULL;
    }

    p_array->read   = 0;
    p_array->write  = 0;
    p_array->length = length;
    p_array->inited = 1;

    for (i = 0; i < length; i++) {
        p_array->data[i] = NULL; 
    } 

    return p_array;
}

array_status_t array_push (array_t *thiz, void *data, int size)
{
    int write = 0;
  
    if (thiz == NULL) {
        debug("array is NULL.:in func:%s\r\n", __func__);
	return ARRAY_NULL_ERROR;
    }

    write = (thiz->write + 1) % thiz->length;

    if (write != thiz->read) {
        thiz->data[thiz->write] = malloc(size);

	memcpy(thiz->data[thiz->write], data, size);

	thiz->write = write;
    }    

    return ARRAY_NO_ERROR;
}

array_status_t array_pop (array_t *thiz, void **data)
{
    if ((thiz == NULL) && (data == NULL)) {
        debug("array is NULL.:in func:%s\r\n", __func__);
	return ARRAY_NULL_ERROR;
    }

    if (thiz->read != thiz->write) {
	if (thiz->data[thiz->read] != NULL) {
            *data = thiz->data[thiz->read]; 
	    thiz->read = (thiz->read + 1) % thiz->length;
	}
    } else {
        debug("read = write\r\n"); 
    }

    return ARRAY_NO_ERROR;

}

array_status_t array_destory (array_t **thiz)
{
    int i = 0;
    array_t *p_array = NULL;

    if (*thiz == NULL) {
        debug("array is NULL.:in func:%s\r\n", __func__);
	return ARRAY_NULL_ERROR;
    }

    p_array = *thiz;

    for (i = p_array->length - 1; i >= 0; i--) {
	if (p_array->data[i] != NULL) {
            SAVE_FREE(p_array->data[i]);
	} else {
            debug("data is NULL\r\n");	
	}
    }

    SAVE_FREE(p_array);
    *thiz = NULL;

    return ARRAY_NO_ERROR;
}



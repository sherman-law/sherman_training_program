#ifndef __ARRAY_H
#define __ARRAY_H

#include "../double_list/common.h"

#define MAX_SIZE   100

typedef int array_status_t;
#define ARRAY_NULL_ERROR   0 
#define ARRAY_NO_ERROR     1
#define ARRAY_FAIL         2 
#define ARRAY_GOT_RESULT   3

typedef struct _array_t {
    int read;
    int write;
    int length;
    int inited;
    void *data[0];
} array_t;

array_t *array_create(int length);

array_status_t array_push(array_t *thiz, void *data, int size);

array_status_t array_pop(array_t *thiz, void **data);

array_status_t array_destory(array_t **thiz);
#endif

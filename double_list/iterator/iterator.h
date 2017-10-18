#ifndef __ITERATOR_H
#define __ITERATOR_H

#include "../common.h"
#include <stdlib.h>
#include <stdio.h>

typedef int iterator_status_t;
#define ITERATOR_NO_ERROR   0
#define ITERATOR_NULL_ERROR 1
#define ITERATOR_FAIL       2

struct _iterator_it;
typedef struct _iterator_t iterator_t;

typedef iterator_status_t (* iterator_set_func)(iterator_t *thiz, void *data);
typedef iterator_status_t (* iterator_get_func)(iterator_t *thiz, void **data);
typedef iterator_status_t (* iterator_next_func)(iterator_t *thiz);
typedef iterator_status_t (* iterator_prev_func)(iterator_t *thiz);
typedef iterator_status_t (* iterator_advance_func)(iterator_t *thiz, int offset);
typedef iterator_status_t (* iterator_clone_func)(iterator_t *thiz, iterator_t **cloned);
typedef void (* iterator_destory_func)(iterator_t *thiz);
typedef int (* iterator_offset_func)(iterator_t *thiz);

struct _iterator_t {
    iterator_set_func      set;
    iterator_get_func      get;
    iterator_next_func     next;
    iterator_prev_func     prev;
    iterator_advance_func  advance;
    iterator_clone_func    clone;
    iterator_destory_func  destory;
    iterator_offset_func   offset;

    char priv[0];
};

static inline iterator_status_t iterator_set (iterator_t *thiz, void *data) 
{
    if (thiz == NULL || thiz->set == NULL) {
        debug("iterator or set func is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR;	
    }

    return thiz->set(thiz, data);
}

static inline iterator_status_t iterator_get (iterator_t *thiz, void **data)
{
    if (thiz == NULL || thiz->get == NULL) {
        debug("iterator or get func is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR;	
    }

    return thiz->get(thiz, data);
}

static inline iterator_status_t iterator_next (iterator_t *thiz)
{
    if (thiz == NULL || thiz->next == NULL) {
        debug("iterator or next func is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR;	
    }

    return thiz->next(thiz);
}

static inline iterator_status_t iterator_prev (iterator_t *thiz) 
{
    if (thiz == NULL || thiz->prev == NULL) {
        debug("iterator or prev func is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR;	
    }

    return thiz->prev(thiz);
}

static inline iterator_status_t iterator_advance (iterator_t *thiz, int offset)
{
    if (thiz == NULL || thiz->advance == NULL) {
        debug("iterator or advance func is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR;	
    }

    return thiz->advance(thiz, offset);
}
static inline iterator_status_t iterator_clone (iterator_t *thiz, iterator_t **clone)
{
    if (thiz == NULL || thiz->clone == NULL) {
        debug("iterator or clone func is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR;	
    }

    return thiz->clone(thiz, clone);
}

static inline void iterator_destory (iterator_t *thiz)
{
    if (thiz == NULL || thiz->destory == NULL) {
        debug("iterator or destory func is NULL.in func:%s\r\n", __func__);
    }

    thiz->destory(thiz);
}

static inline int iterator_offset (iterator_t *thiz)
{
    if (thiz == NULL || thiz->offset == NULL) {
        debug("iterator or offset func is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR;	
    }

    return thiz->offset(thiz);
}

#endif

#include <string.h>
#include "iterator_list.h"

typedef struct _priv_t {
    dlink_list_t *list;
    list_node_t  *cursor;
    int           offset;
} priv_t;

static iterator_status_t dlist_iterator_set(iterator_t *thiz, void *data)
{
    priv_t *p_priv = (priv_t *)thiz->priv;

    if (p_priv->list == NULL || p_priv->cursor == NULL) {
	debug("list or cursor is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR; 
    }

    p_priv->cursor->data = data;

    return ITERATOR_NO_ERROR;
}

static iterator_status_t dlist_iterator_get(iterator_t *thiz, void **data)
{
    priv_t *p_priv = (priv_t *)thiz->priv;

    if (p_priv->list == NULL || p_priv->cursor == NULL) {
	debug("list or cursor is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR; 
    }

    *data = p_priv->cursor->data;

    return ITERATOR_NO_ERROR;
}

static iterator_status_t dlist_iterator_next(iterator_t *thiz)
{
    priv_t *p_priv = (priv_t *)thiz->priv;

    if (p_priv->list == NULL || p_priv->cursor == NULL) {
	debug("list or cursor is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR; 
    }
	
    if (p_priv->cursor->next == p_priv->list->tail) {
	debug("cursor is the last node.in func:%s\r\n", __func__);
	return ITERATOR_FAIL;
    }

    p_priv->cursor = p_priv->cursor->next;
    p_priv->offset++;

    return ITERATOR_NO_ERROR;
}

static iterator_status_t dlist_iterator_prev(iterator_t *thiz)
{
    priv_t *p_priv = (priv_t *)thiz->priv;

    if (p_priv->list == NULL || p_priv->cursor == NULL) {
	debug("list or cursor is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR; 
    }
	
    if (p_priv->cursor->prev == p_priv->list->head) {
	debug("cursor is the first node.in func:%s\r\n", __func__);
	return ITERATOR_FAIL;
    }

    p_priv->cursor = p_priv->cursor->prev;
    p_priv->offset--;

    return ITERATOR_NO_ERROR;
}

static iterator_status_t dlist_iterator_advance(iterator_t *thiz, int offset)
{
    priv_t *p_priv = (priv_t *)thiz->priv;
    list_node_t *p_node = NULL;
    int pos = offset;

    if (p_priv->list == NULL || p_priv->cursor == NULL) {
	debug("list or cursor is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR; 
    }

    p_node = p_priv->cursor;

    if (offset > 0) {
        for (pos = offset; pos > 0 && p_node->next != NULL; pos--) {
            p_node = p_node->next;	
	} 
    } else {
        for (pos = -offset; pos > 0 && p_node->next != NULL; pos--) {
            p_node = p_node->prev;	
	} 
    }

    if (p_node == NULL) {
	debug("p_node is null.in func:%s\r\n", __func__);
        return ITERATOR_FAIL;
    }
    
    p_priv->cursor = p_node; 
    p_priv->offset += offset;
    
    return ITERATOR_NO_ERROR;
}

static iterator_status_t dlist_iterator_clone(iterator_t *thiz, iterator_t **clone)
{
    priv_t *p_priv = (priv_t *)thiz->priv;

    if (p_priv->list == NULL || p_priv->cursor == NULL) {
	debug("list or cursor is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR; 
    }   

    *clone = (iterator_t *)malloc(sizeof(iterator_t) + sizeof(priv_t));

    if (*clone == NULL) {
        return ITERATOR_FAIL;
    }

    memcpy(*clone, thiz, sizeof(iterator_t) + sizeof(priv_t)); 

    return ITERATOR_NO_ERROR;
}

static void dlist_iterator_destory(iterator_t *thiz)
{
    if (thiz != NULL) {
       free(thiz);
       thiz = NULL; 
    }
}


static int dlist_iterator_offset(iterator_t *thiz)
{
    priv_t *p_priv = (priv_t *)thiz->priv;

    if (p_priv->list == NULL || p_priv->cursor == NULL) {
	debug("list or cursor is NULL.in func:%s\r\n", __func__);
        return ITERATOR_NULL_ERROR; 
    }   

    return p_priv->offset;
}

iterator_t *dlist_iterator_create(dlink_list_t *list)
{
    iterator_t *thiz = NULL;
    
    if (list == NULL) {
	debug("list is NULL.in func:%s\r\n", __func__);
        return NULL; 
    }

    thiz = (iterator_t *)malloc(sizeof(iterator_t) + sizeof(priv_t));

    if (thiz == NULL) {
	debug("create fail.in func:%s\r\n", __func__);
        return NULL; 
    }

    priv_t *p_priv = (priv_t *)thiz->priv;

    thiz->set     = dlist_iterator_set;
    thiz->get     = dlist_iterator_get; 
    thiz->clone   = dlist_iterator_clone;
    thiz->prev    = dlist_iterator_prev;
    thiz->next    = dlist_iterator_next;
    thiz->advance = dlist_iterator_advance;
    thiz->offset  = dlist_iterator_offset;
    thiz->destory = dlist_iterator_destory;

    p_priv->list   = list;
    p_priv->cursor = list->head->next;
    p_priv->offset = 0;

    return thiz;
}



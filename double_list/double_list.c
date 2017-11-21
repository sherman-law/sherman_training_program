#include "double_list.h"
#include "common.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

dlink_list_t *dl_list_create()
{
    dlink_list_t *list = (dlink_list_t *)malloc(sizeof(dlink_list_t));
   
    if (NULL == list) {              /* check if the list is NULL */
        debug("create list fail.\r\n"); 
        return NULL;
    }
   
    list->head = (list_node_t *)malloc(sizeof(list_node_t));     

    if (NULL == list->head) {
        debug("create list head fail.\r\n"); 
        return NULL;
    }

    memset(list->head, 0, sizeof(list->head));

    list->tail = (list_node_t *)malloc(sizeof(list_node_t));     

    if (NULL == list->tail) {
        debug("create list tail fail.\r\n"); 
        return NULL;
    }

    memset(list->tail, 0, sizeof(list->tail));

    list->head->next = list->tail;    /* the head node point to tail node */
    list->tail->prev = list->head;
    list->head->prev = NULL;
    list->tail->next = NULL;
    list->length     = 0;

    return list;
}

list_status_t dl_list_for_each(dlink_list_t *thiz, list_node_t **node, fp_visit_func visit_func, void *ctx)
{
    list_node_t *p_node = NULL;

    if (NULL == thiz || thiz->length == 0) {
        debug("list is NULL.in func:%s.\r\n", __func__); 
        return LIST_NULL_ERROR; 
    }

    p_node = thiz->head;           /* start to traverse the list from list's head */

    for (p_node = p_node->next; p_node->next != NULL; p_node = p_node->next) {
        if (visit_func(ctx, p_node) == 1) {     /* if the visit function return 1,return the current node */
            *node = p_node;
            return LIST_GOT_RESULT;
        }
    }    
        
    return LIST_NO_ERROR;
}

list_status_t dl_list_prepend(dlink_list_t *thiz, void *data, int size)
{
    list_node_t *p_node    = NULL;
    list_node_t *data_node = NULL;    /* the node should be prepend which store user's data */
    
    if (NULL == thiz || NULL == data) {
        debug("there is something wrong in function:%s.\r\n", __func__); 
        return LIST_NULL_ERROR;
    }

    data_node = (list_node_t *)malloc(sizeof(list_node_t));    

#ifdef INSERT_PATTERN_MEMCPY  
    data_node->data = malloc(size); 

    memcpy(data_node->data, data, size);
#else
    data_node->data = data;
#endif

    p_node = thiz->head;              /* insert data to list's head */
 
    p_node->next->prev = data_node;
    data_node->next    = p_node->next;
    p_node->next       = data_node;
    data_node->prev    = p_node;

    thiz->length++;

    return LIST_NO_ERROR; 
}

list_status_t dl_list_append(dlink_list_t *thiz, void *data, int size)
{
    list_node_t *p_node    = NULL;
    list_node_t *data_node = NULL;
    
    if (NULL == thiz || NULL == data) {
        debug("there is something wrong in function:%s.\r\n", __func__); 
        return LIST_NULL_ERROR;
    }

    data_node = (list_node_t *)malloc(sizeof(list_node_t));    
     
#ifdef INSERT_PATTERN_MEMCPY  
    data_node->data = malloc(size); 

    memcpy(data_node->data, data, size);
#else
    data_node->data = data;
#endif

    p_node = thiz->tail;                 /* insert data to list's tail */
 
    p_node->prev->next = data_node;
    data_node->prev    = p_node->prev;
    data_node->next    = p_node;
    p_node->prev       = data_node;

    thiz->length++;

    return LIST_NO_ERROR; 
}

list_status_t dl_list_search(dlink_list_t *thiz, list_node_t **node, fp_visit_func compare_func, void *ctx)
{
    list_node_t *p_node = NULL; 

    if (NULL == thiz) {
        debug("list is NULL.in func:%s.\r\n", __func__); 
        return LIST_NULL_ERROR; 
    }

    /* find the required node */
    if (dl_list_for_each(thiz, node, compare_func, ctx) == LIST_GOT_RESULT) { 
        return LIST_NO_ERROR; 
    } else {
        return LIST_FAIL;
    }

}

list_status_t dl_list_delete_node(dlink_list_t *thiz, list_node_t *node)
{
    list_node_t *p_node = NULL; 

    p_node = node;

    if (NULL == thiz) {
        debug("there is something wrong in function:%s.\r\n", __func__); 
        return LIST_NULL_ERROR;
    }

    p_node->prev->next = p_node->next;  /* delete node from list and free memory */
    p_node->next->prev = p_node->prev;

    SAVE_FREE(p_node->data);
    SAVE_FREE(p_node);
    
    thiz->length--;
   
    return LIST_NO_ERROR; 
}

list_status_t dl_list_delete(dlink_list_t *thiz, fp_visit_func compare_func, void *ctx)
{
    list_node_t *p_node = NULL;   

    if (NULL == thiz) {
        debug("list is NULL.in func:%s.\r\n", __func__); 
        return LIST_NULL_ERROR;
    }

    /* search node which you want to delete */
    if (dl_list_for_each(thiz, &p_node, compare_func, ctx) == LIST_GOT_RESULT) {
        dl_list_delete_node(thiz, p_node);

        return LIST_NO_ERROR;
    } else {
        debug("could not find node.in func:%s.\r\n", __func__); 
        return LIST_FAIL;
    }
}

list_status_t dl_list_clear (dlink_list_t *thiz)
{
    list_node_t *p_node = NULL;
    
    if (NULL == thiz) {
        debug("list is NULL.in func:%s.\r\n", __func__); 
        return LIST_NULL_ERROR; 
    }

    if (thiz->length == 0) {
        debug("list is empty!in func:%s.\r\n",__func__);
	return LIST_FAIL;
    }

    p_node = thiz->head->next;
    
    debug("%s:before clear:%d\r\n", __func__, thiz->length);
    for (; thiz->length > 0;) {
        debug("deleted data:%d\r\n", *(int *)p_node->data);
        dl_list_delete_node(thiz, p_node);
        p_node = p_node->next;
    }
    debug("%s:after clear:%d\r\n", __func__, thiz->length);

    return LIST_NO_ERROR;
}

list_status_t dl_list_destory(dlink_list_t **thiz)
{
    dlink_list_t *p_list = *thiz;
    list_node_t *p_node = NULL;
    
    if (NULL == p_list) {
        debug("list is NULL.in func:%s.\r\n", __func__); 
        return LIST_NULL_ERROR; 
    }
    
    p_node = p_list->head->next;   /* start from the node next to head node */

    /* free all memory in the list */
    for (; p_list->length > 0; p_list->length--) {
        
        p_node->prev->next = p_node->next;
        p_node->next->prev = p_node->prev;
 
        free(p_node->data);
        free(p_node);
        p_node = p_node->next;
    } 
  
    /* free the head node memory and tail node memory */
    SAVE_FREE(p_list->head->data); 
    SAVE_FREE(p_list->head);
    SAVE_FREE(p_list->tail->data);
    SAVE_FREE(p_list->tail);
    SAVE_FREE(p_list);

    *thiz = NULL;
    return LIST_NO_ERROR;
}



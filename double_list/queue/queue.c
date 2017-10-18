#include "queue.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

queue_t *queue_create ()
{
    queue_t *p_queue = (queue_t *)malloc(sizeof(queue_t));

    if (NULL == p_queue) {
        debug("create queue fail.\r\n");
        return NULL;
    }

    p_queue->list = dl_list_create();
   
    if (NULL == p_queue->list) { /* free memory if failed to create list */
        debug("create queue fail.\r\n");
        SAVE_FREE(p_queue);
        return NULL;
    }

    return p_queue;   
}

queue_status_t queue_push (queue_t *thiz, void *data, int size)
{
    if (NULL == thiz) {
        debug("queue is NULL.in func:%s", __func__);
        return QUEUE_NULL_ERROR;
    }

    if (dl_list_append(thiz->list, data, size) != LIST_NO_ERROR) {
        return QUEUE_FAIL;
    } 

    return QUEUE_NO_ERROR;
}

queue_status_t queue_pop (queue_t *thiz) 
{
    if (NULL == thiz) {
        debug("queue is NULL.in func:%s", __func__);
        return QUEUE_NULL_ERROR;
    }

    if (dl_list_delete_node(thiz->list, thiz->list->head->next) != LIST_NO_ERROR) {
        return QUEUE_FAIL;
    } 

    return QUEUE_NO_ERROR;
}

queue_status_t queue_head (queue_t *thiz, void **data)
{
    list_node_t *p_node = NULL;

    if (NULL == thiz) {
        debug("queue is NULL.in func:%s", __func__);
        return QUEUE_NULL_ERROR;
    }

    p_node = thiz->list->head; /* got the list's head */
    *data  = p_node->next->data;

    return QUEUE_NO_ERROR;
}

int queue_get_length (queue_t *thiz)
{
    if (NULL == thiz) {
        debug("queue is NULL.in func:%s", __func__);
        return QUEUE_NULL_ERROR;
    }

    return thiz->list->length;
}

queue_status_t queue_search (queue_t *thiz, list_node_t **node, fp_visit_func compare_func, void *ctx)
{
    if (NULL == thiz) {
        debug("list is NULL.in func:%s.\r\n", __func__); 
        return QUEUE_NULL_ERROR;
    }

    if (dl_list_for_each(thiz->list, node, compare_func, ctx) == LIST_GOT_RESULT) {
        return QUEUE_NO_ERROR; 
    } else {
        return QUEUE_FAIL;
    }
}

queue_status_t queue_for_each (queue_t *thiz, fp_visit_func visit_func, void *ctx) 
{
    if (NULL == thiz) {
        debug("list is NULL.in func:%s.\r\n", __func__); 
        return QUEUE_NULL_ERROR;
    }

    if (dl_list_for_each(thiz->list, NULL, visit_func, ctx) != LIST_NO_ERROR) {
        return QUEUE_FAIL;
    }

    return QUEUE_NO_ERROR;
}

queue_status_t queue_destory (queue_t **thiz)
{
    queue_t *p_queue = NULL;

    if (NULL == *thiz) {
        debug("queue is NULL.in func:%s.\r\n", __func__);
        return QUEUE_NULL_ERROR;
    }

    p_queue = *thiz;

    if (dl_list_destory(&p_queue->list) != LIST_NO_ERROR) {
        debug("destory fail!in func:%s.\r\n", __func__);    
        return QUEUE_FAIL;
    }

    SAVE_FREE(p_queue);
    *thiz = NULL;

    return QUEUE_NO_ERROR; 
}


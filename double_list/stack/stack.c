#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

stack_t *stack_create()
{
    stack_t *p_stack = (stack_t *)malloc(sizeof(stack_t));

    if (NULL == p_stack) {
        debug("create stack fail.\r\n");
        return NULL;
    }

    p_stack->list = dl_list_create();

    if (NULL == p_stack->list) {
        debug("create stack fail.\r\n");
        SAVE_FREE(p_stack);
        return NULL;
    }

    return p_stack;        
}

stack_status_t stack_push(stack_t *thiz, void *data, int size)
{
    if (NULL == thiz) {
        debug("stack is NULL.in func:%s\r\n", __func__);
        return STACK_NULL_ERROR;
    }

    if (dl_list_prepend(thiz->list, data, size) != LIST_NO_ERROR) {
        return STACK_FAIL;
    }

    return STACK_NO_ERROR;
}

stack_status_t stack_pop(stack_t *thiz)
{
    if (NULL == thiz) {
        debug("stack is NULL.in func:%s\r\n", __func__);
        return STACK_NULL_ERROR;
    }

    if (dl_list_delete_node(thiz->list, thiz->list->head->next) != LIST_NO_ERROR) {
        return STACK_FAIL;
    }  

    return STACK_NO_ERROR;
}

stack_status_t stack_search(stack_t *thiz, list_node_t **node, fp_visit_func compare_func, void *ctx)
{
    if (NULL == thiz) {
        debug("stack is NULL.in func:%s\r\n", __func__);
        return STACK_NULL_ERROR;
    }
 
    if (dl_list_for_each(thiz->list, node, compare_func, ctx) != LIST_GOT_RESULT) {
        return STACK_FAIL;
    }  

    return STACK_NO_ERROR;
}

stack_status_t stack_for_each(stack_t *thiz, fp_visit_func visit_func, void *ctx)
{
    if (NULL == thiz) {
        debug("stack is NULL.in func:%s\r\n", __func__);
        return STACK_NULL_ERROR;
    }

    if (dl_list_for_each(thiz->list, NULL, visit_func, ctx) != LIST_NO_ERROR) {
        return STACK_FAIL;
    }

    return STACK_NO_ERROR;
}

stack_status_t stack_destory(stack_t **thiz)
{
    stack_t *p_stack = NULL;

    if (NULL == *thiz) {
        debug("stack is NULL.in func:%s\r\n", __func__);
        return STACK_NULL_ERROR;
    }

    p_stack = *thiz;

    if (dl_list_destory(&p_stack->list) != LIST_NO_ERROR) { 
        debug("destory fail!in func:%s.\r\n", __func__);
        return STACK_FAIL;
    }

    SAVE_FREE(p_stack);
    *thiz = NULL;

    return STACK_NO_ERROR;
}

stack_status_t stack_top(stack_t *thiz, void **data)
{
    list_node_t *p_node = NULL;

    if (NULL == thiz) {
        debug("stack is NULL.in func:%s\r\n", __func__);
        return STACK_NULL_ERROR;
    }
 
    p_node = thiz->list->head; 
    *data  = p_node->next->data;

    return STACK_NO_ERROR; 
}

int stack_get_length(stack_t *thiz)
{
    if (NULL == thiz) {
        debug("stack is NULL.in func:%s\r\n", __func__);
        return STACK_NULL_ERROR;
    }

    return thiz->list->length;
}

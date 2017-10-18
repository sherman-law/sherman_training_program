#ifndef __STACK_H
#define __STACK_H

#include "../double_list.h" 
#include "../common.h"

typedef int stack_status_t;
#define STACK_NULL_ERROR  0
#define STACK_NO_ERROR    1
#define STACK_FAIL        2
#define STACK_GOT_RESULT  3

typedef struct _stack_t {
    dlink_list_t *list;
} stack_t;

/**
 * \brief create function which is used to create stack 
 *
 * \retval the pointer point to stack
 */
stack_t *stack_create(void);

/**
 * \brief push data to the stack's top
 * 
 * \param[in] *thiz:this pointer
 * \param[in] *data:the data should be inserted
 * \param[in]  size:size of data
 *
 * \retval error code
 */
stack_status_t stack_push(stack_t *thiz, void *data, int size);

/**
 * \brief   pop stack top data
 *
 * \param[in]  *thiz:thiz pointer
 *
 * \retval  error code 
 */
stack_status_t stack_pop(stack_t *thiz);

/**
 * \brief searching function which is used to search specified data 
 * 
 * \param[in]       *thiz:this pointer
 * \param[in,out]  **node:node which is found 
 * \param[in]        compare_func:comparing function provided by user
 * \param[in]       *ctx:what you want to search
 *
 * \retval error code
 */
stack_status_t stack_search(stack_t *thiz, list_node_t **node, fp_visit_func compare_func, void *ctx);

/**
 * \brief traversing function 
 * 
 * \param[in]   *thiz:this pointer
 * \param[in]    visit_func:comparing function provided by user
 * \param[in]   *ctx:what you want to search
 *
 * \retval error code
 */
stack_status_t stack_for_each(stack_t *thiz, fp_visit_func visit_func, void *ctx);

/**
 * \brief destory stack
 * 
 * \param[in,out] **thiz:this pointer
 *
 * \retval error code
 */
stack_status_t stack_destory(stack_t **thiz);
 
/**
 * \brief destory stack
 * 
 * \param[in]   *thiz:this pointer
 * \param[out] **data:stack top data
 *
 * \retval error code
 */
stack_status_t stack_top(stack_t *thiz, void **data);

/**
 * \brief get the length of stack
 *
 * \param[in]  *thiz:this pointer
 *
 * \retval the length of stack 
 */
int stack_get_length(stack_t *thiz);

#endif

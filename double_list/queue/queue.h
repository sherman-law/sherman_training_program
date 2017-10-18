#ifndef __QUEUE_H
#define __QUEUE_H

#include "../double_list.h"
#include "../common.h"

typedef int queue_status_t;
#define QUEUE_NULL_ERROR 0
#define QUEUE_NO_ERROR   1
#define QUEUE_FAIL       2
#define QUEUE_GOT_RESULT 3	

typedef struct _queue_t {
    dlink_list_t *list;
} queue_t;

/**
 * \brief create function which is used to create queue 
 *
 * \retval the pointer pointe to queue 
 */
queue_t *queue_create();

/**
 * \brief pushing function which is used to insert node to queue's tail
 * 
 * \param[in] *thiz:this pointer
 * \param[in] *data:the data should be inserted
 * \param[in]  size:size of data
 *
 * \retval error code
 */
queue_status_t queue_push(queue_t *thiz, void *data, int size);

/**
 * \brief delete end node 
 * 
 * \param[in]  *thiz:this pointer
 *
 * \retval error code
 */
queue_status_t queue_pop(queue_t *thiz);

/**
 * \brief searching function which is used to search specified data 
 * 
 * \param[in]   *thiz:this pointer
 * \param[out]  **node:node which is found 
 * \param[in]    compare_func:comparing function provided by user
 * \param[in]   *ctx:what you want to search
 *
 * \retval error code
 */
queue_status_t queue_search(queue_t *thiz, list_node_t **node, fp_visit_func compare_func, void *ctx);

/**
 * \brief get the start node
 *
 * \param[in]   *thiz:this pointer
 * \param[out]  **data:data we get 
 *
 * \retval  error code
 */
queue_status_t queue_head(queue_t *thiz, void **data);

/**
 * \brief the function of traversal queue 
 * 
 * \param[in]      *thiz:this pointer
 * \param[in]       visit_func:operating function
 * \param[in, out] *ctx :the pointer of context
 *
 * \retval error code
 */
queue_status_t queue_for_each(queue_t *thiz, fp_visit_func visit_func, void *ctx); 

/**
 * \brief destory the list
 * 
 * \param[in,out] **thiz:this pointer
 *
 * \retval error code
 */
queue_status_t queue_destory(queue_t **thiz);

/**
 * \brief get the length of queue
 *
 * \param[in]  *thiz:this pointer
 *
 * \retval the length of queue 
 */
int queue_get_length(queue_t *thiz);
#endif

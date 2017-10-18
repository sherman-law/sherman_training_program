#ifndef __DOUBLE_LIST_H
#define __DOUBLE_LIST_H

/* define memory copy macro and use memory copy pattern 
   otherwise use pointer pattern */
#define INSERT_PATTERN_MEMCPY  

typedef int list_status_t;
#define LIST_NULL_ERROR 0
#define LIST_NO_ERROR   1
#define LIST_GOT_RESULT 2
#define LIST_FAIL       3 

/**
 * \brief node structure of the list
 */
typedef struct _list_node_t {
    struct _list_node_t *prev;
    struct _list_node_t *next;
    void *data;
} list_node_t;

/**
 * \brief list object structure
 */
typedef struct _dlink_list_t {
    list_node_t *head;
    list_node_t *tail;
    int length;
} dlink_list_t;

/**
 * \brief the function pointer point to the operating function.
 * 
 * \param[in]      *data:the pointer which point to node data. 
 * \param[in, out] *ctx :the pointer of context
 *
 * \retval error code
 */
typedef int (* fp_visit_func)(void *ctx, list_node_t *data);

/**
 * \brief create function which is used to create double linked list
 *
 * \retval the pointer pointe to double linked list
 */
dlink_list_t *dl_list_create();

/**
 * \brief searching function which is used to search specified data 
 * 
 * \param[in]   *thiz:this pointer
 * \param[in]  **node:node which is found 
 * \param[in]    compare_func:comparing function provided by user
 * \param[in]   *ctx:what you want to search
 *
 * \retval error code
 */
list_status_t dl_list_search(dlink_list_t *thiz, list_node_t **node, fp_visit_func compare_func, void *ctx);

/**
 * \brief inserting function which is used to insert node to list's tail
 * 
 * \param[in] *thiz:this pointer
 * \param[in] *data:the data should be inserted
 * \param[in]  size:size of data
 *
 * \retval error code
 */
list_status_t dl_list_append(dlink_list_t *thiz, void *data, int size);

/**
 * \brief inserting function which is used to insert node to list's head
 * 
 * \param[in] *thiz:this pointer
 * \param[in] *data:the data should be inserted
 * \param[in]  size:size of data
 *
 * \retval error code
 */
list_status_t dl_list_prepend(dlink_list_t *thiz, void *data, int size);

/**
 * \brief delete node from list
 * 
 * \param[in]  *thiz:this pointer
 * \param[in]  compare_func:operating function
 * \param[in, out] *ctx :the pointer of context
 *
 * \retval error code
 */
list_status_t dl_list_delete(dlink_list_t *thiz, fp_visit_func compare_func, void *ctx);

/**
 * \brief delete node from list
 * 
 * \param[in]  *thiz:this pointer
 * \param[in]  *node:the node pointer  
 *
 * \retval error code
 */
list_status_t dl_list_delete_node(dlink_list_t *thiz, list_node_t *node);

/**
 * \brief  traversing function 
 * 
 * \param[in]      *thiz:this pointer
 * \param[in,out]     **node:start node 
 * \param[in]       visit_func:operating function
 * \param[in, out] *ctx :the pointer of context
 *
 * \retval error code
 */
list_status_t dl_list_for_each(dlink_list_t *thiz, list_node_t **node, fp_visit_func visit_func, void *ctx);

/**
 * \brief destory the list
 * 
 * \param[in,out] **thiz:this pointer
 *
 * \retval error code
 */
list_status_t dl_list_destory(dlink_list_t **thiz);

#endif

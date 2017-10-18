#include "iterator_list.h"

iterator_status_t invert (iterator_t *forward, iterator_t *backward)
{
    void *data1 = NULL;
    void *data2 = NULL;

    if (forward == NULL || backward == NULL) {
        debug("forward or backward is NULL.in func:%s\r\n", __func__); 
	return ITERATOR_NULL_ERROR;
    }
    
    debug("forward offset:%d, backward offset%d.\r\n", iterator_offset(forward), iterator_offset(backward));

    for (;iterator_offset(forward) < iterator_offset(backward); iterator_prev(backward), iterator_next(forward)) {
        debug("forward offset:%d, backward offset%d.\r\n", iterator_offset(forward), iterator_offset(backward));
        iterator_get(forward, &data1);	
        iterator_get(backward, &data2);	
        iterator_set(forward, data2);	
        iterator_set(backward, data1);	
    }

    return ITERATOR_NO_ERROR;
}

int printf_info (void *ctx, list_node_t *node)
{
    int *list_data = NULL;
    ctx = NULL;
    list_data = (int *)node->data;

    printf("data:%d\r\n", *list_data);
    
    return 0;
}

int main (void)
{
    int data = 0;

    dlink_list_t *ming_list = dl_list_create(); 

    for (data = 10; data < 90; data += 10) {
        dl_list_append(ming_list, (void *)&data, sizeof(data)); 
    }

    dl_list_for_each(ming_list, NULL, printf_info, NULL);

    iterator_t *forward = dlist_iterator_create(ming_list); 
    iterator_t *backward = dlist_iterator_create(ming_list);
    iterator_advance(backward, 7);

    printf("###########\r\n");

    invert(forward, backward); 
    dl_list_for_each(ming_list, NULL, printf_info, NULL);

    return 0;
}


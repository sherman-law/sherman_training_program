#include <stdio.h>

#include "../double_list.h"
#include "../common.h"
#include "queue.h"

int printf_info (void *ctx, list_node_t *node)
{
    int *list_data = NULL;
    ctx = NULL;
    list_data = (int *)node->data;

    printf("data:%d\r\n", *list_data);
    
    return 0;
}

int compare_func (void *ctx, list_node_t *node)
{
    int search_data = *(int *)ctx;
    int list_data   = *(int *)node->data;

    if (search_data == list_data) {
        printf("got the data(%d) which we want\r\n", list_data);
        return 1;
    } else {
        return 0;
    }
}

int main (void) 
{
    int data = 10;    
    int *got_data = 0; 
    list_node_t *ming_node = NULL;
    queue_t *ming_queue = queue_create(); 

    queue_push(ming_queue, (void *)&data, sizeof(data));
    data = 20;
    queue_push(ming_queue, (void *)&data, sizeof(data));
    data = 30;
    queue_push(ming_queue, (void *)&data, sizeof(data));
    data = 40;
    queue_push(ming_queue, (void *)&data, sizeof(data));
    queue_for_each(ming_queue, printf_info, NULL);
    printf("##############\r\n");    

    data = 30;
    if (queue_search(ming_queue, &ming_node, compare_func, (void *)&data) == QUEUE_NO_ERROR) {
        printf("find data:%d\r\n", *(int *)ming_node->data);
    } else {
        printf("not find!\r\n");
    }

    printf("##############\r\n");    
    if (queue_head(ming_queue, (void *)&got_data) == QUEUE_NO_ERROR) {
        printf("head data is:%d\r\n", *got_data);
    }
    if (queue_pop(ming_queue) == QUEUE_NO_ERROR) {
        printf("pop successfully!\r\n");
    }
    if (queue_head(ming_queue, (void *)&got_data) == QUEUE_NO_ERROR) {
        printf("head data is:%d\r\n", *got_data);
    }

    queue_for_each(ming_queue, printf_info, NULL);

    printf("##############\r\n");    
    if (queue_destory(&ming_queue) == QUEUE_NO_ERROR) {
        printf("destory successfully\r\n");
    }
    if (queue_destory(&ming_queue) != QUEUE_NO_ERROR) {
        printf("list is null!\r\n");
    }
    return 0;
}



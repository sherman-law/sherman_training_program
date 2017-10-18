#include <stdio.h>

#include "../double_list.h"
#include "../common.h"
#include "stack.h"

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
    stack_t *ming_stack = stack_create();
    
    for (data = 10; data < 50; data += 10) {
        stack_push(ming_stack, (void *)&data, sizeof(data));
    }
    stack_for_each(ming_stack, printf_info, NULL);
    printf("##############\r\n");    

    data = 30;
    if (stack_search(ming_stack, &ming_node, compare_func, (void *)&data) == STACK_NO_ERROR) {
        printf("find data:%d\r\n", *(int *)ming_node->data);
    } else {
        printf("not find!\r\n");
    }

    printf("##############\r\n");    
    if (stack_top(ming_stack, (void *)&got_data) == STACK_NO_ERROR) {
        printf("top data is:%d\r\n", *got_data);
    }
    if (stack_pop(ming_stack) == STACK_NO_ERROR) {
        printf("pop successfully!\r\n");
    }
    if (stack_top(ming_stack, (void *)&got_data) == STACK_NO_ERROR) {
        printf("top data is:%d\r\n", *got_data);
    }

    stack_for_each(ming_stack, printf_info, NULL);

    printf("##############\r\n");

    if (stack_destory(&ming_stack) == STACK_NO_ERROR) {
        printf("destory successfully\r\n");
    }
    if (stack_destory(&ming_stack) != STACK_NO_ERROR) {
        printf("list is null!\r\n");
    }
    return 0;
}

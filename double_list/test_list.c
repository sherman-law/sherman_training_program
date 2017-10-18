#include <stdio.h>
#include "double_list.h"
#include "common.h"
int printf_info (void *ctx, list_node_t *node)
{
    int *list_data = NULL;
    ctx = NULL;
    list_data = (int *)node->data;

    printf("data:%d\r\n", *list_data);
    
    return 0;
}

int find_max (void *ctx, void *data)
{
    int list_data = 0;
    int max_data  = *(int *)ctx;    

    list_data = *(int *)data;
    
    if (max_data == 0 || (max_data < list_data)) {
        max_data = list_data;
    }
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

    dlink_list_t *ming_list = dl_list_create();
    list_node_t *ming_node = NULL; 

    dl_list_append(ming_list, &data, sizeof(data));
    data = 20;
    dl_list_append(ming_list, &data, sizeof(data));

    data = 30;
    dl_list_append(ming_list, &data, sizeof(data));
    data = 40;
    dl_list_append(ming_list, &data, sizeof(data));
    data = 50;
    dl_list_prepend(ming_list, &data, sizeof(data));
    dl_list_for_each(ming_list, NULL, printf_info, NULL);
    
    data = 40;
    if (dl_list_search(ming_list, &ming_node, compare_func, (void *)&data) == LIST_NO_ERROR) {
        printf("find data:%d\r\n", *(int *)ming_node->data);
    } else {
        printf("not find!\r\n");
    }

    data = 30;
    dl_list_delete(ming_list, compare_func, (void *)&data);

    printf("##############\r\n");    
    dl_list_for_each(ming_list, NULL, printf_info, NULL);

    printf("##############\r\n");    
    if (dl_list_destory(&ming_list) == LIST_NO_ERROR) {
        printf("destory successfully\r\n");
    }
    if (dl_list_destory(&ming_list) == LIST_NO_ERROR) {
        printf("list is null!\r\n");
    }

    return 0;
}


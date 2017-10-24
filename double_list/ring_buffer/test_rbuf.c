#include "ring_buffer.h"

#define ARRAY_LEN(ARRAY) (sizeof(ARRAY)/sizeof(ARRAY[0]))

int main () 
{
    rbuf_t *ming_rbuf = rbuf_create(5, sizeof(int));

    print_all(ming_rbuf, 4);

    int ming_data[] = {1, 2, 3};
    int ming_data2[] = {4, 5};
    int ming_max[]  = {1, 2, 3, 4, 5, 6};
    int ming_get[5] = {0};
    int i = 0;

    printf("****************\r\n");
    rbuf_write(ming_rbuf, ming_data, ARRAY_LEN(ming_data), sizeof(int));
    print_all(ming_rbuf, 5);
    printf("space_left:%d\r\n", ming_rbuf->space_left);
    printf("write_cursor:%d\r\n", ming_rbuf->write_cursor);
    printf("read_cursor:%d\r\n", ming_rbuf->read_cursor);
    printf("page:%d\r\n", ming_rbuf->page_counter);
    printf("last_page:%d\r\n", ming_rbuf->last_read_page);

    rbuf_read(ming_rbuf, ming_get, 2, sizeof(int));
    for (i = 0; i < 5; i++) {
        printf("ming_get[%d]:%d\r\n", i, ming_get[i]); 
    }

    printf("****************\r\n");
    rbuf_write(ming_rbuf, ming_data, ARRAY_LEN(ming_data), sizeof(int));
    printf("append two more!!\r\n");
    rbuf_write(ming_rbuf, ming_data, 2, sizeof(int));
    print_all(ming_rbuf, 5);
    printf("space_left:%d\r\n", ming_rbuf->space_left);
    printf("write_cursor:%d\r\n", ming_rbuf->write_cursor);
    printf("read_cursor:%d\r\n", ming_rbuf->read_cursor);
    printf("page:%d\r\n", ming_rbuf->page_counter);
    printf("last_page:%d\r\n", ming_rbuf->last_read_page);

    printf("had read:%d\r\n", rbuf_read(ming_rbuf, ming_get, 3, sizeof(int)));
    for (i = 0; i < 5; i++) {
        printf("ming_get[%d]:%d\r\n", i, ming_get[i]); 
    }
    
    printf("****************\r\n");
    rbuf_write(ming_rbuf, ming_data2, ARRAY_LEN(ming_data2), sizeof(int));
    print_all(ming_rbuf, 5);
    printf("space_left:%d\r\n", ming_rbuf->space_left);
    printf("write_cursor:%d\r\n", ming_rbuf->write_cursor);
    printf("read_cursor:%d\r\n", ming_rbuf->read_cursor);
    printf("page:%d\r\n", ming_rbuf->page_counter);
    printf("last_page:%d\r\n", ming_rbuf->last_read_page);

    rbuf_read(ming_rbuf, ming_get, 5, sizeof(int));
    for (i = 0; i < 5; i++) {
        printf("ming_get[%d]:%d\r\n", i, ming_get[i]); 
    }

    printf("****************\r\n");
    rbuf_write(ming_rbuf, ming_max, ARRAY_LEN(ming_max), sizeof(int));


    return 0;
}

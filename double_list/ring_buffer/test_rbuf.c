#include "ring_buffer.h"

#define ARRAY_LEN(ARRAY) (sizeof(ARRAY)/sizeof(ARRAY[0]))

void print_all (rbuf_t *thiz, int length) 
{
    int i = 0;

    for (i = 0; i < length * 4; i += 4) {
        printf("data:%d\r\n", *(int *)(thiz->_data + i));
    }
}

int main () 
{
    rbuf_t ming_rbuf;
    int int_buf[10] = {0};
 
    rbuf_create(&ming_rbuf, int_buf, sizeof(int), ARRAY_LEN(int_buf));
    
    print_all(&ming_rbuf, ARRAY_LEN(int_buf));

    int ming_data1[] = {1, 2, 3};
    int ming_data2[] = {4, 5};
    int ming_data3[]  = {1, 2, 3, 4, 5, 6};
    int ming_get[5] = {0};
    int i = 0;

    printf("****************\r\n");
    printf("insert data 1,2,3...!!\r\n");
    rbuf_write(&ming_rbuf, ming_data1, ARRAY_LEN(ming_data1));
    print_all(&ming_rbuf, ARRAY_LEN(int_buf));
    printf("space_left:%d\r\n", ming_rbuf._space_left);
    printf("write_cursor:%d\r\n", ming_rbuf._write_cursor);
    printf("read_cursor:%d\r\n", ming_rbuf._read_cursor);

    printf("****************\r\n");
    printf("reading 2 data ...!!\r\n");
    rbuf_read(&ming_rbuf, ming_get, 2);
    for (i = 0; i < 5; i++) {
        printf("ming_get[%d]:%d\r\n", i, ming_get[i]); 
    }
    printf("write_cursor:%d\r\n", ming_rbuf._write_cursor);
    printf("read_cursor:%d\r\n", ming_rbuf._read_cursor);

    printf("****************\r\n");
    printf("insert data 4,5,1,2,3...!!\r\n");
    rbuf_write(&ming_rbuf, ming_data2, ARRAY_LEN(ming_data2));
    rbuf_write(&ming_rbuf, ming_data1, ARRAY_LEN(ming_data1));
    print_all(&ming_rbuf, ARRAY_LEN(int_buf));
    printf("space_left:%d\r\n", ming_rbuf._space_left);
    printf("write_cursor:%d\r\n", ming_rbuf._write_cursor);
    printf("read_cursor:%d\r\n", ming_rbuf._read_cursor);

    printf("****************\r\n");
    printf("reading 5 data  ...!!\r\n");
    rbuf_read(&ming_rbuf, ming_get, 5);
    for (i = 0; i < 5; i++) {
        printf("ming_get[%d]:%d\r\n", i, ming_get[i]); 
    }
    printf("space_left:%d\r\n", ming_rbuf._space_left);
    printf("write_cursor:%d\r\n", ming_rbuf._write_cursor);
    printf("read_cursor:%d\r\n", ming_rbuf._read_cursor);

    printf("****************\r\n");
    printf("insert data 1,2,3,4,5,6 ...!!\r\n");
    rbuf_write(&ming_rbuf, ming_data3, ARRAY_LEN(ming_data3));
    print_all(&ming_rbuf, ARRAY_LEN(int_buf));
    printf("space_left:%d\r\n", ming_rbuf._space_left);
    printf("write_cursor:%d\r\n", ming_rbuf._write_cursor);
    printf("read_cursor:%d\r\n", ming_rbuf._read_cursor);

    printf("****************\r\n");
    printf("reading 5 data ...!!\r\n");
    rbuf_read(&ming_rbuf, ming_get, 5);
    for (i = 0; i < 5; i++) {
        printf("ming_get[%d]:%d\r\n", i, ming_get[i]); 
    }
    printf("space_left:%d\r\n", ming_rbuf._space_left);
    printf("write_cursor:%d\r\n", ming_rbuf._write_cursor);
    printf("read_cursor:%d\r\n", ming_rbuf._read_cursor);

    return 0;
}

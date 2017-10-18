#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

int main (void)
{
    int data = 0;
    void *got_data = NULL;
    
    array_t *ming_array = array_create(10); 

    printf("the array create successfully!and length is %d\r\n", ming_array->length);

    for (data = 10; data < 110; data += 10) {
        array_push(ming_array, &data, sizeof(data));
    }

    printf("write cursor:%d\r\n", ming_array->write);

    for (data = 10; data < 110; data += 10) {
        array_pop(ming_array, &got_data); 
	printf("got data:%d\r\n", *(int *)got_data);
    }

    printf("read cursor:%d\r\n", ming_array->read);
 
    if (array_destory(&ming_array) == ARRAY_NO_ERROR) {
//	if (ming_array == NULL) {
            printf("destory successfully!\r\n"); 
//	}
    }


    return 0;
}

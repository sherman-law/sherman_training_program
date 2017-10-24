#include "ring_buffer.h"

static void check_limit (rbuf_t *thiz)
{
    if (thiz->read_cursor >= thiz->total_length) {
        thiz->read_cursor = 0;	
    }

    if (thiz->write_cursor >= thiz->total_length) {
       thiz->write_cursor = 0; 
    }
    return;
}

static void get_write_cursor_before (rbuf_t *thiz, int length)
{
    return; 
}

rbuf_t *rbuf_create(uint16_t total_length, uint16_t size)
{
    rbuf_t *thiz = NULL;
    
    thiz = (rbuf_t *)malloc(sizeof(rbuf_t));

    if (NULL == thiz) {
        debug("create fail!in func:%s\r\n", __func__); 
	return NULL;
    } 

    thiz->data = malloc(total_length * size);

    if (NULL == thiz->data) {
        debug("create ring buffer's data fail!in func:%s\r\n", __func__); 
	return NULL;
    } 

    thiz->total_length   = total_length;
    thiz->space_left     = total_length;
    thiz->read_cursor    = 0;
    thiz->write_cursor   = 0;
    thiz->page_counter   = 0;
    thiz->last_read_page = 0;

    return thiz;
}

rbuf_status_t rbuf_write(rbuf_t *thiz, void *data, uint16_t length, int size)
{
    uint16_t temp_space = 0;

    if (NULL == thiz) {
        debug("thiz is NULL!in func:%s\r\n", __func__); 
	return R_BUF_NULL_ERROR;
    } 

    if (length > thiz->total_length) {
        debug("length of data is too big.in func:%s\r\n", __func__);
	return R_BUF_INVAIL_PARAM;
    }

    temp_space = thiz->space_left;

    /****************************************
     * have enough space to be written
     ****************************************/
    if (thiz->space_left >= length) {
        memcpy(thiz->data + (thiz->write_cursor * size), data, length * size);

        thiz->space_left   -= length;	
	thiz->write_cursor += length;

	check_limit(thiz);

	return R_BUF_NO_ERROR;
    } else {
#if 0
        if ((length / thiz->total_length) < 1) {
#endif            
    /****************************************
     * not have enough space to be written 
     * so data should be divided into pieces 
     * to save
     ****************************************/
            memcpy(thiz->data + (thiz->write_cursor * size), data, thiz->space_left * size);
 
            thiz->space_left   = thiz->total_length;	
	    thiz->write_cursor = 0;
            thiz->page_counter++;        

            memcpy(thiz->data + thiz->write_cursor, 
	           data + (temp_space * size), 
	          (length - temp_space) * size);
        
    	    thiz->space_left   -= (length - temp_space);
	    thiz->write_cursor += (length - temp_space);

	    check_limit(thiz);

	    return R_BUF_NO_ERROR;
#if 0
	} else {
            memcpy(thiz->data + thiz->write_cursor, 
                   data + length - thiz->total_length, 
		   temp_space * size);

            thiz->space_left   = thiz->total_length;	
	    thiz->write_cursor = 0;
            thiz->page++;        

	    memcpy(thiz->data + thiz->write_cursor,
	           data + length - thiz->total_length + temp_space,
                  (thiz->total_length - temp_space) * size);
	    
   	    thiz->space_left   -= (length - thiz->space_left);
	    thiz->write_cursor += (length - thiz->space_left);

	    return R_BUF_NO_ERROR;
	}
#endif
    }

}

void print_all (rbuf_t *thiz, int length) 
{
    int i = 0;

    for (i = 0; i < length * 4; i += 4) {
        printf("data:%d\r\n", *(int *)(thiz->data + i));
    }
}
//#if 0
uint16_t rbuf_read(rbuf_t *thiz, void *user_buffer, int length, int size)
{
    int new_data_num     = 0;
    uint16_t data_should_read = 0;

    if (NULL == thiz) {
        debug("thiz is NULL!in func:%s\r\n", __func__); 
	return 0;
    } 

    if (thiz->last_read_page == thiz->page_counter) {
        
        new_data_num = thiz->write_cursor - thiz->read_cursor;

   	debug("new data number is :%d\r\n", new_data_num);
	/* the page_counter change after new_data_num caculate */
	if (new_data_num <= 0) {

   	    debug("page has changed\r\n");
            uint16_t write_offset = thiz->write_cursor; 

            data_should_read = thiz->total_length - thiz->read_cursor;

            memcpy(user_buffer, 
	           thiz->data + thiz->read_cursor * size,
		   data_should_read * size);
	    memcpy(user_buffer + data_should_read * size,
    	           thiz->data,
		   write_offset * size);

	    thiz->read_cursor = write_offset;
	    thiz->last_read_page = thiz->page_counter;

	    return data_should_read + write_offset; 
	}
        
	if (new_data_num < length) {
            memcpy(user_buffer, 
                   thiz->data + (thiz->read_cursor * size),
                   new_data_num * size);	    

	    thiz->read_cursor += new_data_num;
	    check_limit(thiz);
            thiz->last_read_page = thiz->page_counter;
   	    return new_data_num;
	} else {
            memcpy(user_buffer, 
                   thiz->data + (thiz->read_cursor * size),
                   length * size);	    

	    thiz->read_cursor += length;
	    check_limit(thiz);
            thiz->last_read_page = thiz->page_counter;
	    return length;
	}

    } else if (thiz->last_read_page + 1 == thiz->page_counter) {
        uint16_t write_offset = thiz->write_cursor; 

	if (write_offset < thiz->read_cursor) {

            data_should_read = thiz->total_length - thiz->read_cursor;

            memcpy(user_buffer, 
	           thiz->data + thiz->read_cursor * size,
		   data_should_read * size);
	    memcpy(user_buffer + data_should_read * size,
    	           thiz->data,
		   write_offset * size);

	    thiz->read_cursor = write_offset;
	    thiz->last_read_page = thiz->page_counter;

	    return data_should_read + write_offset;           	
	} else {
     	    new_data_num = thiz->write_cursor;

            if (new_data_num < length) {
                memcpy(user_buffer, 
                       thiz->data,
                       new_data_num * size);	    

	        thiz->read_cursor = new_data_num;
	        check_limit(thiz);
                thiz->last_read_page = thiz->page_counter;
   	        return new_data_num;
            } else {
                memcpy(user_buffer, 
                       thiz->data,
                       length * size);	    

	        thiz->read_cursor = length;
	        check_limit(thiz);
                thiz->last_read_page = thiz->page_counter;
	        return length;
    	    }
	}   
    } else {
        new_data_num = thiz->write_cursor;

        if (new_data_num < length) {
            memcpy(user_buffer, 
                   thiz->data + (thiz->read_cursor * size),
                   new_data_num * size);	    

	    thiz->read_cursor = new_data_num;
	    check_limit(thiz);
            thiz->last_read_page = thiz->page_counter;
   	    return new_data_num;
	} else {
            memcpy(user_buffer, 
                   thiz->data + (thiz->read_cursor * size),
                   length * size);	    

	    thiz->read_cursor = length;
	    check_limit(thiz);
            thiz->last_read_page = thiz->page_counter;
	    return length;
	}
    } 
}





//#endif

#include "ring_buffer.h"

static void check_limit (rbuf_t *thiz)
{
    if (thiz->_read_cursor >= thiz->_total_length) {
        thiz->_read_cursor = 0;	
    }

    if (thiz->_write_cursor >= thiz->_total_length) {
       thiz->_write_cursor = 0; 
    }
    return;
}

rbuf_status_t rbuf_create(rbuf_t *thiz, void *buffer, uint16_t itemsize, uint16_t length)
{
    if (NULL == thiz) {                          /* check thiz pointer */
        debug("create fail!in func:%s\r\n", __func__); 
	return R_BUF_NULL_ERROR;
    } 

    /* initialize parameter of ring buffer */
    thiz->_data           = buffer;
    thiz->_total_length   = length;
    thiz->_space_left     = length;
    thiz->_itemsize       = itemsize;
    thiz->_read_cursor    = 0;
    thiz->_write_cursor   = 0;

    return R_BUF_NO_ERROR;
}

rbuf_status_t rbuf_write(rbuf_t *thiz, const void *data, uint16_t length)
{
    uint16_t to_copy  = 0;
    uint16_t had_copy = 0;

    if (NULL == thiz) {                          /* check thiz pointer */
        debug("thiz is NULL!in func:%s\r\n", __func__); 
	return R_BUF_NULL_ERROR;
    } 

    /* check whether the ring buffer has enough space to be written */
    if (length > thiz->_space_left) {
        debug("data is too big to insert to ring buf.in func:%s\r\n", __func__);
	return R_BUF_INVAIL_PARAM;
    }

    /* calculate the space from write_cursor to the end of ring_buffer */
    to_copy = thiz->_total_length - thiz->_write_cursor;
    if (to_copy > length) {
        to_copy = length;   /* if we have enough space to be written */
    }

    /* if we don't have enough space,we write to the end first */
    memcpy(thiz->_data + thiz->_write_cursor * thiz->_itemsize, 
           data, 
	   to_copy * thiz->_itemsize);
    thiz->_space_left -= to_copy;
    
    /* calculate the rest of data */
    had_copy += to_copy;
    thiz->_write_cursor += to_copy;
    length -= to_copy;
    if (length > 0) {
        /* write the rest of data to ring buffer */
        memcpy(thiz->_data, 
               data + had_copy * thiz->_itemsize, 
	       length * thiz->_itemsize); 
	thiz->_write_cursor = length;
        thiz->_space_left -= length;
    }
    check_limit(thiz);

    return R_BUF_NO_ERROR;
}

rbuf_status_t rbuf_read(rbuf_t *thiz, void *data, uint16_t length)
{
    uint16_t to_copy  = 0;
    uint16_t had_copy = 0;
    uint16_t data_num = thiz->_total_length - thiz->_space_left;

    if (NULL == thiz) {
        debug("thiz is NULL!in func:%s\r\n", __func__); 
	return 0;
    } 

    if (length > data_num) {
        if (data_num == 0) {
            return R_BUF_EMPTY;	
	}
	/* only have data_num bytes of data */
        length = data_num;
    }

    to_copy = thiz->_total_length - thiz->_read_cursor;
    if (to_copy > length) {
        to_copy = length; 
    }
    memcpy(data, 
           thiz->_data + thiz->_read_cursor * thiz->_itemsize, 
	   to_copy * thiz->_itemsize);
    thiz->_space_left += to_copy;

    had_copy += to_copy;
    thiz->_read_cursor += to_copy;
    length -= to_copy;
    if (length > 0) {
        memcpy(data + had_copy * thiz->_itemsize, 
               thiz->_data, 
	       length * thiz->_itemsize);
	thiz->_read_cursor = length;
        thiz->_space_left += length;
    }
    check_limit(thiz);

    return R_BUF_NO_ERROR;
}





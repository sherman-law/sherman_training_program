#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#include "../common.h"

typedef int rbuf_status_t;
#define RBUF_NULL_ERROR   0
#define RBUF_NO_ERROR     1
#define RBUF_INVAIL_PARAM 2
#define RBUF_FAIL         3 
#define RBUF_EMPTY        4 

typedef struct _rbuf_t {
    void     *_data;         /* point to the memory that storage user's data */
    uint16_t  _total_length; /* length of ring_buf */
    uint16_t  _space_left;   /* length of free space */
    uint16_t  _itemsize;     /* size of item */

    uint16_t  _read_cursor;  /* read_cursor */
    uint16_t  _write_cursor; /* write_cursor */
} rbuf_t;

/**
 * \brief  ring buffer creating function
 *
 * \param[in,out] thiz    :thiz pointer pointe to rbuf_t object
 * \param[in]     buffer  :source place address to storage data
 * \param[in]     itemsize:size of one item
 * \param[in]     length  :length of this ring buffer
 *
 * \retval error code
 */
rbuf_status_t rbuf_create(rbuf_t *thiz, void *buffer, uint16_t itemsize, uint16_t length);

/**
 * \brief  ring buffer writing function
 *
 * \param[in,out] thiz    :thiz pointer pointe to rbuf_t object
 * \param[in]     data    :source data address which should be written
 * \param[in]     length  :length of data should be written
 *
 * \retval error code
 */
rbuf_status_t rbuf_write(rbuf_t *thiz, const void *data, uint16_t length);

/**
 * \brief  ring buffer reading function
 *
 * \param[in,out] thiz    :thiz pointer pointe to rbuf_t object
 * \param[in]     data    :source data address which should be read
 * \param[in]     length  :length of data should be read
 *
 * \retval error code
 */
rbuf_status_t rbuf_read(rbuf_t *thiz, void *data, uint16_t length);

/**
 * \brief  ring buffer reset function
 *
 * \param[in,out] thiz :thiz pointer pointe to rbuf_t object
 *
 * \retval error code
 */
rbuf_status_t rbuf_reset (rbuf_t *thiz);

#endif


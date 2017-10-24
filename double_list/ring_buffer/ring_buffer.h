#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#include "../common.h"

typedef int rbuf_status_t;
#define R_BUF_NULL_ERROR   0
#define R_BUF_NO_ERROR     1
#define R_BUF_INVAIL_PARAM 2
#define R_BUF_FAIL         3 

typedef struct _rbuf_t {
    void *data;
    uint16_t total_length;
    uint16_t space_left;

    uint8_t page_counter;
    uint8_t last_read_page;

    uint16_t read_cursor;
    uint16_t write_cursor;
} rbuf_t;

rbuf_t *rbuf_create(uint16_t total_length, uint16_t size);

rbuf_status_t rbuf_write(rbuf_t *thiz, void *data, uint16_t length, int size);

uint16_t rbuf_read(rbuf_t *thiz, void *user_buffer, int length, int size);

void print_all (rbuf_t *thiz, int length); 

#endif


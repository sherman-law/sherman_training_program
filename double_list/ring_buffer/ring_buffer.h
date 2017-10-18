#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

typedef int rbuf_status_t;
#define R_BUF_NULL_ERROR 0
#define R_BUF_NO_ERROR   1
#define R_BUF_GOT_RESULT 2
#define R_BUF_FAIL       3 

typedef struct _rbuf_t {
    dlink_list_t *list;
    list_node_t *read;
    list_node_t *write;
} rbuf_t;

rbuf_t *r_buf_create(void);

rbuf_status_t rbuf_push();

rbuf_status_t rbuf_pop();

rbuf_status_t

#endif

#ifndef __ITERATOR_LIST_H
#define __ITERATOR_LIST_H

#include "../double_list.h"
#include "../common.h"
#include "iterator.h"

iterator_t *dlist_iterator_create(dlink_list_t *list);

#endif

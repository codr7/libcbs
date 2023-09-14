#ifndef CDEQUE_H
#define CDEQUE_H

#include "cbs/list.h"

struct cdeque {
  uint32_t item_count, item_size, length;
  struct clist blocks, free_blocks;
};

struct cdeque *cdeque_init(struct cdeque *d, uint32_t item_size, uint32_t block_size);
struct cdeque *cdeque_deinit(struct cdeque *d);
void *cdeque_push_front(struct cdeque *d);
void *cdeque_push_back(struct cdeque *d);
void *cdeque_pop_back(struct cdeque *d);
void *cdeque_get(struct cdeque *d, uint32_t i);

#endif

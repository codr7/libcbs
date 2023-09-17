#ifndef CMALLOC_H
#define CMALLOC_H

#include <stddef.h>
#include <stdint.h>
#include "cbs/list.h"
#include "cbs/vector.h"

struct cmalloc {
  struct clist slabs;
  struct cvector free_slots;
  size_t slot_count, slot_size, i;
};

struct cmalloc *cmalloc_init(struct cmalloc *m, size_t slot_count, size_t slot_size);
struct cmalloc *cmalloc_deinit(struct cmalloc *m);

void *cmalloc_acquire(struct cmalloc *m);
void cmalloc_release(struct cmalloc *m, void *p);
  
#endif

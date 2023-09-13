#ifndef CSLAB_H
#define CSLAB_H

#include <stddef.h>
#include <stdint.h>
#include "cbs/list.h"
#include "cbs/vector.h"

struct cslab_alloc {
  struct clist slabs;
  struct cvector free_slots;
  size_t slot_count, slot_size, i;
};

struct cslab_alloc *cslab_alloc_init(struct cslab_alloc *a, size_t slot_count, size_t slot_size);
struct cslab_alloc *cslab_alloc_deinit(struct cslab_alloc *a);

void *cslab_malloc(struct cslab_alloc *a);
void cslab_free(struct cslab_alloc *a, void *p);
  
#endif

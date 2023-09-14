#include <stdlib.h>
#include "cbs/slab.h"

struct slab {
  struct clist list;
  uint8_t slots[];
};

struct cslab_alloc *cslab_alloc_init(struct cslab_alloc *a, size_t slot_count, size_t slot_size) {
  clist_init(&a->slabs);
  cvector_init(&a->free_slots, sizeof(uint8_t *));
  a->slot_count = slot_count;
  a->slot_size = slot_size;
  a->i = slot_count;
  return a;
}

struct cslab_alloc *cslab_alloc_deinit(struct cslab_alloc *a) {
  CLIST_DO(&a->slabs, sl) {
    struct slab *s = CBASEOF(sl, struct slab, list);
    free(s);
  }

  cvector_deinit(&a->free_slots);
  return a;
}

static struct slab *get_slab(struct cslab_alloc *a) {
  if (a->i == a->slot_count) {
    struct slab *s = malloc(sizeof(struct slab) + a->slot_size * a->slot_count);
    clist_push_back(&a->slabs, &s->list);
    a->i = 0;
    return s;
  }

  return CBASEOF(a->slabs.prev, struct slab, list);
}

void *cslab_malloc(struct cslab_alloc *a) {
  return a->free_slots.length
    ? *(uint8_t **)cvector_pop(&a->free_slots)
    : get_slab(a)->slots + a->i++ * a->slot_size;
}

void cslab_free(struct cslab_alloc *a, void *p) {
  *(uint8_t **)cvector_push(&a->free_slots) = p;
}

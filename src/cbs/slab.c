#include <stdlib.h>
#include "cbs/slab.h"

struct cslab {
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
    struct cslab *s = CBASEOF(sl, struct cslab, list);
    free(s);
  }

  cvector_deinit(&a->free_slots);
  return a;
}

static struct cslab *cslab_get(struct cslab_alloc *a) {
  if (a->i == a->slot_count) {
    struct cslab *s = malloc(sizeof(struct cslab) + a->slot_size * a->slot_count);
    clist_push_back(&a->slabs, &s->list);
    a->i = 0;
    return s;
  }

  return CBASEOF(a->slabs.prev, struct cslab, list);
}

void *cslab_malloc(struct cslab_alloc *a) {
  return a->free_slots.length
    ? *(uint8_t **)cvector_pop(&a->free_slots)
    : cslab_get(a)->slots + a->i++ * a->slot_size;
}

void cslab_free(struct cslab_alloc *a, void *p) {
  *(uint8_t **)cvector_push(&a->free_slots) = p;
}

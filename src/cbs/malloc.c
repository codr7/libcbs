#include <stdlib.h>
#include "cbs/malloc.h"

struct slab {
  struct clist list;
  uint8_t slots[];
};

struct cmalloc *cmalloc_init(struct cmalloc *m, size_t slot_count, size_t slot_size) {
  clist_init(&m->slabs);
  cvector_init(&m->free_slots, sizeof(uint8_t *));
  m->slot_count = slot_count;
  m->slot_size = slot_size;
  m->i = slot_count;
  return m;
}

struct cmalloc *cmalloc_deinit(struct cmalloc *m) {
  CLIST_DO(&m->slabs, sl) {
    struct slab *s = CBASEOF(sl, struct slab, list);
    free(s);
  }

  cvector_deinit(&m->free_slots);
  return m;
}

static struct slab *get_slab(struct cmalloc *m) {
  if (m->i == m->slot_count) {
    struct slab *s = malloc(sizeof(struct slab) + m->slot_size * m->slot_count);
    clist_push_back(&m->slabs, &s->list);
    m->i = 0;
    return s;
  }

  return CBASEOF(m->slabs.prev, struct slab, list);
}

void *cmalloc_acquire(struct cmalloc *m) {
  return m->free_slots.length
    ? *(uint8_t **)cvector_pop(&m->free_slots)
    : get_slab(m)->slots + m->i++ * m->slot_size;
}

void cmalloc_release(struct cmalloc *m, void *p) {
  *(uint8_t **)cvector_push(&m->free_slots) = p;
}

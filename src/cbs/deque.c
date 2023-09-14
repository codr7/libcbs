#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cbs/deque.h"

struct block {
  struct clist list;
  uint8_t length;
  uint8_t items[];
};

static struct block *block_alloc(struct cdeque *d) {
  struct block *b = malloc(sizeof(struct block) + (d->item_count+1)*d->item_size);
  b->length = 0;
  return b;
}

static void *block_push_front(struct cdeque *d, struct block *b) {
  uint8_t *p = CALIGN(b->items, d->item_size);
  memmove(p + d->item_size, p, d->item_size * b->length++);
  return p;
}

static void *block_push_back(struct cdeque *d, struct block *b) {
  return CALIGN(b->items, d->item_size) + d->item_size * b->length++;
}

static void *block_get(struct cdeque *d, struct block *b, uint8_t i) {
  return CALIGN(b->items, d->item_size) + i*d->item_size;
}

static void *block_pop(struct cdeque *d, struct block *b) {
  return block_get(d, b, --b->length);
}

struct cdeque *cdeque_init(struct cdeque *d, uint32_t item_count, uint32_t item_size) {
  d->item_count = item_count;
  d->item_size = item_size;
  d->length = 0;
  
  clist_init(&d->blocks);
  clist_init(&d->free_blocks);
  return d;
}

struct cdeque *cdeque_deinit(struct cdeque *d) {
  CLIST_DO(&d->blocks, bl) { free(CBASEOF(bl, struct block, list)); }
  CLIST_DO(&d->free_blocks, bl) { free(CBASEOF(bl, struct block, list)); }
  return d;
}

void *cdeque_push_front(struct cdeque *d) {
  struct clist *bl = clist_peek_front(&d->blocks);
  struct block *b = bl ? CBASEOF(bl, struct block, list) : NULL;

  if (!b || b->length == d->item_count) {
    b = block_alloc(d);
    clist_push_front(&d->blocks, &b->list);
  }

  d->length++;
  return block_push_front(d, b);
}

void *cdeque_push_back(struct cdeque *d) {
  struct clist *bl = clist_peek_back(&d->blocks);
  struct block *b = bl ? CBASEOF(bl, struct block, list) : NULL;

  if (!b || b->length == d->item_count) {
    b = block_alloc(d);
    clist_push_back(&d->blocks, &b->list);
  }

  d->length++;
  return block_push_back(d, b);
}

void *cdeque_pop_back(struct cdeque *d) {
  struct clist *bl = clist_peek_back(&d->blocks);
  struct block *b = CBASEOF(bl, struct block, list);
  void *p = block_pop(d, b);

  if (!b->length) {
    clist_delete(&b->list);
    clist_push_back(&d->free_blocks, &b->list);
  }

  d->length--;
  return p;
}

void *cdeque_get(struct cdeque *d, uint32_t i) {
  CLIST_DO(&d->blocks, bl) {
    struct block *b = CBASEOF(bl, struct block, list);
    if (i < b->length) { return block_get(d, b, i); }
    i -= b->length;
  }

  return NULL;
}

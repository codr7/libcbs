#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cbs/deque.h"

struct block {
  struct clist list;
  uint8_t front, back;
  uint8_t items[];
};

static struct block *alloc_block(struct cdeque *d) {
  struct clist *bl = clist_pop_front(&d->free_blocks);
  if (bl) { return CBASEOF(bl, struct block, list); }
  struct block *b = malloc(sizeof(struct block) + (d->item_count+1)*d->item_size);
  b->front = b->back = 0;
  return b;
}

static void *block_push_front(struct cdeque *d, struct block *b) {
  uint8_t *p = CALIGN(b->items, d->item_size);

  if (b->back == 0) {
    b->back++;
    return p;
  }
  
  if (b->front > 0) { return p + --b->front * d->item_size; }

  memmove(p + d->item_size, p, d->item_size * (b->back-b->front));
  b->back++;
  return p;
}

static void *block_get(struct cdeque *d, struct block *b, uint8_t i) {
  return CALIGN(b->items, d->item_size) + i*d->item_size;
}

static void *block_pop_front(struct cdeque *d, struct block *b) {
  return block_get(d, b, b->front++);
}

static void *block_push_back(struct cdeque *d, struct block *b) {
  uint8_t *p = CALIGN(b->items, d->item_size);
  if (b->back < d->item_count) { return p + b->back++*d->item_size; }
  p += b->front*d->item_size;
  memmove(p-d->item_size, p, d->item_size * (b->back-b->front--));
  return CALIGN(b->items, d->item_size) + (b->back-1)*d->item_size;
}

static void *block_pop_back(struct cdeque *d, struct block *b) {
  return block_get(d, b, --b->back);
}

static uint8_t block_length(struct block *b) { return b->back - b->front; }

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

  if (!b || block_length(b) == d->item_count) {
    b = alloc_block(d);
    clist_push_front(&d->blocks, &b->list);
  }

  d->length++;
  return block_push_front(d, b);
}

void *cdeque_pop_front(struct cdeque *d) {
  struct clist *bl = clist_peek_front(&d->blocks);
  struct block *b = CBASEOF(bl, struct block, list);
  void *p = block_pop_front(d, b);
  if (!block_length(b)) { clist_push_back(&d->free_blocks, clist_delete(&b->list)); }
  d->length--;
  return p;
}

void *cdeque_push_back(struct cdeque *d) {
  struct clist *bl = clist_peek_back(&d->blocks);
  struct block *b = bl ? CBASEOF(bl, struct block, list) : NULL;

  if (!b || block_length(b) == d->item_count) {
    b = alloc_block(d);
    clist_push_back(&d->blocks, &b->list);
  }

  d->length++;
  return block_push_back(d, b);
}

void *cdeque_pop_back(struct cdeque *d) {
  struct clist *bl = clist_peek_back(&d->blocks);
  struct block *b = CBASEOF(bl, struct block, list);
  void *p = block_pop_back(d, b);
  if (!block_length(b)) { clist_push_back(&d->free_blocks, clist_delete(&b->list)); }
  d->length--;
  return p;
}

void *cdeque_get(struct cdeque *d, uint32_t i) {
  CLIST_DO(&d->blocks, bl) {
    struct block *b = CBASEOF(bl, struct block, list);
    uint8_t b_length = block_length(b);
    if (i < b_length) { return block_get(d, b, i); }
    i -= b_length;
  }

  return NULL;
}

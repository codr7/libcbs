#include <stdlib.h>
#include <string.h>

#include "cbs/vector.h"

struct cvector *cvector_init(struct cvector *v, size_t item_size) {
  v->item_size = item_size;
  v->capacity = v->length = 0;
  v->items = v->start = v->end = NULL;
  return v;
}

void cvector_deinit(struct cvector *v) {
  if (v->items) { free(v->items); }
}

void cvector_grow(struct cvector *v, size_t capacity) {
  v->capacity = capacity ? capacity : 2;
  v->items = realloc(v->items, v->item_size*(v->capacity+1));
  v->start = CALIGN(v->items, v->item_size);
  v->end = v->start + v->item_size*v->length;
}

void cvector_clear(struct cvector *v) {
  v->length = 0;
  v->end = v->start;
}

void *cvector_get(struct cvector *v, size_t i) {
  return v->items ? v->start + v->item_size*i : NULL;
}

void *cvector_push(struct cvector *v) {
  if (v->length == v->capacity) { cvector_grow(v, v->capacity*2); }
  void *p = v->end;
  v->end += v->item_size;
  v->length++;
  return p;
}

void *cvector_peek(struct cvector *v) {
  return v->length ? v->end - v->item_size : NULL;
}

void *cvector_pop(struct cvector *v) {
  if (!v->length) { return NULL; }
  v->end -= v->item_size;
  v->length--;
  return v->end;
}

void *cvector_insert(struct cvector *v, size_t i) {
  if (v->length == v->capacity) { cvector_grow(v, v->capacity*2); } 
  uint8_t *const p = cvector_get(v, i);
  memmove(p+v->item_size, p, (v->length-i)*v->item_size);
  v->length++;
  v->end += v->item_size;
  return p;
}

bool cvector_delete(struct cvector *v, size_t i, size_t n) {
  if (v->length < i+n) { return false; }

  if (i+n < v->length) {
    uint8_t *const p = cvector_get(v, i);
    size_t s = v->item_size*n;
    memmove(p, p + s, s);
  }

  v->length -= n;
  v->end -= n*v->item_size;
  return true;
}

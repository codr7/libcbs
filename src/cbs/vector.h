#ifndef CVECTOR_H
#define CVECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "cbs/macros.h"

#define _CVECTOR_DO(_v, _type, var, _i)		\
  size_t _i = 0;				\
  for (_type var = (_type)(_v)->start;	\
       _i < (_v)->length;			\
       _i++, var++)

#define CVECTOR_DO(_v, _type, var)		\
  _CVECTOR_DO(_v, _type, var, CUNIQUE(i))

struct cvector {
  size_t item_size, capacity, length;
  uint8_t *items, *start, *end;
};

struct cvector *cvector_init(struct cvector *v, size_t item_size);
void cvector_deinit(struct cvector *v);
void cvector_grow(struct cvector *v, size_t capacity);
void cvector_clear(struct cvector *v);
void *cvector_get(struct cvector *v, size_t i);
void *cvector_push(struct cvector *v);
void *cvector_peek(struct cvector *v);
void *cvector_pop(struct cvector *v);
void *cvector_insert(struct cvector *v, size_t i);
bool cvector_delete(struct cvector *v, size_t i, size_t n);

#endif

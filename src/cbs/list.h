#ifndef CLIST_H
#define CLIST_H

#include "cbs/macros.h"

#define _CLIST_DO(_l, i, _next)				\
  for (struct clist *i = (_l)->next, *_next = i->next;	\
       i != (_l);					\
       i = _next, _next = i->next)

#define CLIST_DO(_l, i)				\
  _CLIST_DO(_l, i, CUNIQUE(next))

struct clist {
  struct clist *prev, *next;
};

void clist_init(struct clist *l);
struct clist *clist_delete(struct clist *l);

void clist_push_front(struct clist *l, struct clist *it);
struct clist *clist_pop_front(struct clist *l);
struct clist *clist_peek_front(struct clist *l);

void clist_push_back(struct clist *l, struct clist *it);
struct clist *clist_pop_back(struct clist *l);
struct clist *clist_peek_back(struct clist *l);

#endif

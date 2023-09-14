#include <stddef.h>
#include "cbs/list.h"

void clist_init(struct clist *l) {
  l->prev = l->next = l;
}

struct clist *clist_delete(struct clist *l) {
  l->prev->next = l->next;
  l->next->prev = l->prev;
  return l;
}

void clist_push_front(struct clist *l, struct clist *it) {
  clist_push_back(l->next, it);
}

struct clist *clist_pop_front(struct clist *l) {
  struct clist *it = l->next;
  return (it == l) ? NULL : clist_delete(it);
}

struct clist *clist_peek_front(struct clist *l) {
  struct clist *it = l->next;
  return (it == l) ? NULL : it;
}

void clist_push_back(struct clist *l, struct clist *it) {
  it->prev = l->prev;
  l->prev->next = it;
  it->next = l;
  l->prev = it;
}

struct clist *clist_pop_back(struct clist *l) {
  struct clist *it = l->prev;
  return (it == l) ? NULL : clist_delete(it);
}

struct clist *clist_peek_back(struct clist *l) {
  struct clist *it = l->prev;
  return (it == l) ? NULL : it;
}

#include <stdbool.h>
#include "cbs/fail.h"
#include "cbs/util.h"

__thread struct clist errors;

struct cerror *cpush_error(struct cerror *e) {
  static __thread bool first = true;
  if (first) { clist_init(&errors); }
  clist_push_back(&errors, &e->list);
  return e;
}

struct cerror *cpop_error() {
  struct clist *el = clist_pop_front(&errors);
  return el ? CBASEOF(el, struct cerror, list) : NULL;
}

struct cerror *cerror_init(struct cerror *e,
			   const char *file, int line,
			   int code,
			   const char *spec, ...) {
  e->file = cstrdup(file);
  e->line = line;
  e->code = code;
  
  va_list args;
  va_start(args, spec);
  e->message = cformat_va(spec, args);
  va_end(args);

  return e;
}

struct cerror *cerror_deinit(struct cerror *e) {
  free(e->file);
  free(e->message);
  return e;
}

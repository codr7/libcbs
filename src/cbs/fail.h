#ifndef CFAIL_H
#define CFAIL_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "cbs/list.h"

#define CPUSH_ERROR(code, spec, ...)				\
  cpush_error(cerror_init(malloc(sizeof(struct cerror)),	\
			  __FILE__, __LINE__,			\
			  code,					\
			  spec, ##__VA_ARGS__, NULL));

struct cerror {
  struct clist list;

  char *file;
  int line;

  int code;
  char *message;
};

extern __thread struct clist errors;

struct cerror *cpush_error(struct cerror *e);
struct cerror *cpop_error();

struct cerror *cerror_init(struct cerror *e,
			   const char *file, int line,
			   int code,
			   const char *spec, ...);

struct cerror *cerror_deinit(struct cerror *e);

#endif

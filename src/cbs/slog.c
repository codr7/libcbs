#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "cbs/macros.h"
#include "cbs/slog.h"
#include "cbs/string.h"
#include "cbs/time.h"

void cslog_int_t(struct cslog *s, struct cslog_field *f) {
  int v = *(int *)CALIGN(f->value, sizeof(int));
  fprintf(s->out, "%s=%d", f->name, v);
}

struct cslog_field *cslog_int(const char *name, int value) {
  struct cslog_field *f = malloc(sizeof(struct cslog_field) + CALIGN(sizeof(int), sizeof(int)));
  strcpy(f->name, name);
  f->type = cslog_int_t;
  *(int *)CALIGN(f->value, sizeof(int)) = value;
  return f;
}

void cslog_string_t(struct cslog *s, struct cslog_field *f) {
  struct cstring *v = (struct cstring *)CALIGN(f->value, sizeof(struct cstring));
  fprintf(s->out, "%s=\"%s\"", f->name, v->data);
}

struct cslog_field *cslog_string(const char *name, const char *value) {
  const size_t length = strlen(value);
  const size_t ss = CSTRING_SIZE(length); 
  struct cslog_field *f = malloc(sizeof(struct cslog_field) + CALIGN(ss, ss));
  strcpy(f->name, name);
  f->type = cslog_string_t;
  cstring_init((struct cstring *)CALIGN(f->value, sizeof(struct cstring)), value, length);
  return f;
}

void cslog_timestamp_t(struct cslog *s, struct cslog_field *f) {
  fprintf(s->out, "%s=", f->name);
  struct timespec *v = (struct timespec *)CALIGN(f->value, sizeof(struct timespec));
  cput_timestamp(*v, s->out);
}

struct cslog_field *cslog_timestamp(const char *name, struct timespec value) {
  struct cslog_field *f = malloc(sizeof(struct cslog_field) +
				 CALIGN(sizeof(struct timespec), sizeof(struct timespec)));
  strcpy(f->name, name);
  f->type = cslog_timestamp_t;
  *(struct timespec *)CALIGN(f->value, sizeof(struct timespec)) = value;
  return f;
}

struct cslog *cslog_init(struct cslog *s, FILE *out, enum cslog_format format) {
  s->out = out;
  s->format = format;
  return s;
}

struct cslog *cslog_deinit(struct cslog *s) {
  return s;
}

void cslog_write(struct cslog *s, const char *message, ...) {
  va_list args;
  va_start(args, message);

  for(int i = 0;;i++) {
    struct cslog_field *f = va_arg(args, struct cslog_field *);
    if (!f) { break; }
    if (i) { fputs(", ", s->out); }
    f->type(s, f);
    free(f);
  }

  va_end(args);
  fputc('\n', s->out);
}

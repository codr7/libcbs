#ifndef CSLOG_H
#define CSLOG_H

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "cbs/time.h"

#define CSLOG(_s, _m, ...)					\
  cslog_write((_s), (_m),					\
	      cslog_timestamp("timestamp", cnow()),		\
	      cslog_string("message", _m), ##__VA_ARGS__, NULL)

enum cslog_format {CSLOG_TEXT};
  
struct cslog {
  FILE *out;
  enum cslog_format format;
};

struct cslog_field;
typedef void (*cslog_field_t)(struct cslog *, struct cslog_field *);

void cslog_int_t(struct cslog *s, struct cslog_field *f);
struct cslog_field *cslog_int(const char *name, int value);

void cslog_string_t(struct cslog *s, struct cslog_field *f);
struct cslog_field *cslog_string(const char *name, const char *value);

void cslog_timestamp_t(struct cslog *s, struct cslog_field *f);
struct cslog_field *cslog_timestamp(const char *name, struct timespec value);

struct cslog_field {
  char name[64];
  cslog_field_t type;
  uint8_t value[];
};
  
struct cslog *cslog_init(struct cslog *s, FILE *out, enum cslog_format format);
struct cslog *cslog_deinit(struct cslog *s);
void cslog_write(struct cslog *s, const char *message, ...);

#endif

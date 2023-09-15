#ifndef CTIME_H
#define CTIME_H

#include <stdio.h>
#include <time.h>

struct timespec cnow();
void cput_timestamp(struct timespec ts, FILE *out);

#endif

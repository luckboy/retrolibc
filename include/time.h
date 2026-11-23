/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _TIME_H
#define _TIME_H

#include <bits/types.h>
#include <stddef.h>

#ifndef _TIME_T
#define _TIME_T
typedef __time_t time_t;
#endif

struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
  long tm_gmtoff;
  const char *tm_zone;
};

#ifdef __cplusplus
extern "C" {
#endif

extern int daylight;
extern long timezone;
extern char *tzname[2];

extern int getdate_err;

char *asctime(const struct tm *tm);
char *asctime_r(const struct tm *tm, char *buf);
char *ctime(const time_t *time);
char *ctime_r(const time_t *time, char *buf);
double difftime(time_t time1, time_t time0);
struct tm *getdate(const char *s);
struct tm *gmtime(const time_t *time);
struct tm *gmtime_r(const time_t *time, struct tm *tm);
struct tm *localtime(const time_t *time);
struct tm *localtime_r(const time_t *time, struct tm *tm);
time_t mktime(struct tm *tm);
size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *tm);
char *strptime(const char *buf, const char *format, struct tm *tm);
void tzset(void);

#ifdef __cplusplus
}
#endif

#endif

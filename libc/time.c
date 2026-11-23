/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <stdio.h>
#ifndef TEST
#include <time.h>
#else
#define RETROLIBC_TIME
#include "retrolibc.h"
#endif

extern const char *__abbreviated_month_names[12];

extern const char *__abbreviated_week_day_names[7];

#ifdef L_asctime
char *asctime(const struct tm *tm)
{
  static char buf[26];
  return asctime_r(tm, buf);
}
#endif

#ifdef L_asctime_r
char *asctime_r(const struct tm *tm, char *buf)
{
  snprintf(buf, 26, "%s %s%3d %.2d:%.2d:%.2d %.d\n",
    __abbreviated_week_day_names[tm->tm_wday >= 0 ? (tm->tm_wday <= 6 ? tm->tm_wday : 6) : 0],
    __abbreviated_month_names[tm->tm_mon % 12 ? (tm->tm_mon <= 11 ? tm->tm_mon : 11) : 0],
    tm->tm_mday,
    tm->tm_hour, tm->tm_min, tm->tm_sec,
    tm->tm_year + 1900);
  return buf;
}
#endif

#ifdef L_ctime
char *ctime(const time_t *time)
{
  static char buf[26];
  return ctime_r(time, buf);
}
#endif

#ifdef L_ctime_r
char *ctime_r(const time_t *time, char *buf)
{
  struct tm tm;
  return asctime_r(localtime_r(time, &tm), buf);
}
#endif

#ifdef L_difftime
double difftime(time_t time1, time_t time0)
{ return time1 - time0; }
#endif

#ifdef L_gmtime
struct tm *gmtime(const time_t *time)
{
  static struct tm tm;
  return gmtime_r(time, &tm);
}
#endif

#ifdef L_localtime
struct tm *localtime(const time_t *time)
{
  static struct tm tm;
  return localtime_r(time, &tm);
}
#endif

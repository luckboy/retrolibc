/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _PRIV_GMTIME_R_H
#define _PRIV_GMTIME_R_H

#include <limits.h>
#include <time.h>

#define TIME_T_MAX                      LONG_MAX
#define TIME_T_MIN                      LONG_MIN

struct tz_rule
{
  char type;
  union {
    short n;
    struct
    {
      signed char m;
      signed char w;
      signed char d;
    } m;
  } u;
  long time;
};

#ifndef TEST
int __cmp_tm_with_tz_rule(const struct tm *tm, struct tz_rule *rule);
time_t __mktime_no_tzset(const struct tm *tm);
#endif

#endif

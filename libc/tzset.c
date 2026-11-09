/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <ctype.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "priv_tzset.h"

#define TZ              "TZ"

pthread_mutex_t __tz_mutex = PTHREAD_MUTEX_INITIALIZER;
long __tz_std_offset = 0L;
const char *__tz_std_zone = NULL;
long __tz_dst_offset = 0L;
const char *__tz_dst_zone = NULL;
int __are_tz_rules = 0;
struct tz_rule __tz_rule_std_to_dst;
struct tz_rule __tz_rule_dst_to_std;

int daylight = 0;
long timezone = 0L;
char *tzname[2] = { NULL, NULL };

static int parse_int(const char *s, char **endptr, int max_digits)
{
  int n, is_digit = 0;
  for(n = 0; *s != '\0' && max_digits > 0; s++, max_digits--) {
    if(isdigit(*s)) {
      n = n * 10 + *s - '0';
      is_digit = 1;
    } else {
      break;
    }
  }
  if(!is_digit) return -1;
  *endptr = (char *) s;
  return n;
}

static long parse_time(const char *s, long *time, char **endptr)
{
  long hour, min, sec;
  hour = parse_int(s, (char **) (&s), 2);
  if(hour > 23) return -1L;
  if(*s != ':') return -1L;
  s++;
  min = parse_int(s, (char **) (&s), 2);
  if(hour > 59) return -1L;
  if(*s != ':') return -1L;
  s++;
  sec = parse_int(s, (char **) (&s), 2);
  if(sec > 59) return -1L;
  *endptr = (char *) s;
  *time = hour * 60 * 60 + min * 60 + sec;
  return 0;
}

static int parse_offset(const char *s, long *offset, char **endptr)
{
  int sign = 1;
  if(*s == '+') {
    s++;
  } else if(*s == '-') {
    sign = -1;
    s++;
  }
  if(parse_time(s, offset, (char **) (&s)) == -1) return -1;
  *offset *= sign;
  return 0;
}

static void parse_tzname(const char *s, char *tzname, char **endptr)
{
  int n;
  for(n = TZNAME_MAX; n > 0 && isupper(*s); s++, tzname++, n--) *tzname = *s;
  *tzname = '\0';
  *endptr = (char *) s;
}

static int parse_tz_rule(const char *s, struct tz_rule *rule, char **endptr)
{
  struct tz_rule tmp_rule;
  if(*s =='J' || *s == 'M') {
    tmp_rule.type = *s;
    s++;
  } else {
    tmp_rule.type = '\0';
  }
  switch(tmp_rule.type) {
  case '\0':
  case 'J':
    tmp_rule.u.n = parse_int(s, (char **) (&s), 3);
    if(tmp_rule.u.n < (tmp_rule.type == 'J' ? 1 : 0) || tmp_rule.u.n > 365) return -1;
    break;
  case 'M':
    tmp_rule.u.m.m = parse_int(s, (char **) (&s), 3);
    if(tmp_rule.u.m.m < 1 || tmp_rule.u.m.m > 12) return -1;
    if(*s != '.') return -1;
    s++;
    tmp_rule.u.m.w = parse_int(s, (char **) (&s), 1);
    if(tmp_rule.u.m.w < 1 || tmp_rule.u.m.w > 5) return -1;
    if(*s != '.') return -1;
    s++;
    tmp_rule.u.m.w = parse_int(s, (char **) (&s), 1);
    if(tmp_rule.u.m.w > 6) return -1;
    break;
  }
  if(*s == '/') {
     if(parse_time(s, &(tmp_rule.time), (char **) (&s)) == -1) return -1;
  } else {
    tmp_rule.time = 2 * 60 * 60;
  }
  *endptr = (char *) s;
  *rule = tmp_rule;
  return 0;
}

static int parse_tz(const char *s)
{
  static char std[TZNAME_MAX + 1];
  static char dst[TZNAME_MAX + 1];
  char tmp_std[TZNAME_MAX + 1];
  char tmp_dst[TZNAME_MAX + 1];
  long tz_std_offset = 0L;
  long tz_dst_offset = 0L;
  int are_tz_rules = 0;
  struct tz_rule tz_rule_std_to_dst = {
    .type = '\0',
    .u = { .n = 0 },
    .time = 0L
  };
  struct tz_rule tz_rule_dst_to_std = {
    .type = '\0',
    .u = { .n = 0 },
    .time = 0L
  };
  parse_tzname(s, tmp_std, (char **) (&s));
  if(parse_offset(s, &tz_std_offset, (char **) (&s)) == -1) return -1;
  if(*s != '\0') {
    parse_tzname(s, tmp_dst, (char **) (&s));
    if(*s != ',') {
      if(parse_offset(s, &tz_dst_offset, (char **) (&s)) == -1) return -1;
      if(*s != ',') return -1;
    } else {
      tz_dst_offset = tz_std_offset - 60 * 60;
    }
    s++;
    if(parse_tz_rule(s, &tz_rule_std_to_dst, (char **) (&s)) == -1) return -1;
    if(*s != ',') return -1;
    s++;
    if(parse_tz_rule(s, &tz_rule_std_to_dst, (char **) (&s)) == -1) return -1;
    if(*s != '\0') return -1;
    are_tz_rules = 1;
  }
  __tz_std_offset = tz_std_offset;
  strcpy(std, tmp_std);
  __tz_std_zone = std;
  __tz_dst_offset = tz_dst_offset;
  strcpy(dst, tmp_dst);
  __tz_dst_zone = dst;
  __are_tz_rules = are_tz_rules;
  __tz_rule_std_to_dst = tz_rule_std_to_dst;
  __tz_rule_dst_to_std = tz_rule_dst_to_std;
  return 0;
}

void __tzset_unlocked(void)
{
  char *tz = getenv(TZ);
  if(tz != NULL) {
    parse_tz(tz);
  } else {
    __tz_std_offset = 0L;
    __tz_std_zone = "GMT";
    __tz_dst_offset = 0L;
    __tz_dst_zone = NULL;
    __are_tz_rules = 0;
  }
  daylight = __are_tz_rules;
  timezone = __tz_std_offset;
  tzname[0] = (char *) __tz_std_zone;
  tzname[1] = (char *) __tz_dst_zone;
}

void tzset(void)
{
  pthread_mutex_lock(&__tz_mutex);
  __tzset_unlocked();
  pthread_mutex_unlock(&__tz_mutex);
}

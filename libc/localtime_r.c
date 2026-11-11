/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#ifndef TEST
#include <time.h>
#include "priv_tzset.h"
#else
#define RETROLIBC_LOCALTIME_R
#include "retrolibc.h"
#endif

#ifndef TZNAME_MAX
#define TZNAME_MAX      _POSIX_TZNAME_MAX
#endif

struct tzname_elem
{
  struct tzname_elem *next;
  char tzname[TZNAME_MAX + 1];
};

static struct tzname_elem *first_tzname_elem = NULL;

static char *find_or_dup_tzname(const char *tzname)
{
  struct tzname_elem *elem;
  for(elem = first_tzname_elem; elem != NULL; elem = elem->next) {
    if(strcmp(tzname, elem->tzname) == 0) return elem->tzname;
  }
  elem = malloc(sizeof(struct tzname_elem));
  if(elem == NULL) return "???";
  memset(elem->tzname, '\0', TZNAME_MAX + 1);
  strncpy(elem->tzname, tzname, TZNAME_MAX);
  elem->next = first_tzname_elem;
  first_tzname_elem = elem;
  return elem->tzname;
}

#ifdef TEST
void free_retrolibc_tzname_list(void)
{
  struct tzname_elem *elem = first_tzname_elem;
  while(elem != NULL) {
    struct tzname_elem *next_elem = elem->next;
    free(elem);
    elem = next_elem;
  }
  first_tzname_elem = NULL;
}
#endif

static struct tm *localtime_r_unlocked(const time_t *time, struct tm *tm)
{
  time_t t = *time;
  time_t t2;
  if(__tz_std_offset <= 0 ? t > TIME_T_MAX + __tz_std_offset : t < TIME_T_MIN + __tz_std_offset) return NULL;
  t2 = t - __tz_std_offset;
  if(gmtime_r(&t2, tm) == NULL) return NULL;
  if(__are_tz_rules) {
    if(__cmp_tm_with_tz_rule(tm, &__tz_rule_std_to_dst) < 0) {
      tm->tm_isdst = 0;
      tm->tm_gmtoff = -__tz_std_offset;
      tm->tm_zone = find_or_dup_tzname(__tz_std_zone);
      return tm;
    }
    if(__tz_dst_offset <= 0 ? t > TIME_T_MAX + __tz_dst_offset : t < TIME_T_MIN + __tz_dst_offset) return NULL;
    t2 = t - __tz_dst_offset;
    if(gmtime_r(&t2, tm) == NULL) return NULL;
    if(__cmp_tm_with_tz_rule(tm, &__tz_rule_dst_to_std) < 0) {
      tm->tm_isdst = 1;
      tm->tm_gmtoff = -__tz_dst_offset;
      tm->tm_zone = find_or_dup_tzname(__tz_dst_zone);
      return tm;
    }
    t2 = t - __tz_std_offset;
    if(gmtime_r(&t2, tm) == NULL) return NULL;
  }
  tm->tm_isdst = 0;
  tm->tm_gmtoff = -__tz_std_offset;
  tm->tm_zone = find_or_dup_tzname(__tz_std_zone);
  return tm;
}

struct tm *localtime_r(const time_t *time, struct tm *tm)
{
  struct tm *r;
  pthread_mutex_lock(&__tz_mutex);
  __tzset_unlocked();
  r = localtime_r_unlocked(time, tm);
  pthread_mutex_unlock(&__tz_mutex);
  return r;
}

static time_t mktime_unlocked(struct tm *tm)
{
  time_t t, t2;
  long saved_gmtoff = tm->tm_gmtoff;
  const char *saved_zone = tm->tm_zone;
  if(tm->tm_zone == NULL) {
    tm->tm_gmtoff = -__tz_std_offset;
    tm->tm_zone = __tz_std_zone;
  }
  t = __mktime_no_tzset(tm);
  if(t == (time_t) -1) {
    tm->tm_gmtoff = saved_gmtoff;
    tm->tm_zone = saved_zone;
    return -1;
  }
  if((strcmp(tm->tm_zone, __tz_std_zone) == 0 && tm->tm_gmtoff == -__tz_std_offset) || (__are_tz_rules && strcmp(tm->tm_zone, __tz_dst_zone) == 0 && tm->tm_gmtoff == -__tz_dst_offset)) {
    if(localtime_r_unlocked(&t, tm) == NULL) return -1;
  } else {
    t2 = t + tm->tm_gmtoff;
    if(gmtime_r(&t2, tm) == NULL) return -1;
    tm->tm_isdst = (strcmp(saved_zone, "GMT") == 0 ? 0 : -1);
    tm->tm_gmtoff = saved_gmtoff;
    tm->tm_zone = saved_zone;
  }
  return t;
}

time_t mktime(struct tm *tm)
{
  time_t r;
  pthread_mutex_lock(&__tz_mutex);
  __tzset_unlocked();
  r = mktime_unlocked(tm);
  pthread_mutex_unlock(&__tz_mutex);
  return r;
}

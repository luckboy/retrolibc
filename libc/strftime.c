/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <string.h>
#include <time.h>
#include "priv_stdlib.h"

extern const char *__month_names[12];
extern const char *__abbreviated_month_names[12];

extern const char *__week_day_names[7];
extern const char *__abbreviated_week_day_names[7];

static int is_leap_year(int year)
{ return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0; }

/* Iso_week_days function and week_num function use algorithms from strftime_l.c file of GNU C Library */
static int  iso_week_days(int yday, int wday)
{
  int tmp = (366 / 7 + 2) * 7;
  return yday - (yday - wday + 4 + tmp) % 7 + 4 - 1;
}

static int week_num(const struct tm *tm)
{
    int year = tm->tm_year + 1900;
    int days = iso_week_days(tm->tm_yday, tm->tm_wday);
    if(days >= 0) {
      int tmp_days = iso_week_days(tm->tm_yday - ((365 + is_leap_year(year) ? 1 : 0)), tm->tm_wday);
      if(tmp_days >= 0) {
        days = tmp_days;
      }
    } else {
      year -= 1;
      days = iso_week_days(tm->tm_yday + (365 + (is_leap_year(year) ? 1 : 0)), tm->tm_wday);
    }
    return days / 7 + 1;
}

static char *copy_str(char *s, size_t *maxsize, const char *t)
{
  for(; *t != '\0'; s++, t++, (*maxsize)--) {
    if(*maxsize <= 0) return NULL;
    *s = *t;
  }
  return s;
}

static char *format_llong(char *s, size_t *maxsize, int width, long long n, int is_zero)
{
  char buf[21];
  int m, len;
  __ulltostr(buf, (unsigned long long) n, 10, 0);
  len = (int) strlen(buf);
  for(m = width - len; m > 0; s++, maxsize--, m--) {
    if(*maxsize <= 0) return NULL;
    *s = (is_zero ? '0' : ' ');
  }
  return copy_str(s, maxsize, buf);
}

static char *format_tm(char *s, size_t *maxsize, const char *format, const struct tm *tm, long long t)
{
  for(; *format != '\0'; format++) {
    if(*format == '%') {
      const char *saved_format = format;
      int am_pm_hour;
      format++;
      switch(*format) {
      case 'a':
        if((s = copy_str(s, maxsize, __abbreviated_week_day_names[tm->tm_wday])) == NULL) return NULL;
        break;
      case 'A':
        if((s = copy_str(s, maxsize, __week_day_names[tm->tm_wday])) == NULL) return NULL;
        break;
      case 'b':
      case 'h':
        if((s = copy_str(s, maxsize, __abbreviated_month_names[tm->tm_mon])) == NULL) return NULL;
        break;
      case 'B':
        if((s = copy_str(s, maxsize, __month_names[tm->tm_mon])) == NULL) return NULL;
        break;
      case 'c':
        if((s = format_tm(s, maxsize, "%a %b %e %H:%M:%S %Y", tm, t)) == NULL) return NULL; 
        break;
      case 'C':
        if((s = format_llong(s, maxsize, 2, (tm->tm_year + 1900) / 100, 1)) == NULL) return NULL;
        break;
      case 'd':
        if((s = format_llong(s, maxsize, 2, tm->tm_mday, 1)) == NULL) return NULL;
        break;
      case 'D':
        if((s = format_tm(s, maxsize, "%m/%d/%y", tm, t)) == NULL) return NULL; 
        break;
      case 'e':
        if((s = format_llong(s, maxsize, 2, tm->tm_mday, 0)) == NULL) return NULL;
        break;
      case 'H':
        if((s = format_llong(s, maxsize, 2, tm->tm_hour, 1)) == NULL) return NULL;
        break;
      case 'I':
        am_pm_hour = tm->tm_hour;
        am_pm_hour %= 12;
        if(am_pm_hour == 0) am_pm_hour = 12;
        if((s = format_llong(s, maxsize, 2, am_pm_hour, 1)) == NULL) return NULL;
        break;
      case 'j':
        if((s = format_llong(s, maxsize, 3, tm->tm_yday + 1, 1)) == NULL) return NULL;
        break;
      case 'm':
        if((s = format_llong(s, maxsize, 2, tm->tm_mday, 1)) == NULL) return NULL;
        break;
      case 'M':
        if((s = format_llong(s, maxsize, 2, tm->tm_min, 1)) == NULL) return NULL;
      case 'n':
        if(*maxsize <= 0) return NULL;
        *s = '\n';
        s++;
        (*maxsize)--;
        break;
      case 'p':
        if((s = copy_str(s, maxsize, tm->tm_hour >= 12 ? "PM" : "AM")) == NULL) return NULL;
        break;
      case 'r':
        if((s = format_tm(s, maxsize, "%I:%M:%S %p", tm, t)) == NULL) return NULL;
        break;
      case 'R':
        if((s = format_tm(s, maxsize, "%H:%M", tm, t)) == NULL) return NULL;
        break;
      case 's':
        if((s = format_llong(s, maxsize, 1, t, 0)) == NULL) return NULL;
        break;
      case 'S':
        if((s = format_llong(s, maxsize, 2, tm->tm_sec, 1)) == NULL) return NULL;
        break;
      case 't':
        if(*maxsize <= 0) return NULL;
        *s = '\t';
        s++;
        (*maxsize)--;
        break;
      case 'T':
      case 'X':
        if((s = format_tm(s, maxsize, "%H:%M:%S", tm, t)) == NULL) return NULL;
        break;
      case 'u':
        if((s = format_llong(s, maxsize, 1, tm->tm_wday != 0 ? tm->tm_wday : 7, 1)) == NULL) return NULL;
        break;
      case 'U':
        if((s = format_llong(s, maxsize, 2, (tm->tm_yday + 7 - tm->tm_wday) / 7, 1)) == NULL) return NULL;
        break;
      case 'V':
        if((s = format_llong(s, maxsize, 2, week_num(tm), 1)) == NULL) return NULL;
        break;
      case 'w':
        if((s = format_llong(s, maxsize, 1, tm->tm_wday, 1)) == NULL) return NULL;
        break;
      case 'W':
        if((s = format_llong(s, maxsize, 2, (tm->tm_yday + 7 - (tm->tm_wday + 6) % 7) / 7, 1)) == NULL) return NULL;
        break;
      case 'x':
        if((s = format_tm(s, maxsize, "%m/%d/%y", tm, t)) == NULL) return NULL;
        break;
      case 'y':
        if((s = format_llong(s, maxsize, 1, tm->tm_year % 100, 1)) == NULL) return NULL;
        break;
      case 'Y':
        if((s = format_llong(s, maxsize, 1, tm->tm_year + 1900, 1)) == NULL) return NULL;
        break;
      case 'Z':
        if((s = copy_str(s, maxsize, tm->tm_zone)) == NULL) return NULL;
        break;
      case '%':  
        if(*maxsize <= 0) return NULL;
        *s = '%';
        s++;
        (*maxsize)--;
        break;
      default:
        if(*maxsize <= 0) return NULL;
        *s = *format;
        s++;
        (*maxsize)--;
        format = saved_format;
        break;
      }
    } else {
      if(*maxsize <= 0) return NULL;
      *s = *format;
      s++;
      (*maxsize)--;
    }
  }
  return s;
}

size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *tm)
{
  struct tm tmp_tm = *tm;
  size_t tmp_maxsize = maxsize;
  long long t;
  if((t = mktime(&tmp_tm)) == -1) return 0;
  if((s = format_tm(s, &tmp_maxsize, format, &tmp_tm, t)) == NULL) return 0;
  if(tmp_maxsize <= 0) return 0;
  *s = '\0';
  return maxsize - tmp_maxsize;
}

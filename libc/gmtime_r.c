/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <limits.h>
#ifndef TEST
#include <time.h>
#include "priv_gmtime_r.h"
#else
#define RETROLIBC_GMTIME_R
#include "retrolibc.h"
#endif

#define DAYS_PER_400_YEARS              (303 * 365 + 97 * 366)
#define DAYS_PER_100_YEARS              (76 * 365 + 24 * 366)
#define DAYS_PER_4_YEARS                (3 * 365 + 366)
#define DAYS_SINCE_1600_TO_EPOCH        (280 * 365 + 90 * 366)

#define IS_OVERFLOW1(x, y)              ((x) > TIME_T_MAX / (y))
#define IS_OVERFLOW2(x, y)              ((x) < TIME_T_MIN / (y))
#define IS_OVERFLOW(x, y)               (IS_OVERFLOW1((x), (y)) || IS_OVERFLOW2((x), (y)))

static const signed char tab_month_days[12] = {
  31, /* January */
  28, /* February */
  31, /* March */
  30, /* April */
  31, /* May */
  30, /* June */
  31, /* July */
  31, /* August */
  30, /* September */
  31, /* October */
  30, /* Novamber */
  31  /* Decembder */
};

struct tm *gmtime_r(const time_t *time, struct tm *tm)
{
  time_t t = *time;
  time_t days = t / 60 / 60 / 24;
  time_t days_since_1600 = days + DAYS_SINCE_1600_TO_EPOCH;
  time_t days_div_400y;
  int days_div_100y;
  int days_div_4y;
  int days_div_y;
  int mday, yday;
  int mon;
  time_t year;
  int is_leap;
  if(days_since_1600 < 0) return NULL;
  days_div_400y = days_since_1600 / DAYS_PER_400_YEARS;
  yday = days_since_1600 % DAYS_PER_400_YEARS;
  if(yday < 0) yday += DAYS_PER_400_YEARS;
  if(yday < 366) {
    days_div_100y = days_div_4y = days_div_y = 0;
    is_leap = 1;
  } else {
    days_div_100y = (yday - 1) / DAYS_PER_100_YEARS;
    yday = (yday - 1) % DAYS_PER_100_YEARS;
    if(yday < 365) {
      days_div_4y = days_div_y = 0;
      is_leap = 0;
    } else {
      days_div_4y = (yday + 1) / DAYS_PER_4_YEARS;
      yday = (yday + 1) % DAYS_PER_4_YEARS;
      if(yday < 366) {
        days_div_y = 0;
        is_leap = 1;
      } else {
        days_div_y = (yday - 1) / 365;
        yday = (yday - 1) % 365;
        is_leap = 0;
      }
    }
  }
  year = days_div_400y * 400 + days_div_100y * 100 + days_div_4y * 4 + days_div_y + 1600 - 1900;
  if(year < INT_MIN || year > INT_MAX) return NULL;
  for(mon = 0, mday = yday; mon < 12; mon++) {
    int month_days = tab_month_days[mon];
    if(mon == 1 && is_leap) month_days++;
    if(mday < month_days) break;
    mday -= month_days;
  }
  mday++;
  tm->tm_sec = t % 60;
  tm->tm_min = (t / 60) % 60;
  tm->tm_hour = (t / 60 / 60) % 24;
  tm->tm_mday = mday;
  tm->tm_mon = mon;
  tm->tm_year = year;
  tm->tm_wday = (days + 4) % 7;
  if(tm->tm_wday < 0) tm->tm_wday += 7;
  tm->tm_yday = yday;
  tm->tm_isdst = 0;
  tm->tm_gmtoff = 0;
  tm->tm_zone = "GMT";
  return tm;
}

static int is_leap_year(int year)
{ return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0; }

int __cmp_tm_with_tz_rule(const struct tm *tm, const struct tz_rule *rule)
{
  int tm_time = tm->tm_hour * 60 * 60 + tm->tm_min * 60 + tm->tm_sec;
  if(rule->type == '\0' || rule->type == 'J') {
    int rule_day = rule->u.n;
    if(rule->type == 'J' && (rule_day < 31 + 29 || !is_leap_year(tm->tm_year + 1900))) rule_day--;
    if(tm->tm_yday < rule_day) {
      return -1;
    } else if(tm->tm_yday > rule_day) {
      return 1;
    }
  } else {
    if(tm->tm_mon < rule->u.m.m - 1) {
      return -1;
    } else if(tm->tm_mon > rule->u.m.m - 1) {
      return 1;
    } else {
      int rule_day = tm->tm_mday - tm->tm_wday + rule->u.m.d - 1;
      int month_days = tab_month_days[tm->tm_mon];
      if(is_leap_year(tm->tm_year + 1900)) month_days++;
      if(rule_day < 0) rule_day += 7;
      rule_day %= 7;
      rule_day += (rule->u.m.w - 1) * 7;
      if(rule_day >= month_days) rule_day -= 7; /* last week is 4th week */
      rule_day++;
      if(tm->tm_mday < rule_day) {
        return -1;
      } else if(tm->tm_mday > rule_day) {
        return 1;
      }
    }
  }
  if(tm_time < rule->time) {
    return -1;
  } else if(tm_time > rule->time) {
    return 1;
  } else {
    return 0;
  }
}

time_t __mktime_no_tzset(const struct tm *tm)
{
  time_t t;
  time_t years_since_1600;
  time_t days_since_1600;
  time_t days;
  time_t tmp;
  int i, mon;
  if(tm->tm_year < -(1900 - 1600) || tm->tm_year > TIME_T_MAX - (1900 - 1600)) return -1;
  /* days */
  /* year */
  years_since_1600 = ((time_t) tm->tm_year) + 1900 - 1600;
  tmp = years_since_1600 / 400;
  if(IS_OVERFLOW1(tmp, DAYS_PER_400_YEARS)) return -1;
  tmp *= DAYS_PER_400_YEARS;
  days_since_1600 = tmp;
  if(years_since_1600 % 400 != 0) {
    tmp = (years_since_1600 % 400) / 100;
    if(IS_OVERFLOW1(tmp, DAYS_PER_100_YEARS)) return -1;
    tmp *= DAYS_PER_100_YEARS;
    if(days_since_1600 > TIME_T_MAX - tmp - 1) return -1;
    days_since_1600 += tmp + 1;
    if(years_since_1600 % 400 % 100 != 0) {
      tmp = (years_since_1600 % 400 % 100) / 4;
      if(IS_OVERFLOW1(tmp, DAYS_PER_4_YEARS)) return -1;
      tmp *= DAYS_PER_4_YEARS;
      if(days_since_1600 - 1 > TIME_T_MAX - tmp) return - 1;
      days_since_1600 += tmp - 1;
      if(years_since_1600 % 400 % 100 % 4 != 0) {
        tmp = years_since_1600 % 400 % 100 % 4;
        if(IS_OVERFLOW1(tmp, 365)) return -1;
        tmp *= 365;
        if(days_since_1600 > TIME_T_MAX - tmp - 1) return -1;
        days_since_1600 += tmp + 1;
      }
    }
  }
  days = days_since_1600 - DAYS_SINCE_1600_TO_EPOCH;
  if(days < 0) return -1;
  /* month */
  if(tm->tm_mon >= 0) {
    time_t y;
    if(tm->tm_year > TIME_T_MAX - 1900) return -1;
    for(i = 0, y = ((time_t) tm->tm_year) + 1900, mon = tm->tm_mon; mon > 0; mon--) {
      int month_days = tab_month_days[i];
      if(i == 1 && is_leap_year(y)) month_days++;
      if(days > TIME_T_MAX - month_days) return -1;
      days += month_days;
      if(i + 1 < 12) {
        i++;
      } else {
        i = 0;
        if(y > TIME_T_MAX - 1) return -1;
        y++;
      }
    }
  } else {
    time_t y;
    if(tm->tm_year > TIME_T_MAX - 1900) return -1;
    for(i = 11, y = ((time_t) tm->tm_year) + 1900, mon = tm->tm_mon; mon < 0; mon++) {
      int month_days = tab_month_days[i];
      if(i == 1 && is_leap_year(y)) month_days++;
      if(days - month_days < 0) return -1;
      days -= month_days;
      if(i - 1 >= 0) {
        i--;
      } else {
        i = 11;
        if(y < TIME_T_MIN + 1) return -1;
        y--;
      }
    }
  }
  /* month day */
  tmp = tm->tm_mday - 1;
  if(tmp >= 0 ? days > TIME_T_MAX - tmp : days < TIME_T_MIN - tmp) return -1;
  days += tmp;
  /* t */
  if(IS_OVERFLOW(days, 24 * 60 * 60)) return -1;
  t = days * 24 * 60 * 60;
  /* hour */
  tmp = tm->tm_hour;
  if(IS_OVERFLOW(tmp, 60 * 60)) return -1;
  tmp *= 60 * 60;
  if(tmp >= 0 ? t > TIME_T_MAX - tmp : t < TIME_T_MIN - tmp) return -1;
  t += tmp;
  /* minute */
  tmp = tm->tm_min;
  if(IS_OVERFLOW(tmp, 60)) return -1;
  tmp *= 60;
  if(tmp >= 0 ? t > TIME_T_MAX - tmp : t < TIME_T_MIN - tmp) return -1;
  t += tmp;
  /* second */
  tmp = tm->tm_sec;
  if(tmp >= 0 ? t > TIME_T_MAX - tmp : t < TIME_T_MIN - tmp) return -1;
  t += tmp;
  /* gmtoff */
  tmp = tm->tm_gmtoff;
  if(tmp <= 0 ? t > TIME_T_MAX + tmp : t < TIME_T_MIN + tmp) return -1;
  t -= tmp;
  if(t < 0) return -1;
  return t;
}

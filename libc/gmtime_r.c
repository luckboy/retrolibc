/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <limits.h>
#include <time.h>

#define TIME_T_MAX                      LONG_MAX
#define TIME_T_MIN                      LONG_MIN

#define DAYS_PER_400_YEARS              (303 * 365 + 97 * 366)
#define DAYS_PER_100_YEARS              (76 * 365 + 24 * 366)
#define DAYS_PER_4_YEARS                (3 * 365 + 366)
#define DAYS_SINCE_1600_TO_EPOCH        (280 * 365 + 90 * 366)

#define IS_OVERFLOW1(x, y)              ((x) > TIME_T_MAX / (y))
#define IS_OVERFLOW2(x, y)              ((x) < TIME_T_MIN / (y))
#define IS_OVERFLOW(x, y)               (IS_OVERFLOW1((x), (y)) || IS_OVERFLOW2((x), (y)))

static const int tab_month_days[12] = {
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
  mon++;
  tm->tm_sec = t % 60;
  tm->tm_min = (t / 60) % 60;
  tm->tm_hour = (t / 60 / 60) % 24;
  tm->tm_mday = mday;
  tm->tm_mon = mon;
  tm->tm_year = year;
  tm->tm_wday = (days + 4) % 7;
  if(tm->tm_wday < 0) tm->tm_wday += 7;
  tm->tm_yday = yday;
  tm->tm_isdst = -1;
  tm->tm_gmtoff = 0;
  tm->tm_zone = NULL;
  return tm;
}

time_t mktime(const struct tm *tm)
{
  time_t t;
  time_t years_since_1600;
  time_t days_since_1600;
  time_t days;
  time_t tmp;
  int is_leap;
  int i, mon;
  if(tm->tm_year < -(1900 - 1600) || tm->tm_year > TIME_T_MAX - (1900 - 1600)) return -1;
  /* days */
  /* year */
  years_since_1600 = tm->tm_year + 1900 - 1600;
  tmp = years_since_1600 / 400;
  if(IS_OVERFLOW1(tmp, DAYS_PER_400_YEARS)) return -1;
  tmp *= DAYS_PER_400_YEARS;
  days_since_1600 = tmp;
  if(years_since_1600 % 400 == 0) {
    is_leap = 1;
  } else {
    tmp = (years_since_1600 % 400) / 100;
    if(IS_OVERFLOW1(tmp, DAYS_PER_100_YEARS)) return -1;
    tmp *= DAYS_PER_100_YEARS;
    if(days_since_1600 > TIME_T_MAX - tmp - 1) return -1;
    days_since_1600 += tmp + 1;
    if(years_since_1600 % 400 % 100 == 0) {
      is_leap = 0;
    } else {
      tmp = (years_since_1600 % 400 % 100) / 4;
      if(IS_OVERFLOW1(tmp, DAYS_PER_4_YEARS)) return -1;
      tmp *= DAYS_PER_4_YEARS;
      if(days_since_1600 - 1 > TIME_T_MAX - tmp) return - 1;
      days_since_1600 += tmp - 1;
      if(years_since_1600 % 400 % 100 % 4 == 0) {
        is_leap = 1;
      } else {
        tmp = years_since_1600 % 400 % 100 % 4;
        if(IS_OVERFLOW1(tmp, 365)) return -1;
        tmp *= 365;
        if(days_since_1600 > TIME_T_MAX - tmp - 1) return -1;
        days_since_1600 += tmp + 1;
        is_leap = 0;
      }
    }
  }
  days = days_since_1600 - DAYS_SINCE_1600_TO_EPOCH;
  if(days < 0) return -1;
  /* month */
  if(tm->tm_mon >= 0) {
    for(i = 0, mon = tm->tm_mon; mon > 0; i = (i + 1 < 12 ? i + 1 : 0), mon--) {
      int month_days = tab_month_days[i];
      if(i == 1 && is_leap) month_days++;
      if(days > TIME_T_MAX - month_days) return -1;
      days += month_days;
    }
  } else {
    for(i = 11, mon = tm->tm_mon; mon < 0; i = (i - 1 >= 0 ? i - 1 : 0), mon++) {
      int month_days = tab_month_days[i];
      if(i == 1 && is_leap) month_days++;
      if(days - month_days < 0) return -1;
      days -= month_days;
    }
  }
  /* month day */
  if(days > TIME_T_MAX - tm->tm_mday) return -1;
  days += tm->tm_mday;
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

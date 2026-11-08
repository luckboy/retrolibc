/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <CUnit/Basic.h>
#include "retrolibc.h"

#ifndef LONG_BIT
#define LONG_BIT        __WORDSIZE
#endif

static const int tab_month_days_for_no_leap[12] = {
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

static const int tab_month_days_for_leap[12] = {
  31, /* January */
  29, /* February */
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

unsigned test_seed(void);

static int initialize_gmtime_r(void)
{
  srand(test_seed());
  return 0;
}

static int cleanup_gmtime_r(void)
{ return 0; }

static int is_leap_year(int year)
{ return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0; }

static void test_gmtime_r_returns_year(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, max_y;
#if LONG_BIT == 64
  max_y = 2500;
#else
  max_y = 2038;
#endif
  for(y = 1970, t = 0; y <= max_y; y++) {
    if(retrolibc_gmtime_r(&t, &tm) != &tm) break; 
    if(tm.tm_year + 1900 != y) break;
    if(tm.tm_sec != 0) break;
    if(tm.tm_min != 0) break;
    if(tm.tm_hour != 0) break;
    if(tm.tm_mday != 1) break;
    if(tm.tm_mon != 0) break;
    if(tm.tm_isdst != 0) break;
    if(tm.tm_gmtoff != 0) break;
    if(tm.tm_zone != NULL) break;
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  CU_ASSERT_EQUAL(y, max_y + 1);
}

static void test_gmtime_r_returns_month_and_month_day_for_leap(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, mday, is_stop;
  for(y = 1970, t = 0; y < 2000; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 12; mon++) {
    for(mday = 1; mday <= tab_month_days_for_leap[mon]; mday++) {
      if(retrolibc_gmtime_r(&t, &tm) != &tm) break; 
      is_stop = 1;
      if(tm.tm_year + 1900 != y) break;
      if(tm.tm_sec != 0) break;
      if(tm.tm_min != 0) break;
      if(tm.tm_hour != 0) break;
      if(tm.tm_mday != mday) break;
      if(tm.tm_mon != mon) break;
      if(tm.tm_isdst != 0) break;
      if(tm.tm_gmtoff != 0) break;
      if(tm.tm_zone != NULL) break;
      is_stop = 0;
      t += 24 * 60 * 60;
    }
    if(is_stop) break;
  }
  CU_ASSERT_EQUAL(is_stop, 0);
}

static void test_gmtime_r_returns_month_and_month_day_for_no_leap(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, mday, is_stop;
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 12; mon++) {
    for(mday = 1; mday <= tab_month_days_for_no_leap[mon]; mday++) {
      if(retrolibc_gmtime_r(&t, &tm) != &tm) break; 
      is_stop = 1;
      if(tm.tm_year + 1900 != y) break;
      if(tm.tm_sec != 0) break;
      if(tm.tm_min != 0) break;
      if(tm.tm_hour != 0) break;
      if(tm.tm_mday != mday) break;
      if(tm.tm_mon != mon) break;
      if(tm.tm_isdst != 0) break;
      if(tm.tm_gmtoff != 0) break;
      if(tm.tm_zone != NULL) break;
      is_stop = 0;
      t += 24 * 60 * 60;
    }
    if(is_stop) break;
  }
  CU_ASSERT_EQUAL(is_stop, 0);
}

static void test_gmtime_r_returns_hour_and_minute_and_second(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, hour, min, sec;
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 4; mon++) {
    t += tab_month_days_for_no_leap[mon] * 24 * 60 * 60;
  }
  t += (5 - 1) * 24 * 60 * 60;
  hour = rand() / (RAND_MAX * 1.0) * 23.0;
  min = rand() / (RAND_MAX * 1.0) * 59.0;
  sec = rand() / (RAND_MAX * 1.0) * 59.0;
  t += hour * 60 * 60 + min * 60 + sec;
  CU_ASSERT_PTR_EQUAL(retrolibc_gmtime_r(&t, &tm), &tm);
  CU_ASSERT_EQUAL(tm.tm_sec, sec);
  CU_ASSERT_EQUAL(tm.tm_min, min);
  CU_ASSERT_EQUAL(tm.tm_hour, hour);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 4);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_isdst, 0);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_gmtime_r_returns_year_day_for_leap(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, yday;
  for(y = 1970, t = 0; y < 2000; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(yday = 0; yday < 366; yday++) {
    retrolibc_gmtime_r(&t, &tm);
    if(tm.tm_yday != yday) break;
    t += 24 * 60 * 60;
  }
  CU_ASSERT_EQUAL(yday, 366);
}

static void test_gmtime_r_returns_year_day_for_no_leap(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, yday;
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(yday = 0; yday < 365; yday++) {
    retrolibc_gmtime_r(&t, &tm);
    if(tm.tm_yday != yday) break;
    t += 24 * 60 * 60;
  }
  CU_ASSERT_EQUAL(yday, 365);
}

static void test_gmtime_r_returns_week_day_for_leap(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, yday, wday;
  for(y = 1970, t = 0; y < 2000; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(yday = 0, wday = 6; yday < 366; yday++, wday = (wday + 1) % 7) {
    retrolibc_gmtime_r(&t, &tm);
    if(tm.tm_wday != wday) break;
    t += 24 * 60 * 60;
  }
  CU_ASSERT_EQUAL(yday, 366);
}

static void test_gmtime_r_returns_week_day_for_no_leap(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, yday, wday;
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(yday = 0, wday = 3; yday < 365; yday++, wday = (wday + 1) % 7) {
    retrolibc_gmtime_r(&t, &tm);
    if(tm.tm_wday != wday) break;
    t += 24 * 60 * 60;
  }
  CU_ASSERT_EQUAL(yday, 365);
}

static void test_mktime_returns_seconds_for_year(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, max_y;
#if LONG_BIT == 64
  max_y = 2500;
#else
  max_y = 2038;
#endif
  for(y = 1970, t = 0; y <= max_y; y++) {
    tm.tm_year = y - 1900;
    tm.tm_sec = 0;
    tm.tm_min = 0;
    tm.tm_hour = 0;
    tm.tm_mday = 1;
    tm.tm_mon = 0;
    tm.tm_gmtoff = 0;
    if(retrolibc_mktime(&tm) != t) break;
    if(tm.tm_year + 1900 != y) break;
    if(tm.tm_sec != 0) break;
    if(tm.tm_min != 0) break;
    if(tm.tm_hour != 0) break;
    if(tm.tm_mday != 1) break;
    if(tm.tm_mon != 0) break;
    if(tm.tm_isdst != 0) break;
    if(tm.tm_gmtoff != 0) break;
    if(tm.tm_zone != NULL) break;
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  CU_ASSERT_EQUAL(y, max_y + 1);
}

static void test_mktime_returns_seconds_for_month_and_month_day_and_leap(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, mday, is_stop;
  for(y = 1970, t = 0; y < 2000; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 12; mon++) {
    for(mday = 1; mday <= tab_month_days_for_leap[mon]; mday++) {
      is_stop = 1;
      tm.tm_year = y - 1900;
      tm.tm_sec = 0;
      tm.tm_min = 0;
      tm.tm_hour = 0;
      tm.tm_mday = mday;
      tm.tm_mon = mon;
      tm.tm_gmtoff = 0;
      if(retrolibc_mktime(&tm) != t) break;
      if(tm.tm_year + 1900 != y) break;
      if(tm.tm_sec != 0) break;
      if(tm.tm_min != 0) break;
      if(tm.tm_hour != 0) break;
      if(tm.tm_mday != mday) break;
      if(tm.tm_mon != mon) break;
      if(tm.tm_isdst != 0) break;
      if(tm.tm_gmtoff != 0) break;
      if(tm.tm_zone != NULL) break;
      is_stop = 0;
      t += 24 * 60 * 60;
    }
    if(is_stop) break;
  }
  CU_ASSERT_EQUAL(is_stop, 0);
}

static void test_mktime_returns_seconds_for_month_and_month_day_and_no_leap(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, mday, is_stop;
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 12; mon++) {
    for(mday = 1; mday <= tab_month_days_for_no_leap[mon]; mday++) {
      is_stop = 1;
      tm.tm_year = y - 1900;
      tm.tm_sec = 0;
      tm.tm_min = 0;
      tm.tm_hour = 0;
      tm.tm_mday = mday;
      tm.tm_mon = mon;
      tm.tm_gmtoff = 0;
      if(retrolibc_mktime(&tm) != t) break;
      if(tm.tm_year + 1900 != y) break;
      if(tm.tm_sec != 0) break;
      if(tm.tm_min != 0) break;
      if(tm.tm_hour != 0) break;
      if(tm.tm_mday != mday) break;
      if(tm.tm_mon != mon) break;
      if(tm.tm_isdst != 0) break;
      if(tm.tm_gmtoff != 0) break;
      if(tm.tm_zone != NULL) break;
      is_stop = 0;
      t += 24 * 60 * 60;
    }
    if(is_stop) break;
  }
  CU_ASSERT_EQUAL(is_stop, 0);
}

static void test_mktime_returns_seconds_for_hour_and_minute_and_second(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, hour, min, sec;
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 4; mon++) {
    t += tab_month_days_for_no_leap[mon] * 24 * 60 * 60;
  }
  t += (5 - 1) * 24 * 60 * 60;
  hour = rand() / (RAND_MAX * 1.0) * 23.0;
  min = rand() / (RAND_MAX * 1.0) * 59.0;
  sec = rand() / (RAND_MAX * 1.0) * 59.0;
  t += hour * 60 * 60 + min * 60 + sec;
  tm.tm_sec = sec;
  tm.tm_min = min;
  tm.tm_hour = hour;
  tm.tm_mday = 5;
  tm.tm_mon = 4;
  tm.tm_year = 2003 - 1900;
  CU_ASSERT_EQUAL(retrolibc_mktime(&tm), t);
  CU_ASSERT_EQUAL(tm.tm_sec, sec);
  CU_ASSERT_EQUAL(tm.tm_min, min);
  CU_ASSERT_EQUAL(tm.tm_hour, hour);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 4);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_isdst, 0);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_mktime_returns_seconds_for_month_greater_than_in_year(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, mday, is_stop;
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  t += 365 * 24 * 60 * 60;
  for(mon = 12; mon < 12 + 12; mon++) {
    for(mday = 1; mday <= tab_month_days_for_leap[mon - 12]; mday++) {
      is_stop = 1;
      tm.tm_year = y - 1900;
      tm.tm_sec = 0;
      tm.tm_min = 0;
      tm.tm_hour = 0;
      tm.tm_mday = mday;
      tm.tm_mon = mon;
      tm.tm_gmtoff = 0;
      if(retrolibc_mktime(&tm) != t) break;
      if(tm.tm_year + 1900 != y + 1) break;
      if(tm.tm_sec != 0) break;
      if(tm.tm_min != 0) break;
      if(tm.tm_hour != 0) break;
      if(tm.tm_mday != mday) break;
      if(tm.tm_mon != mon - 12) break;
      if(tm.tm_isdst != 0) break;
      if(tm.tm_gmtoff != 0) break;
      if(tm.tm_zone != NULL) break;
      is_stop = 0;
      t += 24 * 60 * 60;
    }
    if(is_stop) break;
  }
  CU_ASSERT_EQUAL(is_stop, 0);
}

static void test_mktime_returns_seconds_for_negative_month(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, mday, is_stop;
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  t -= 365 * 24 * 60 * 60;
  for(mon = -12; mon < 0; mon++) {
    for(mday = 1; mday <= tab_month_days_for_no_leap[mon + 12]; mday++) {
      is_stop = 1;
      tm.tm_year = y - 1900;
      tm.tm_sec = 0;
      tm.tm_min = 0;
      tm.tm_hour = 0;
      tm.tm_mday = mday;
      tm.tm_mon = mon;
      tm.tm_gmtoff = 0;
      if(retrolibc_mktime(&tm) != t) break;
      if(tm.tm_year + 1900 != y - 1) break;
      if(tm.tm_sec != 0) break;
      if(tm.tm_min != 0) break;
      if(tm.tm_hour != 0) break;
      if(tm.tm_mday != mday) break;
      if(tm.tm_mon != mon + 12) break;
      if(tm.tm_isdst != 0) break;
      if(tm.tm_gmtoff != 0) break;
      if(tm.tm_zone != NULL) break;
      is_stop = 0;
      t += 24 * 60 * 60;
    }
    if(is_stop) break;
  }
  CU_ASSERT_EQUAL(is_stop, 0);
}

int add_gmtime_r_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("gmtime_r suite", initialize_gmtime_r, cleanup_gmtime_r)) == NULL) return -1;
  if(CU_add_test(suite, "gmtime_r returns year", test_gmtime_r_returns_year) == NULL) return -1;
  if(CU_add_test(suite, "gmtime_r returns month and month day for leap", test_gmtime_r_returns_month_and_month_day_for_leap) == NULL) return -1;
  if(CU_add_test(suite, "gmtime_r returns month and month day for no leap", test_gmtime_r_returns_month_and_month_day_for_no_leap) == NULL) return -1;
  if(CU_add_test(suite, "gmtime_r returns hour and minute and second", test_gmtime_r_returns_hour_and_minute_and_second) == NULL) return -1;
  if(CU_add_test(suite, "gmtime_r returns year day for leap", test_gmtime_r_returns_year_day_for_leap) == NULL) return -1;
  if(CU_add_test(suite, "gmtime_r returns year day for no leap", test_gmtime_r_returns_year_day_for_no_leap) == NULL) return -1;
  if(CU_add_test(suite, "gmtime_r returns week day for leap", test_gmtime_r_returns_week_day_for_leap) == NULL) return -1;
  if(CU_add_test(suite, "gmtime_r returns week day for no leap", test_gmtime_r_returns_week_day_for_no_leap) == NULL) return -1;
  if(CU_add_test(suite, "mktime returns seconds for year", test_mktime_returns_seconds_for_year) == NULL) return -1;
  if(CU_add_test(suite, "mktime returns seconds for month and month day and leap", test_mktime_returns_seconds_for_month_and_month_day_and_leap) == NULL) return -1;
  if(CU_add_test(suite, "mktime returns seconds for month and month day and no leap", test_mktime_returns_seconds_for_month_and_month_day_and_no_leap) == NULL) return -1;
  if(CU_add_test(suite, "mktime returns seconds for hour and minute and second", test_mktime_returns_seconds_for_hour_and_minute_and_second) == NULL) return -1;
  if(CU_add_test(suite, "mktime returns seconds for month greater than in year", test_mktime_returns_seconds_for_month_greater_than_in_year) == NULL) return -1;
  if(CU_add_test(suite, "mktime returns seconds for negative month", test_mktime_returns_seconds_for_negative_month) == NULL) return -1;
  return 0;
}

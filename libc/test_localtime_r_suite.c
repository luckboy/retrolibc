/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <string.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"

#ifndef LONG_BIT
#define LONG_BIT        __WORDSIZE
#endif

void free_retrolibc_tzname_list(void);

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

static void initialize_retrolibc_localtime_r()
{
  retrolibc___tz_std_offset = 0L;
  retrolibc___tz_std_zone = "???";
  retrolibc___tz_dst_offset = 0L;
  retrolibc___tz_dst_zone = "???";
  retrolibc___are_tz_rules = 0;
  retrolibc___tz_rule_std_to_dst.type = '\0';
  retrolibc___tz_rule_std_to_dst.u.n = 0;
  retrolibc___tz_rule_std_to_dst.time = 0L;
  retrolibc___tz_rule_dst_to_std.type = '\0';
  retrolibc___tz_rule_dst_to_std.u.n = 0;
  retrolibc___tz_rule_dst_to_std.time = 0L;
  retrolibc_daylight = 0;
  retrolibc_timezone = 0L;
  retrolibc_tzname[0] = "";
  retrolibc_tzname[1] = "";
}

static int initialize_localtime_r(void)
{ return 0; }

static int cleanup_localtime_r(void)
{
  free_retrolibc_tzname_list();
  return 0;
}

static int is_leap_year(int year)
{ return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0; }

static void test_localtime_r_returns_tm(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, hour, min, sec;
  initialize_retrolibc_localtime_r();
  putenv("RETROLIBC_TZ=CET-1");
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 4; mon++) {
    t += tab_month_days[mon] * 24 * 60 * 60;
  }
  t += (5 - 1) * 24 * 60 * 60;
  hour = 12;
  min = 34;
  sec = 56;
  t += hour * 60 * 60 + min * 60 + sec;
  t -= 60 * 60;
  CU_ASSERT_PTR_EQUAL(retrolibc_localtime_r(&t, &tm), &tm);
  CU_ASSERT_EQUAL(tm.tm_sec, sec);
  CU_ASSERT_EQUAL(tm.tm_min, min);
  CU_ASSERT_EQUAL(tm.tm_hour, hour);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 4);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_isdst, 0);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 60 * 60);
  CU_ASSERT_STRING_EQUAL(tm.tm_zone, "CET");
}

static void test_localtime_r_returns_tm_before_dst(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, hour, min, sec;
  initialize_retrolibc_localtime_r();
  putenv("RETROLIBC_TZ=CET-1CEST,M3.5.0,M10.5.0/3");
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 2; mon++) {
    t += tab_month_days[mon] * 24 * 60 * 60;
  }
  t += (5 - 1) * 24 * 60 * 60;
  hour = 12;
  min = 34;
  sec = 56;
  t += hour * 60 * 60 + min * 60 + sec;
  t -= 60 * 60;
  CU_ASSERT_PTR_EQUAL(retrolibc_localtime_r(&t, &tm), &tm);
  CU_ASSERT_EQUAL(tm.tm_sec, sec);
  CU_ASSERT_EQUAL(tm.tm_min, min);
  CU_ASSERT_EQUAL(tm.tm_hour, hour);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 2);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_isdst, 0);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 60 * 60);
  CU_ASSERT_STRING_EQUAL(tm.tm_zone, "CET");
}

static void test_localtime_r_returns_tm_in_dst(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, hour, min, sec;
  initialize_retrolibc_localtime_r();
  putenv("RETROLIBC_TZ=CET-1CEST,M3.5.0,M10.5.0/3");
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 4; mon++) {
    t += tab_month_days[mon] * 24 * 60 * 60;
  }
  t += (5 - 1) * 24 * 60 * 60;
  hour = 12;
  min = 34;
  sec = 56;
  t += hour * 60 * 60 + min * 60 + sec;
  t -= 2 * 60 * 60;
  CU_ASSERT_PTR_EQUAL(retrolibc_localtime_r(&t, &tm), &tm);
  CU_ASSERT_EQUAL(tm.tm_sec, sec);
  CU_ASSERT_EQUAL(tm.tm_min, min);
  CU_ASSERT_EQUAL(tm.tm_hour, hour);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 4);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_isdst, 1);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 2 * 60 * 60);
  CU_ASSERT_STRING_EQUAL(tm.tm_zone, "CEST");
}

static void test_localtime_r_returns_tm_after_dst(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, hour, min, sec;
  initialize_retrolibc_localtime_r();
  putenv("RETROLIBC_TZ=CET-1CEST,M3.5.0,M10.5.0/3");
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 9; mon++) {
    t += tab_month_days[mon] * 24 * 60 * 60;
  }
  t += (30 - 1) * 24 * 60 * 60;
  hour = 12;
  min = 34;
  sec = 56;
  t += hour * 60 * 60 + min * 60 + sec;
  t -= 60 * 60;
  CU_ASSERT_PTR_EQUAL(retrolibc_localtime_r(&t, &tm), &tm);
  CU_ASSERT_EQUAL(tm.tm_sec, sec);
  CU_ASSERT_EQUAL(tm.tm_min, min);
  CU_ASSERT_EQUAL(tm.tm_hour, hour);
  CU_ASSERT_EQUAL(tm.tm_mday, 30);
  CU_ASSERT_EQUAL(tm.tm_mon, 9);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_isdst, 0);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 60 * 60);
  CU_ASSERT_STRING_EQUAL(tm.tm_zone, "CET");
}

static void test_mktime_returns_seconds(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, hour, min, sec;
  initialize_retrolibc_localtime_r();
  putenv("RETROLIBC_TZ=CET-1");
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 4; mon++) {
    t += tab_month_days[mon] * 24 * 60 * 60;
  }
  t += (5 - 1) * 24 * 60 * 60;
  hour = 12;
  min = 34;
  sec = 56;
  t += hour * 60 * 60 + min * 60 + sec;
  t -= 60 * 60;
  tm.tm_sec = sec;
  tm.tm_min = min;
  tm.tm_hour = hour;
  tm.tm_mday = 5;
  tm.tm_mon = 4;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_mktime(&tm), t);
  CU_ASSERT_EQUAL(tm.tm_sec, sec);
  CU_ASSERT_EQUAL(tm.tm_min, min);
  CU_ASSERT_EQUAL(tm.tm_hour, hour);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 4);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_isdst, 0);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 60 * 60);
  CU_ASSERT_STRING_EQUAL(tm.tm_zone, "CET");
}

static void test_mktime_returns_seconds_and_corrects_tm(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, hour, min, sec;
  initialize_retrolibc_localtime_r();
  putenv("RETROLIBC_TZ=CET-1");
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 4; mon++) {
    t += tab_month_days[mon] * 24 * 60 * 60;
  }
  t += (5 - 1) * 24 * 60 * 60;
  hour = 12;
  min = 34;
  sec = 56;
  t += hour * 60 * 60 + min * 60 + sec;
  t -= 60 * 60;
  tm.tm_sec = sec;
  tm.tm_min = min;
  tm.tm_hour = hour;
  tm.tm_mday = 5;
  tm.tm_mon = 12 + 4;
  tm.tm_year = 2003 - 1900 - 1;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_mktime(&tm), t);
  CU_ASSERT_EQUAL(tm.tm_sec, sec);
  CU_ASSERT_EQUAL(tm.tm_min, min);
  CU_ASSERT_EQUAL(tm.tm_hour, hour);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 4);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_isdst, 0);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 60 * 60);
  CU_ASSERT_STRING_EQUAL(tm.tm_zone, "CET");
}

static void test_mktime_returns_seconds_for_gmt(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, hour, min, sec;
  initialize_retrolibc_localtime_r();
  putenv("RETROLIBC_TZ=CET-1");
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 4; mon++) {
    t += tab_month_days[mon] * 24 * 60 * 60;
  }
  t += (5 - 1) * 24 * 60 * 60;
  hour = 12;
  min = 34;
  sec = 56;
  t += hour * 60 * 60 + min * 60 + sec;
  tm.tm_sec = sec;
  tm.tm_min = min;
  tm.tm_hour = hour;
  tm.tm_mday = 5;
  tm.tm_mon = 4;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 0;
  tm.tm_zone = "GMT";
  CU_ASSERT_EQUAL(retrolibc_mktime(&tm), t);
  CU_ASSERT_EQUAL(tm.tm_sec, sec);
  CU_ASSERT_EQUAL(tm.tm_min, min);
  CU_ASSERT_EQUAL(tm.tm_hour, hour);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 4);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_isdst, 0);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0);
  CU_ASSERT_STRING_EQUAL(tm.tm_zone, "GMT");
}

static void test_mktime_returns_seconds_and_corrects_tm_for_gmt(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, hour, min, sec;
  initialize_retrolibc_localtime_r();
  putenv("RETROLIBC_TZ=CET-1");
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 4; mon++) {
    t += tab_month_days[mon] * 24 * 60 * 60;
  }
  t += (5 - 1) * 24 * 60 * 60;
  hour = 12;
  min = 34;
  sec = 56;
  t += hour * 60 * 60 + min * 60 + sec;
  tm.tm_sec = sec;
  tm.tm_min = min;
  tm.tm_hour = hour;
  tm.tm_mday = 5;
  tm.tm_mon = 12 + 4;
  tm.tm_year = 2003 - 1900 - 1;
  tm.tm_gmtoff = 0;
  tm.tm_zone = "GMT";
  CU_ASSERT_EQUAL(retrolibc_mktime(&tm), t);
  CU_ASSERT_EQUAL(tm.tm_sec, sec);
  CU_ASSERT_EQUAL(tm.tm_min, min);
  CU_ASSERT_EQUAL(tm.tm_hour, hour);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 4);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_isdst, 0);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0);
  CU_ASSERT_STRING_EQUAL(tm.tm_zone, "GMT");
}

static void test_mktime_returns_seconds_for_cst(void)
{
  static struct retrolibc_tm tm;
  time_t t;
  int y, mon, hour, min, sec;
  initialize_retrolibc_localtime_r();
  putenv("RETROLIBC_TZ=CET-1");
  for(y = 1970, t = 0; y < 2003; y++) {
    t += 365 * 24 * 60 * 60;
    if(is_leap_year(y)) t += 24 * 60 * 60;
  }
  for(mon = 0; mon < 4; mon++) {
    t += tab_month_days[mon] * 24 * 60 * 60;
  }
  t += (5 - 1) * 24 * 60 * 60;
  hour = 12;
  min = 34;
  sec = 56;
  t += hour * 60 * 60 + min * 60 + sec;
  t += 8 * 60 * 60;
  tm.tm_sec = sec;
  tm.tm_min = min;
  tm.tm_hour = hour;
  tm.tm_mday = 5;
  tm.tm_mon = 4;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = -8 * 60 * 60;
  tm.tm_zone = "CST";
  CU_ASSERT_EQUAL(retrolibc_mktime(&tm), t);
  CU_ASSERT_EQUAL(tm.tm_sec, sec);
  CU_ASSERT_EQUAL(tm.tm_min, min);
  CU_ASSERT_EQUAL(tm.tm_hour, hour);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 4);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_isdst, -1);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, -8 * 60 * 60);
  CU_ASSERT_STRING_EQUAL(tm.tm_zone, "CST");
}

int add_localtime_r_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("lcoaltime_r suite", initialize_localtime_r, cleanup_localtime_r)) == NULL) return -1;
  if(CU_add_test(suite, "localtime_r returns tm", test_localtime_r_returns_tm) == NULL) return -1;
  if(CU_add_test(suite, "localtime_r returns tm before dst", test_localtime_r_returns_tm_before_dst) == NULL) return -1;
  if(CU_add_test(suite, "localtime_r returns tm in dst", test_localtime_r_returns_tm_in_dst) == NULL) return -1;
  if(CU_add_test(suite, "localtime_r returns tm after dst", test_localtime_r_returns_tm_after_dst) == NULL) return -1;
  if(CU_add_test(suite, "mktime returns seconds", test_mktime_returns_seconds) == NULL) return -1;
  if(CU_add_test(suite, "mktime returns seconds and corrects tm", test_mktime_returns_seconds_and_corrects_tm) == NULL) return -1;
  if(CU_add_test(suite, "mktime returns seconds for GMT", test_mktime_returns_seconds_for_gmt) == NULL) return -1;
  if(CU_add_test(suite, "mktime returns seconds and corrects tm for GMT", test_mktime_returns_seconds_and_corrects_tm_for_gmt) == NULL) return -1;
  if(CU_add_test(suite, "mktime returns seconds for CST", test_mktime_returns_seconds_for_cst) == NULL) return -1;
  return 0;
}

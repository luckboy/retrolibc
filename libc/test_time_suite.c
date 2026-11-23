/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"

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

static void initialize_retrolibc_time(void)
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

static int initialize_time(void)
{ return 0; }

static int cleanup_time(void)
{
  free_retrolibc_tzname_list();
  return 0;
}

static int is_leap_year(int year)
{ return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0; }

static void test_asctime_converts_tm_to_string(void)
{
  static struct retrolibc_tm tm;
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 12;
  tm.tm_mday = 5;
  tm.tm_mon = 4;
  tm.tm_year = 2003 - 1900;
  tm.tm_wday = 1;
  CU_ASSERT_STRING_EQUAL(retrolibc_asctime(&tm), "Mon May  5 12:34:56 2003\n");
}

static void test_asctime_r_converts_tm_to_string(void)
{
  static struct retrolibc_tm tm;
  static char buf[26];
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 12;
  tm.tm_mday = 5;
  tm.tm_mon = 4;
  tm.tm_year = 2003 - 1900;
  tm.tm_wday = 1;
  CU_ASSERT_STRING_EQUAL(retrolibc_asctime_r(&tm, buf), "Mon May  5 12:34:56 2003\n");
}

static void test_ctime_converts_time_to_string(void)
{
  time_t t;
  int y, mon, hour, min, sec;
  initialize_retrolibc_time();
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
  CU_ASSERT_STRING_EQUAL(retrolibc_ctime(&t), "Mon May  5 12:34:56 2003\n");
}

static void test_ctime_r_converts_time_to_string(void)
{
  static char buf[26];
  time_t t;
  int y, mon, hour, min, sec;
  initialize_retrolibc_time();
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
  CU_ASSERT_STRING_EQUAL(retrolibc_ctime_r(&t, buf), "Mon May  5 12:34:56 2003\n");
}

static void test_difftime_returns_difference_between_2_time_values(void)
{ CU_ASSERT_EQUAL(difftime(3456, 1234), (double) (3456 - 1234)); }

static void test_gmtime_returns_tm(void)
{
  struct retrolibc_tm *tm;
  time_t t;
  int y, mon, hour, min, sec;
  initialize_retrolibc_time();
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
  tm = retrolibc_gmtime(&t);
  CU_ASSERT_EQUAL(tm->tm_sec, sec);
  CU_ASSERT_EQUAL(tm->tm_min, min);
  CU_ASSERT_EQUAL(tm->tm_hour, hour);
  CU_ASSERT_EQUAL(tm->tm_mday, 5);
  CU_ASSERT_EQUAL(tm->tm_mon, 4);
  CU_ASSERT_EQUAL(tm->tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm->tm_isdst, 0);
  CU_ASSERT_EQUAL(tm->tm_gmtoff, 0);
  CU_ASSERT_STRING_EQUAL(tm->tm_zone, "GMT");
}

static void test_localtime_returns_tm(void)
{
  struct retrolibc_tm *tm;
  time_t t;
  int y, mon, hour, min, sec;
  initialize_retrolibc_time();
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
  tm = retrolibc_localtime(&t);
  CU_ASSERT_EQUAL(tm->tm_sec, sec);
  CU_ASSERT_EQUAL(tm->tm_min, min);
  CU_ASSERT_EQUAL(tm->tm_hour, hour);
  CU_ASSERT_EQUAL(tm->tm_mday, 5);
  CU_ASSERT_EQUAL(tm->tm_mon, 4);
  CU_ASSERT_EQUAL(tm->tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm->tm_isdst, 0);
  CU_ASSERT_EQUAL(tm->tm_gmtoff, 60 * 60);
  CU_ASSERT_STRING_EQUAL(tm->tm_zone, "CET");
}

int add_time_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("time suite", initialize_time, cleanup_time)) == NULL) return -1;
  if(CU_add_test(suite, "asctime converts tm to string", test_asctime_converts_tm_to_string) == NULL) return -1;
  if(CU_add_test(suite, "asctime_r converts tm to string", test_asctime_r_converts_tm_to_string) == NULL) return -1;
  if(CU_add_test(suite, "ctime converts time to string", test_ctime_converts_time_to_string) == NULL) return -1;
  if(CU_add_test(suite, "ctime converts time_r to string", test_ctime_r_converts_time_to_string) == NULL) return -1;
  if(CU_add_test(suite, "difftime returns difference between 2 time values", test_difftime_returns_difference_between_2_time_values) == NULL) return -1;
  if(CU_add_test(suite, "gmttime returns tm", test_gmtime_returns_tm) == NULL) return -1;
  if(CU_add_test(suite, "localtime returns tm", test_localtime_returns_tm) == NULL) return -1;
  return 0;
}

/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <CUnit/Basic.h>
#include "retrolibc.h"

static int initialize_strptime(void)
{ return 0; }

static int cleanup_strptime(void)
{ return 0; }

static void test_strptime_converts_string_to_tm(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "21:34:56";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%H:%M:%S", &tm), buf + 8);
  CU_ASSERT_EQUAL(tm.tm_sec, 56);
  CU_ASSERT_EQUAL(tm.tm_min, 34);
  CU_ASSERT_EQUAL(tm.tm_hour, 21);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_converts_string_to_tm_for_a(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "Wed";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%a", &tm), buf + 3);
  CU_ASSERT_EQUAL(tm.tm_wday, 3);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "Tuesday";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%a", &tm), buf + 7);
  CU_ASSERT_EQUAL(tm.tm_wday, 2);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_a(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%a", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_a(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "Wed";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%A", &tm), buf + 3);
  CU_ASSERT_EQUAL(tm.tm_wday, 3);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "Tuesday";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%A", &tm), buf + 7);
  CU_ASSERT_EQUAL(tm.tm_wday, 2);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_a(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%A", &tm));
}

static void test_strptime_converts_string_to_tm_for_b(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "Mar";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%b", &tm), buf + 3);
  CU_ASSERT_EQUAL(tm.tm_mon, 2);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "April";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%b", &tm), buf + 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 3);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_b(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%b", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_b(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "Mar";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%B", &tm), buf + 3);
  CU_ASSERT_EQUAL(tm.tm_mon, 2);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "April";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%B", &tm), buf + 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 3);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_b(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%B", &tm));
}

static void test_strptime_converts_string_to_tm_for_h(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "Mar";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%h", &tm), buf + 3);
  CU_ASSERT_EQUAL(tm.tm_mon, 2);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "April";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%h", &tm), buf + 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 3);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_h(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%h", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_c(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "20";
  tm.tm_year = 3;
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%C", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  tm.tm_year = 83;
  buf = "19";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%C", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_year, 1983 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_c(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%C", &tm));
}

static void test_strptime_converts_string_to_tm_for_d(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "05";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%d", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "6";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%d", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_mday, 6);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "310";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%d", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_mday, 31);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_d(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%d", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("0", "%d", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("32", "%d", &tm));
}

static void test_strptime_converts_string_to_tm_for_e(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "05";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%e", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "6";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%e", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_mday, 6);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "310";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%e", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_mday, 31);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_e(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%e", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("0", "%e", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("32", "%e", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_h(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "21";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%H", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_hour, 21);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "9";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%H", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_hour, 9);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "230";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%H", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_hour, 23);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_h(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%H", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("24", "%H", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_i(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "10";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%I", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_hour, 10);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "9";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%I", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_hour, 9);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "120";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%I", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_hour, 0);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_i(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%I", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("0", "%I", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("13", "%I", &tm));
}

static void test_strptime_converts_string_to_tm_for_j(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "12";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%j", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_yday, 11);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "345";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%j", &tm), buf + 3);
  CU_ASSERT_EQUAL(tm.tm_yday, 344);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "3660";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%j", &tm), buf + 3);
  CU_ASSERT_EQUAL(tm.tm_yday, 365);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_j(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%j", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("0", "%j", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("367", "%j", &tm));
}

static void test_strptime_converts_string_to_tm_for_m(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "10";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%m", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_mon, 9);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "3";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%m", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_mon, 2);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "120";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%m", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_mon, 11);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_m(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%m", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("0", "%m", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("13", "%m", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_m(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "34";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%M", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_min, 34);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "4";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%M", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_min, 4);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "590";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%M", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_min, 59);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_m(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%M", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("60", "%M", &tm));
}

static void test_strptime_converts_string_to_tm_for_n(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = " \t ";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%n", &tm), buf + 3);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = " \n\t xxx";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%n", &tm), buf + 4);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_converts_string_to_tm_for_t(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = " \t ";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%t", &tm), buf + 3);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = " \n\t xxx";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%t", &tm), buf + 4);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_converts_string_to_tm_for_p(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  tm.tm_hour = 9;
  buf = "AM";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%p", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_hour, 9);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  tm.tm_hour = 10;
  buf = "PM";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%p", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_hour, 22);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  tm.tm_hour = 21;
  buf = "am";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%p", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_hour, 9);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  tm.tm_hour = 22;
  buf = "pm";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%p", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_hour, 22);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_p(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%p", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_s(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "56";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%S", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_sec, 56);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "7";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%S", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_sec, 7);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "610";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%S", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_sec, 59);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_s(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%S", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("62", "%S", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_u(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "09";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%U", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "8";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%U", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "530";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%U", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_u(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%U", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("54", "%U", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_w(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "09";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%W", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "8";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%W", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "530";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%W", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_w(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%W", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("54", "%W", &tm));
}

static void test_strptime_converts_string_to_tm_for_w(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "3";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%w", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_wday, 3);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "2";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%w", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_wday, 2);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "6";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%w", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_wday, 6);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_w(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%w", &tm));
  CU_ASSERT_PTR_NULL(retrolibc_strptime("7", "%w", &tm));
}

static void test_strptime_converts_string_to_tm_for_y(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "83";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%y", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_year, 1983 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "3";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%y", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "69";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%y", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_year, 1969 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "68";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%y", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_year, 2068 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "990";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%y", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_year, 1999 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_y(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%y", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_y(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "2003";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%Y", &tm), buf + 4);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "1983";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%Y", &tm), buf + 4);
  CU_ASSERT_EQUAL(tm.tm_year, 1983 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = "99990";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%Y", &tm), buf + 4);
  CU_ASSERT_EQUAL(tm.tm_year, 9999 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_y(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%y", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_c_and_y(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "20 69";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%C %y", &tm), buf + 5);
  CU_ASSERT_EQUAL(tm.tm_year, 2069 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_converts_string_to_tm_for_percent(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "%";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%%", &tm), buf + 1);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_percent(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%%", &tm));
}

static void test_strptime_converts_string_to_tm_for_c(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "Wed Mar  5 21:34:56 2003";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%c", &tm), buf + 24);
  CU_ASSERT_EQUAL(tm.tm_sec, 56);
  CU_ASSERT_EQUAL(tm.tm_min, 34);
  CU_ASSERT_EQUAL(tm.tm_hour, 21);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 2);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_wday, 3);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_c(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%c", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_d(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "02/05/03";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%D", &tm), buf + 8);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 1);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_d(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%D", &tm));
}

static void test_strptime_converts_string_to_tm_for_r(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "09:34:56 PM";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%r", &tm), buf + 11);
  CU_ASSERT_EQUAL(tm.tm_sec, 56);
  CU_ASSERT_EQUAL(tm.tm_min, 34);
  CU_ASSERT_EQUAL(tm.tm_hour, 21);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_r(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%r", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_r(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "21:34";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%R", &tm), buf + 5);
  CU_ASSERT_EQUAL(tm.tm_min, 34);
  CU_ASSERT_EQUAL(tm.tm_hour, 21);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_r(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%R", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_t(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "21:34:56";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%T", &tm), buf + 8);
  CU_ASSERT_EQUAL(tm.tm_sec, 56);
  CU_ASSERT_EQUAL(tm.tm_min, 34);
  CU_ASSERT_EQUAL(tm.tm_hour, 21);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_t(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%T", &tm));
}

static void test_strptime_converts_string_to_tm_for_x(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "02/05/03";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%x", &tm), buf + 8);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 1);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_x(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%x", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_x(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "21:34:56";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%X", &tm), buf + 8);
  CU_ASSERT_EQUAL(tm.tm_sec, 56);
  CU_ASSERT_EQUAL(tm.tm_min, 34);
  CU_ASSERT_EQUAL(tm.tm_hour, 21);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_x(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%X", &tm));
}

static void test_strptime_converts_string_to_tm_for_uppercase_d_and_uppercase_t(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "02/05/03 21:34:56";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%D %T", &tm), buf + 17);
  CU_ASSERT_EQUAL(tm.tm_sec, 56);
  CU_ASSERT_EQUAL(tm.tm_min, 34);
  CU_ASSERT_EQUAL(tm.tm_hour, 21);
  CU_ASSERT_EQUAL(tm.tm_mday, 5);
  CU_ASSERT_EQUAL(tm.tm_mon, 1);
  CU_ASSERT_EQUAL(tm.tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_uppercase_d_and_uppercase_t(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%D %T", &tm));
}

static void test_strptime_converts_string_to_tm_for_space(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = " \t ";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, " ", &tm), buf + 3);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
  buf = " \n\t xxx";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "\t", &tm), buf + 4);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_converts_string_to_tm_for_invalid_conversion_specificator(void)
{
  static struct retrolibc_tm tm;
  char *buf;
  buf = "%q";
  CU_ASSERT_PTR_EQUAL(retrolibc_strptime(buf, "%q", &tm), buf + 2);
  CU_ASSERT_EQUAL(tm.tm_gmtoff, 0L);
  CU_ASSERT_PTR_NULL(tm.tm_zone);
}

static void test_strptime_returns_null_for_invalid_conversion_specificator(void)
{
  static struct retrolibc_tm tm;
  CU_ASSERT_PTR_NULL(retrolibc_strptime("XXX", "%q", &tm));
}

int add_strptime_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("strptime suite", initialize_strptime, cleanup_strptime)) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm", test_strptime_converts_string_to_tm) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for a", test_strptime_converts_string_to_tm_for_a) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for a", test_strptime_returns_null_for_a) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for A", test_strptime_converts_string_to_tm_for_uppercase_a) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for A", test_strptime_returns_null_for_uppercase_a) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for b", test_strptime_converts_string_to_tm_for_b) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for b", test_strptime_returns_null_for_b) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for B", test_strptime_converts_string_to_tm_for_uppercase_b) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for B", test_strptime_returns_null_for_uppercase_b) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for h", test_strptime_converts_string_to_tm_for_h) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for h", test_strptime_returns_null_for_h) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for C", test_strptime_converts_string_to_tm_for_uppercase_c) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for C", test_strptime_returns_null_for_uppercase_c) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for d", test_strptime_converts_string_to_tm_for_d) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for d", test_strptime_returns_null_for_d) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for e", test_strptime_converts_string_to_tm_for_e) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for e", test_strptime_returns_null_for_e) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for H", test_strptime_converts_string_to_tm_for_uppercase_h) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for H", test_strptime_returns_null_for_uppercase_h) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for I", test_strptime_converts_string_to_tm_for_uppercase_i) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for I", test_strptime_returns_null_for_uppercase_i) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for j", test_strptime_converts_string_to_tm_for_j) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for j", test_strptime_returns_null_for_j) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for m", test_strptime_converts_string_to_tm_for_m) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for m", test_strptime_returns_null_for_m) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for M", test_strptime_converts_string_to_tm_for_uppercase_m) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for M", test_strptime_returns_null_for_uppercase_m) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for n", test_strptime_converts_string_to_tm_for_n) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for t", test_strptime_converts_string_to_tm_for_t) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for p", test_strptime_converts_string_to_tm_for_p) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for p", test_strptime_returns_null_for_p) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for S", test_strptime_converts_string_to_tm_for_uppercase_s) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for S", test_strptime_returns_null_for_uppercase_s) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for U", test_strptime_converts_string_to_tm_for_uppercase_u) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for U", test_strptime_returns_null_for_uppercase_u) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for W", test_strptime_converts_string_to_tm_for_uppercase_w) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for W", test_strptime_returns_null_for_uppercase_w) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for w", test_strptime_converts_string_to_tm_for_w) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for w", test_strptime_returns_null_for_w) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for y", test_strptime_converts_string_to_tm_for_y) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for y", test_strptime_returns_null_for_y) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for Y", test_strptime_converts_string_to_tm_for_uppercase_y) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for Y", test_strptime_returns_null_for_uppercase_y) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for C and y", test_strptime_converts_string_to_tm_for_uppercase_c_and_y) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for %", test_strptime_converts_string_to_tm_for_percent) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for %", test_strptime_returns_null_for_percent) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for c", test_strptime_converts_string_to_tm_for_c) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for c", test_strptime_returns_null_for_c) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for D", test_strptime_converts_string_to_tm_for_uppercase_d) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for D", test_strptime_returns_null_for_uppercase_d) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for r", test_strptime_converts_string_to_tm_for_r) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for r", test_strptime_returns_null_for_r) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for R", test_strptime_converts_string_to_tm_for_uppercase_r) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for R", test_strptime_returns_null_for_uppercase_r) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for T", test_strptime_converts_string_to_tm_for_uppercase_t) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for T", test_strptime_returns_null_for_uppercase_t) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for x", test_strptime_converts_string_to_tm_for_x) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for x", test_strptime_returns_null_for_x) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for X", test_strptime_converts_string_to_tm_for_uppercase_x) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for X", test_strptime_returns_null_for_uppercase_x) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for D and T", test_strptime_converts_string_to_tm_for_uppercase_d_and_uppercase_t) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for D and T", test_strptime_returns_null_for_uppercase_d_and_uppercase_t) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for space", test_strptime_converts_string_to_tm_for_space) == NULL) return -1;
  if(CU_add_test(suite, "strptime converts string to tm for invalid conversion specificator", test_strptime_converts_string_to_tm_for_invalid_conversion_specificator) == NULL) return -1;
  if(CU_add_test(suite, "strptime returns null for invalid conversion specificator", test_strptime_returns_null_for_invalid_conversion_specificator) == NULL) return -1;
  return 0;
}

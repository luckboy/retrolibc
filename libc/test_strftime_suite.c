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

void free_retrolibc_tzname_list(void);

static void initialize_retrolibc_strftime()
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

static int initialize_strftime(void)
{ return 0; }

static int cleanup_strftime(void)
{
  free_retrolibc_tzname_list();
  return 0;
}

static void test_strftime_converts_tm_to_string(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%H:%M:%S", &tm), 8);
  CU_ASSERT_STRING_EQUAL(buf, "21:34:56");
}

static void test_strftime_converts_tm_to_string_for_a(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%a", &tm), 3);
  CU_ASSERT_STRING_EQUAL(buf, "Wed");
}

static void test_strftime_converts_tm_to_string_for_uppercase_a(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%A", &tm), 9);
  CU_ASSERT_STRING_EQUAL(buf, "Wednesday");
}

static void test_strftime_converts_tm_to_string_for_b(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%b", &tm), 3);
  CU_ASSERT_STRING_EQUAL(buf, "Mar");
}

static void test_strftime_converts_tm_to_string_for_h(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%h", &tm), 3);
  CU_ASSERT_STRING_EQUAL(buf, "Mar");
}

static void test_strftime_converts_tm_to_string_for_uppercase_b(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%B", &tm), 5);
  CU_ASSERT_STRING_EQUAL(buf, "March");
}

static void test_strftime_converts_tm_to_string_for_uppercase_c(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%C", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "20");
}

static void test_strftime_converts_tm_to_string_for_d(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%d", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "05");
}

static void test_strftime_converts_tm_to_string_for_e(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%e", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, " 5");
}

static void test_strftime_converts_tm_to_string_for_uppercase_h(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%H", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "21");
}

static void test_strftime_converts_tm_to_string_for_uppercase_i(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%I", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "09");
}

static void test_strftime_converts_tm_to_string_for_uppercase_i_and_0(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 0;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%I", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "12");
}

static void test_strftime_converts_tm_to_string_for_uppercase_i_and_12(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 12;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%I", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "12");
}

static void test_strftime_converts_tm_to_string_for_j(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%j", &tm), 3);
  CU_ASSERT_STRING_EQUAL(buf, "064");
}

static void test_strftime_converts_tm_to_string_for_m(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 1;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%m", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "02");
}

static void test_strftime_converts_tm_to_string_for_uppercase_m(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%M", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "34");
}

static void test_strftime_converts_tm_to_string_for_n(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%n", &tm), 1);
  CU_ASSERT_STRING_EQUAL(buf, "\n");
}

static void test_strftime_converts_tm_to_string_for_p(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%p", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "PM");
}

static void test_strftime_converts_tm_to_string_for_p_and_0(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 0;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%p", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "AM");
}

static void test_strftime_converts_tm_to_string_for_p_and_12(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 12;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%p", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "PM");
}

static void test_strftime_converts_tm_to_string_for_s(void)
{
  static char buf[100];
  static char buf2[100];
  static struct retrolibc_tm tm;
  static struct retrolibc_tm tm2;
  time_t t;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  tm2 = tm;
  t = retrolibc_mktime(&tm2);
  sprintf(buf2, "%lld", (long long) t);
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%s", &tm), strlen(buf2));
  CU_ASSERT_STRING_EQUAL(buf, buf2);
}

static void test_strftime_converts_tm_to_string_for_uppercase_s(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%S", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "56");
}

static void test_strftime_converts_tm_to_string_for_t(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%t", &tm), 1);
  CU_ASSERT_STRING_EQUAL(buf, "\t");
}

static void test_strftime_converts_tm_to_string_for_u(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%u", &tm), 1);
  CU_ASSERT_STRING_EQUAL(buf, "3");
}

static void test_strftime_converts_tm_to_string_for_u_and_2(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 2;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%u", &tm), 1);
  CU_ASSERT_STRING_EQUAL(buf, "7");
}

static void test_strftime_converts_tm_to_string_for_uppercase_u(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%U", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "09");
}

static void test_strftime_converts_tm_to_string_for_uppercase_v(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%V", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "10");
}

static void test_strftime_converts_tm_to_string_for_w(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%w", &tm), 1);
  CU_ASSERT_STRING_EQUAL(buf, "3");
}

static void test_strftime_converts_tm_to_string_for_w_and_2(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 2;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%w", &tm), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_strftime_converts_tm_to_string_for_uppercase_w(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%W", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "09");
}

static void test_strftime_converts_tm_to_string_for_y(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%y", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "03");
}

static void test_strftime_converts_tm_to_string_for_uppercase_y(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%Y", &tm), 4);
  CU_ASSERT_STRING_EQUAL(buf, "2003");
}

static void test_strftime_converts_tm_to_string_for_uppercase_z(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%Z", &tm), 3);
  CU_ASSERT_STRING_EQUAL(buf, "CET");
}

static void test_strftime_converts_tm_to_string_for_percent(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%%", &tm), 1);
  CU_ASSERT_STRING_EQUAL(buf, "%");
}

static void test_strftime_converts_tm_to_string_for_c(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%c", &tm), 24);
  CU_ASSERT_STRING_EQUAL(buf, "Wed Mar  5 21:34:56 2003");
}

static void test_strftime_converts_tm_to_string_for_uppercase_d(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 1;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%D", &tm), 8);
  CU_ASSERT_STRING_EQUAL(buf, "02/05/03");
}

static void test_strftime_converts_tm_to_string_for_r(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%r", &tm), 11);
  CU_ASSERT_STRING_EQUAL(buf, "09:34:56 PM");
}

static void test_strftime_converts_tm_to_string_for_uppercase_r(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%R", &tm), 5);
  CU_ASSERT_STRING_EQUAL(buf, "21:34");
}

static void test_strftime_converts_tm_to_string_for_uppercase_t(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%T", &tm), 8);
  CU_ASSERT_STRING_EQUAL(buf, "21:34:56");
}

static void test_strftime_converts_tm_to_string_for_x(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 1;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%x", &tm), 8);
  CU_ASSERT_STRING_EQUAL(buf, "02/05/03");
}

static void test_strftime_converts_tm_to_string_for_uppercase_x(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%X", &tm), 8);
  CU_ASSERT_STRING_EQUAL(buf, "21:34:56");
}

static void test_strftime_converts_tm_to_string_for_uppercase_d_and_uppercase_t(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 1;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%D %T", &tm), 17);
  CU_ASSERT_STRING_EQUAL(buf, "02/05/03 21:34:56");
}

static void test_strftime_converts_tm_to_string_for_invalid_conversion_specificator(void)
{
  static char buf[100];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 100, "%q", &tm), 2);
  CU_ASSERT_STRING_EQUAL(buf, "%q");
}

static void test_strftime_returns_0_for_too_small_buffer(void)
{
  static char buf[20];
  static struct retrolibc_tm tm;
  initialize_retrolibc_strftime();
  putenv("RETROLIBC_TZ=CET-1");
  tm.tm_sec = 56;
  tm.tm_min = 34;
  tm.tm_hour = 21;
  tm.tm_mday = 5;
  tm.tm_mon = 2;
  tm.tm_year = 2003 - 1900;
  tm.tm_gmtoff = 60 * 60;
  tm.tm_zone = "CET";
  CU_ASSERT_EQUAL(retrolibc_strftime(buf, 20, "%c", &tm), 0);
  CU_ASSERT_EQUAL(memcmp(buf, "Wed Mar  5 21:34:56 ", 20), 0);
}

int add_strftime_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("strftime suite", initialize_strftime, cleanup_strftime)) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string", test_strftime_converts_tm_to_string) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for a", test_strftime_converts_tm_to_string_for_a) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for A", test_strftime_converts_tm_to_string_for_uppercase_a) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for b", test_strftime_converts_tm_to_string_for_b) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for h", test_strftime_converts_tm_to_string_for_h) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for B", test_strftime_converts_tm_to_string_for_uppercase_b) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for C", test_strftime_converts_tm_to_string_for_uppercase_c) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for d", test_strftime_converts_tm_to_string_for_d) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for e", test_strftime_converts_tm_to_string_for_e) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for H", test_strftime_converts_tm_to_string_for_uppercase_h) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for I", test_strftime_converts_tm_to_string_for_uppercase_i) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for I and 0 ", test_strftime_converts_tm_to_string_for_uppercase_i_and_0) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for I and 12", test_strftime_converts_tm_to_string_for_uppercase_i_and_12) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for j", test_strftime_converts_tm_to_string_for_j) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for m", test_strftime_converts_tm_to_string_for_m) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for M", test_strftime_converts_tm_to_string_for_uppercase_m) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for n", test_strftime_converts_tm_to_string_for_n) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for p", test_strftime_converts_tm_to_string_for_p) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for p and 0 ", test_strftime_converts_tm_to_string_for_p_and_0) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for p and 12", test_strftime_converts_tm_to_string_for_p_and_12) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for s", test_strftime_converts_tm_to_string_for_s) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for S", test_strftime_converts_tm_to_string_for_uppercase_s) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for t", test_strftime_converts_tm_to_string_for_t) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for u", test_strftime_converts_tm_to_string_for_u) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for u and 2", test_strftime_converts_tm_to_string_for_u_and_2) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for U", test_strftime_converts_tm_to_string_for_uppercase_u) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for V", test_strftime_converts_tm_to_string_for_uppercase_v) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for w", test_strftime_converts_tm_to_string_for_w) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for w and 2", test_strftime_converts_tm_to_string_for_w_and_2) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for W", test_strftime_converts_tm_to_string_for_uppercase_w) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for y", test_strftime_converts_tm_to_string_for_y) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for Y", test_strftime_converts_tm_to_string_for_uppercase_y) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for Z", test_strftime_converts_tm_to_string_for_uppercase_z) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for %", test_strftime_converts_tm_to_string_for_percent) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for c", test_strftime_converts_tm_to_string_for_c) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for D", test_strftime_converts_tm_to_string_for_uppercase_d) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for r", test_strftime_converts_tm_to_string_for_r) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for R", test_strftime_converts_tm_to_string_for_uppercase_r) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for T", test_strftime_converts_tm_to_string_for_uppercase_t) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for x", test_strftime_converts_tm_to_string_for_x) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for X", test_strftime_converts_tm_to_string_for_uppercase_x) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for D and T", test_strftime_converts_tm_to_string_for_uppercase_d_and_uppercase_t) == NULL) return -1;
  if(CU_add_test(suite, "strftime converts tm to string for invalid conversion specificator", test_strftime_converts_tm_to_string_for_invalid_conversion_specificator) == NULL) return -1;
  if(CU_add_test(suite, "strftime returns 0 for too small buffer", test_strftime_returns_0_for_too_small_buffer) == NULL) return -1;
  return 0;
}

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

static void initialize_retrolibc_tzset()
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

static int initialize_tzset(void)
{ return 0; }

static int cleanup_tzset(void)
{ return 0; }

static void test_tzset_sets_timezone_for_cet(void)
{
  initialize_retrolibc_tzset();
  putenv("RETROLIBC_TZ=CET-1");
  retrolibc_tzset();
  CU_ASSERT_EQUAL(retrolibc___tz_std_offset, -60 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_std_zone, "CET");
  CU_ASSERT_EQUAL(retrolibc___tz_dst_offset, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_dst_zone, "");
  CU_ASSERT_EQUAL(retrolibc___are_tz_rules, 0);
  CU_ASSERT_EQUAL(retrolibc_daylight, 0);
  CU_ASSERT_EQUAL(retrolibc_timezone, -60 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[0], "CET");
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[1], "");
}

static void test_tzset_sets_timezone_for_cst(void)
{
  initialize_retrolibc_tzset();
  putenv("RETROLIBC_TZ=CST+8");
  retrolibc_tzset();
  CU_ASSERT_EQUAL(retrolibc___tz_std_offset, 8 * 60 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_std_zone, "CST");
  CU_ASSERT_EQUAL(retrolibc___tz_dst_offset, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_dst_zone, "");
  CU_ASSERT_EQUAL(retrolibc___are_tz_rules, 0);
  CU_ASSERT_EQUAL(retrolibc_daylight, 0);
  CU_ASSERT_EQUAL(retrolibc_timezone, 8 * 60 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[0], "CST");
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[1], "");
}

static void test_tzset_sets_timezone_for_offset_hh(void)
{
  initialize_retrolibc_tzset();
  putenv("RETROLIBC_TZ=ABC12");
  retrolibc_tzset();
  CU_ASSERT_EQUAL(retrolibc___tz_std_offset, 12 * 60 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_std_zone, "ABC");
  CU_ASSERT_EQUAL(retrolibc___tz_dst_offset, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_dst_zone, "");
  CU_ASSERT_EQUAL(retrolibc___are_tz_rules, 0);
  CU_ASSERT_EQUAL(retrolibc_daylight, 0);
  CU_ASSERT_EQUAL(retrolibc_timezone, 12 * 60 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[0], "ABC");
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[1], "");
}

static void test_tzset_sets_timezone_for_offset_hh_mm(void)
{
  initialize_retrolibc_tzset();
  putenv("RETROLIBC_TZ=ABC12:34");
  retrolibc_tzset();
  CU_ASSERT_EQUAL(retrolibc___tz_std_offset, 12 * 60 * 60 + 34 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_std_zone, "ABC");
  CU_ASSERT_EQUAL(retrolibc___tz_dst_offset, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_dst_zone, "");
  CU_ASSERT_EQUAL(retrolibc___are_tz_rules, 0);
  CU_ASSERT_EQUAL(retrolibc_daylight, 0);
  CU_ASSERT_EQUAL(retrolibc_timezone, 12 * 60 * 60 + 34 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[0], "ABC");
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[1], "");
}

static void test_tzset_sets_timezone_for_offset_hh_mm_ss(void)
{
  initialize_retrolibc_tzset();
  putenv("RETROLIBC_TZ=ABC12:34:56");
  retrolibc_tzset();
  CU_ASSERT_EQUAL(retrolibc___tz_std_offset, 12 * 60 * 60 + 34 * 60 + 56);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_std_zone, "ABC");
  CU_ASSERT_EQUAL(retrolibc___tz_dst_offset, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_dst_zone, "");
  CU_ASSERT_EQUAL(retrolibc___are_tz_rules, 0);
  CU_ASSERT_EQUAL(retrolibc_daylight, 0);
  CU_ASSERT_EQUAL(retrolibc_timezone, 12 * 60 * 60 + 34 * 60 + 56);
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[0], "ABC");
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[1], "");
}

static void test_tzset_sets_timezone_for_0_rules(void)
{
  initialize_retrolibc_tzset();
  putenv("RETROLIBC_TZ=CET-1CEST-3,85/03:00:00,300/04:00:00");
  retrolibc_tzset();
  CU_ASSERT_EQUAL(retrolibc___tz_std_offset, -60 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_std_zone, "CET");
  CU_ASSERT_EQUAL(retrolibc___tz_dst_offset, -3 * 60 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_dst_zone, "CEST");
  CU_ASSERT_EQUAL(retrolibc___are_tz_rules, 1);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_std_to_dst.type, '\0');
  CU_ASSERT_EQUAL(retrolibc___tz_rule_std_to_dst.u.n, 85);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_std_to_dst.time, 3 * 60 * 60);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_dst_to_std.type, '\0');
  CU_ASSERT_EQUAL(retrolibc___tz_rule_dst_to_std.u.n, 300);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_dst_to_std.time, 4 * 60 * 60);
  CU_ASSERT_EQUAL(retrolibc_daylight, 1);
  CU_ASSERT_EQUAL(retrolibc_timezone, -60  * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[0], "CET");
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[1], "CEST");
}

static void test_tzset_sets_timezone_for_j_rules(void)
{
  initialize_retrolibc_tzset();
  putenv("RETROLIBC_TZ=CET-1CEST,J85,J300/03:00:00");
  retrolibc_tzset();
  CU_ASSERT_EQUAL(retrolibc___tz_std_offset, -60 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_std_zone, "CET");
  CU_ASSERT_EQUAL(retrolibc___tz_dst_offset, -2 * 60 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_dst_zone, "CEST");
  CU_ASSERT_EQUAL(retrolibc___are_tz_rules, 1);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_std_to_dst.type, 'J');
  CU_ASSERT_EQUAL(retrolibc___tz_rule_std_to_dst.u.n, 85);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_std_to_dst.time, 2 * 60 * 60);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_dst_to_std.type, 'J');
  CU_ASSERT_EQUAL(retrolibc___tz_rule_dst_to_std.u.n, 300);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_dst_to_std.time, 3 * 60 * 60);
  CU_ASSERT_EQUAL(retrolibc_daylight, 1);
  CU_ASSERT_EQUAL(retrolibc_timezone, -60  * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[0], "CET");
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[1], "CEST");
}

static void test_tzset_sets_timezone_for_m_rules(void)
{
  initialize_retrolibc_tzset();
  putenv("RETROLIBC_TZ=CET-1CEST,M3.5.0,M10.5.0/3");
  retrolibc_tzset();
  CU_ASSERT_EQUAL(retrolibc___tz_std_offset, -60 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_std_zone, "CET");
  CU_ASSERT_EQUAL(retrolibc___tz_dst_offset, -2 * 60 * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc___tz_dst_zone, "CEST");
  CU_ASSERT_EQUAL(retrolibc___are_tz_rules, 1);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_std_to_dst.type, 'M');
  CU_ASSERT_EQUAL(retrolibc___tz_rule_std_to_dst.u.m.m, 3);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_std_to_dst.u.m.w, 5);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_std_to_dst.u.m.d, 0);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_std_to_dst.time, 2 * 60 * 60);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_dst_to_std.type, 'M');
  CU_ASSERT_EQUAL(retrolibc___tz_rule_dst_to_std.u.m.m, 10);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_dst_to_std.u.m.w, 5);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_dst_to_std.u.m.d, 0);
  CU_ASSERT_EQUAL(retrolibc___tz_rule_dst_to_std.time, 3 * 60 * 60);
  CU_ASSERT_EQUAL(retrolibc_daylight, 1);
  CU_ASSERT_EQUAL(retrolibc_timezone, -60  * 60);
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[0], "CET");
  CU_ASSERT_STRING_EQUAL(retrolibc_tzname[1], "CEST");
}

int add_tzset_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("tzset suite", initialize_tzset, cleanup_tzset)) == NULL) return -1;
  if(CU_add_test(suite, "tzset sets timezone for CET", test_tzset_sets_timezone_for_cet) == NULL) return -1;
  if(CU_add_test(suite, "tzset sets timezone for CST", test_tzset_sets_timezone_for_cst) == NULL) return -1;
  if(CU_add_test(suite, "tzset sets timezone for offset hh", test_tzset_sets_timezone_for_offset_hh) == NULL) return -1;
  if(CU_add_test(suite, "tzset sets timezone for offset hh:mm", test_tzset_sets_timezone_for_offset_hh_mm) == NULL) return -1;
  if(CU_add_test(suite, "tzset sets timezone for offset hh:mm:ss", test_tzset_sets_timezone_for_offset_hh_mm_ss) == NULL) return -1;
  if(CU_add_test(suite, "tzset sets timezone for 0 rules", test_tzset_sets_timezone_for_0_rules) == NULL) return -1;
  if(CU_add_test(suite, "tzset sets timezone for J rules", test_tzset_sets_timezone_for_j_rules) == NULL) return -1;
  if(CU_add_test(suite, "tzset sets timezone for M rules", test_tzset_sets_timezone_for_m_rules) == NULL) return -1;
  return 0;
}

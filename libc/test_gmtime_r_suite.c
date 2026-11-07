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

static int initialize_gmtime_r(void)
{ return 0; }

static int cleanup_gmtime_r(void)
{ return 0; }

int is_leap(int year)
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
    retrolibc_gmtime_r(&t, &tm);
    if(tm.tm_year + 1900 != y) break;
    if(tm.tm_sec != 0) break;
    if(tm.tm_min != 0) break;
    if(tm.tm_hour != 0) break;
    if(tm.tm_mday != 1) break;
    if(tm.tm_mon != 0) break;
    t += 365 * 24 * 60 * 60;
    if(is_leap(y)) t += 24 * 60 * 60;
  }
  CU_ASSERT_EQUAL(y, max_y + 1);
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
    t += 365 * 24 * 60 * 60;
    if(is_leap(y)) t += 24 * 60 * 60;
  }
  CU_ASSERT_EQUAL(y, max_y + 1);
}

int add_gmtime_r_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("gmtime_r suite", initialize_gmtime_r, cleanup_gmtime_r)) == NULL) return -1;
  if(CU_add_test(suite, "gmtime_r returns year", test_gmtime_r_returns_year) == NULL) return -1;
  if(CU_add_test(suite, "mktime returns seconds for year", test_mktime_returns_seconds_for_year) == NULL) return -1;
  return 0;
}

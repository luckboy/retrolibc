/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <stdlib.h>
#include <time.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"
#include "test_file_helpers.h"

unsigned test_seed(void);

static int initialize_getdate(void)
{ return 0; }

static int cleanup_getdate(void)
{
  clean_current_dir();
  return 0;
}

static void test_getdate_converts_date_string_to_tm(void)
{
  static struct tm *tm;
  clean_current_dir();
  CU_ASSERT_FATAL(write_to_file("test.txt", "%c\n%D %T\n%Y-%m-%dT%H:%M:%SZ\n"));
  putenv("RETROLIBC_DATEMSK=test.txt");
  retrolibc_getdate_err = 0;
  tm = retrolibc_getdate("Wed Mar  5 21:34:56 2003");
  CU_ASSERT_PTR_NOT_NULL(tm);
  CU_ASSERT_EQUAL(tm->tm_sec, 56);
  CU_ASSERT_EQUAL(tm->tm_min, 34);
  CU_ASSERT_EQUAL(tm->tm_hour, 21);
  CU_ASSERT_EQUAL(tm->tm_mday, 5);
  CU_ASSERT_EQUAL(tm->tm_mon, 2);
  CU_ASSERT_EQUAL(tm->tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(tm->tm_wday, 3);
  CU_ASSERT_EQUAL(retrolibc_getdate_err, 0);
  retrolibc_getdate_err = 0;
  tm = retrolibc_getdate("02/05/03 21:34:56");
  CU_ASSERT_PTR_NOT_NULL(tm);
  CU_ASSERT_EQUAL(tm->tm_sec, 56);
  CU_ASSERT_EQUAL(tm->tm_min, 34);
  CU_ASSERT_EQUAL(tm->tm_hour, 21);
  CU_ASSERT_EQUAL(tm->tm_mday, 5);
  CU_ASSERT_EQUAL(tm->tm_mon, 1);
  CU_ASSERT_EQUAL(tm->tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(retrolibc_getdate_err, 0);
  retrolibc_getdate_err = 0;
  tm = retrolibc_getdate("2003-03-5T21:34:56Z");
  CU_ASSERT_PTR_NOT_NULL(tm);
  CU_ASSERT_EQUAL(tm->tm_sec, 56);
  CU_ASSERT_EQUAL(tm->tm_min, 34);
  CU_ASSERT_EQUAL(tm->tm_hour, 21);
  CU_ASSERT_EQUAL(tm->tm_mday, 5);
  CU_ASSERT_EQUAL(tm->tm_mon, 2);
  CU_ASSERT_EQUAL(tm->tm_year, 2003 - 1900);
  CU_ASSERT_EQUAL(retrolibc_getdate_err, 0);
}

static void test_getdate_complains_on_no_line_in_template(void)
{
  static struct tm *tm;
  clean_current_dir();
  CU_ASSERT_FATAL(write_to_file("test.txt", "%c\n%D %T\n%Y-%m-%dT%H:%M:%SZ\n"));
  putenv("RETROLIBC_DATEMSK=test.txt");
  retrolibc_getdate_err = 0;
  tm = retrolibc_getdate("21:34:56 02/05/03");
  CU_ASSERT_PTR_NULL(tm);
  CU_ASSERT_EQUAL(retrolibc_getdate_err, 7);
}

static void test_getdate_complains_on_datemsk_is_not_defined(void)
{
  static struct tm *tm;
  clean_current_dir();
  putenv("RETROLIBC_DATEMSK");
  retrolibc_getdate_err = 0;
  tm = retrolibc_getdate("21:34:56 02/05/03");
  CU_ASSERT_PTR_NULL(tm);
  CU_ASSERT_EQUAL(retrolibc_getdate_err, 1);
}

static void test_getdate_complains_on_datemsk_is_empty(void)
{
  static struct tm *tm;
  clean_current_dir();
  putenv("RETROLIBC_DATEMSK=");
  retrolibc_getdate_err = 0;
  tm = retrolibc_getdate("21:34:56 02/05/03");
  CU_ASSERT_PTR_NULL(tm);
  CU_ASSERT_EQUAL(retrolibc_getdate_err, 1);
}

static void test_getdate_complains_on_template_file_can_not_be_opened_for_reading(void)
{
  static struct tm *tm;
  clean_current_dir();
  putenv("RETROLIBC_DATEMSK=test.txt");
  retrolibc_getdate_err = 0;
  tm = retrolibc_getdate("21:34:56 02/05/03");
  CU_ASSERT_PTR_NULL(tm);
  CU_ASSERT_EQUAL(retrolibc_getdate_err, 2);
}

int add_getdate_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("getdate suite", initialize_getdate, cleanup_getdate)) == NULL) return -1;
  if(CU_add_test(suite, "getdate converts date string to tm", test_getdate_converts_date_string_to_tm) == NULL) return -1;
  if(CU_add_test(suite, "getdate complains on no line in template", test_getdate_complains_on_no_line_in_template) == NULL) return -1;
  if(CU_add_test(suite, "getdate complains on DATEMSK isn't defined", test_getdate_complains_on_datemsk_is_not_defined) == NULL) return -1;
  if(CU_add_test(suite, "getdate complains on DATEMSK is empty", test_getdate_complains_on_datemsk_is_empty) == NULL) return -1;
  if(CU_add_test(suite, "getdate complains on template file can't be opened for reading", test_getdate_complains_on_template_file_can_not_be_opened_for_reading) == NULL) return -1;
  return 0;
}

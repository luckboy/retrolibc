/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <limits.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"

static int initialize_ulltostr(void)
{ return 0; }

static int cleanup_ulltostr(void)
{ return 0; }

static void test___ulltostr_converts_number_to_string_for_base_10(void)
{
  static char buf[65];
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 1234ULL, 10, 0), "1234");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 3456ULL, 10, 0), "3456");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 123499ULL, 10, 0), "123499");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 12348ULL, 10, 0), "12348");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0ULL, 10, 0), "0");
}

static void test___ulltostr_converts_number_to_string_for_base_2(void)
{
  static char buf[65];
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 10ULL, 2, 0), "1010");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 11ULL, 2, 0), "1011");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 7ULL, 2, 0), "111");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0ULL, 2, 0), "0");
}

static void test___ulltostr_converts_number_to_string_for_base_8(void)
{
  static char buf[65];
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 01234ULL, 8, 0), "1234");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 03456ULL, 8, 0), "3456");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0123477ULL, 8, 0), "123477");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 012346ULL, 8, 0), "12346");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0ULL, 8, 0), "0");
}

static void test___ulltostr_converts_number_to_string_for_base_16(void)
{
  static char buf[65];
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0x1234ULL, 16, 0), "1234");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0x12afULL, 16, 0), "12af");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0xfa1299ULL, 16, 0), "fa1299");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0x1234eULL, 16, 0), "1234e");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0ULL, 16, 0), "0");
}

static void test___ulltostr_converts_number_to_string_for_base_36(void)
{
  static char buf[65];
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 49360ULL, 36, 0), "1234");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 49391ULL, 36, 0), "123z");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 1371ULL, 36, 0), "123");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0ULL, 36, 0), "0");
}

static void test___ulltostr_converts_max_number_to_string(void)
{
  static char buf[65];
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, ULLONG_MAX, 10, 0), "18446744073709551615");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, ULLONG_MAX, 16, 0), "ffffffffffffffff");
}

static void test___ulltostr_converts_number_to_string_for_base_10_and_uppercase(void)
{
  static char buf[65];
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 1234ULL, 10, 1), "1234");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 3456ULL, 10, 1), "3456");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 123499ULL, 10, 1), "123499");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 12348ULL, 10, 1), "12348");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0ULL, 10, 1), "0");
}

static void test___ulltostr_converts_number_to_string_for_base_2_and_uppercase(void)
{
  static char buf[65];
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 10ULL, 2, 1), "1010");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 11ULL, 2, 1), "1011");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 7ULL, 2, 1), "111");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0ULL, 2, 1), "0");
}

static void test___ulltostr_converts_number_to_string_for_base_8_and_uppercase(void)
{
  static char buf[65];
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 01234ULL, 8, 1), "1234");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 03456ULL, 8, 1), "3456");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0123477ULL, 8, 1), "123477");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 012346ULL, 8, 1), "12346");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0ULL, 8, 1), "0");
}

static void test___ulltostr_converts_number_to_string_for_base_16_and_uppercase(void)
{
  static char buf[65];
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0x1234ULL, 16, 1), "1234");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0x12afULL, 16, 1), "12AF");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0xfa1299ULL, 16, 1), "FA1299");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0x1234eULL, 16, 1), "1234E");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0ULL, 16, 1), "0");
}

static void test___ulltostr_converts_number_to_string_for_base_36_and_uppercase(void)
{
  static char buf[65];
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 49360ULL, 36, 1), "1234");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 49391ULL, 36, 1), "123Z");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 1371ULL, 36, 1), "123");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, 0ULL, 36, 1), "0");
}

static void test___ulltostr_converts_max_number_to_string_for_uppercase(void)
{
  static char buf[65];
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, ULLONG_MAX, 10, 1), "18446744073709551615");
  CU_ASSERT_STRING_EQUAL(retrolibc___ulltostr(buf, ULLONG_MAX, 16, 1), "FFFFFFFFFFFFFFFF");
}

static void test___ulltostr_complains_on_invalid_base(void)
{
  static char buf[65];
  CU_ASSERT_PTR_NULL(retrolibc___ulltostr(buf, 1234ULL, 1, 0));
  CU_ASSERT_PTR_NULL(retrolibc___ulltostr(buf, 1234ULL, 37, 0));
  CU_ASSERT_PTR_NULL(retrolibc___ulltostr(buf, 1234ULL, 0, 0));
}

int add_ulltostr_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("ulltostr suite", initialize_ulltostr, cleanup_ulltostr)) == NULL) return -1;
  if(CU_add_test(suite, "__ulltostr converts number to string for base 10", test___ulltostr_converts_number_to_string_for_base_10) == NULL) return -1;
  if(CU_add_test(suite, "__ulltostr converts number to string for base 2", test___ulltostr_converts_number_to_string_for_base_2) == NULL) return -1;
  if(CU_add_test(suite, "__ulltostr converts number to string for base 8", test___ulltostr_converts_number_to_string_for_base_8) == NULL) return -1;
  if(CU_add_test(suite, "__ulltostr converts number to string for base 16", test___ulltostr_converts_number_to_string_for_base_16) == NULL) return -1;
  if(CU_add_test(suite, "__ulltostr converts number to string for base 36", test___ulltostr_converts_number_to_string_for_base_36) == NULL) return -1;
  if(CU_add_test(suite, "__ulltostr converts max number to string", test___ulltostr_converts_max_number_to_string) == NULL) return -1;
  if(CU_add_test(suite, "__ulltostr converts number to string for base 10 and uppercase", test___ulltostr_converts_number_to_string_for_base_10_and_uppercase) == NULL) return -1;
  if(CU_add_test(suite, "__ulltostr converts number to string for base 2 and uppercase", test___ulltostr_converts_number_to_string_for_base_2_and_uppercase) == NULL) return -1;
  if(CU_add_test(suite, "__ulltostr converts number to string for base 8 and uppercase", test___ulltostr_converts_number_to_string_for_base_8_and_uppercase) == NULL) return -1;
  if(CU_add_test(suite, "__ulltostr converts number to string for base 16 and uppercase", test___ulltostr_converts_number_to_string_for_base_16_and_uppercase) == NULL) return -1;
  if(CU_add_test(suite, "__ulltostr converts number to string for base 36 and uppercase", test___ulltostr_converts_number_to_string_for_base_36_and_uppercase) == NULL) return -1;
  if(CU_add_test(suite, "__ulltostr converts max number to string for uppercase", test___ulltostr_converts_max_number_to_string_for_uppercase) == NULL) return -1;
  if(CU_add_test(suite, "__ulltostr complains on invalid base", test___ulltostr_complains_on_invalid_base) == NULL) return -1;
  return 0;
}

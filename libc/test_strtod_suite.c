/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <errno.h>
#include <math.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"

static int initialize_strtod(void)
{ return 0; }

static int cleanup_strtod(void)
{ return 0; }

static void test_strtod_converts_string_to_number(void)
{
  const char *s;
  char *endptr;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 1234.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "3456A";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 3456.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "123499a";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 123499.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "01234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 1234.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
  s = "1234Ea";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 1234.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "1234ea";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 1234.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "1234E+";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 1234.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "1234e+";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 1234.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "1234E-";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 1234.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "1234e-";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 1234.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 0.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 1);
}

static void test_strtod_converts_string_to_number_for_dot(void)
{
  const char *s;
  char *endptr;
  s = "12.34";
  errno = 0;
  CU_ASSERT(fabs(retrolibc_strtod(s, &endptr) - 12.34) <= 0.0001);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
  s = "34.56A";
  errno = 0;
  CU_ASSERT(fabs(retrolibc_strtod(s, &endptr) - 34.56) <= 0.0001);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
  s = "123.499a";
  errno = 0;
  CU_ASSERT(fabs(retrolibc_strtod(s, &endptr) - 123.499) <= 0.0001);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 7);
  s = ".123.";
  errno = 0;
  CU_ASSERT(fabs(retrolibc_strtod(s, &endptr) - 0.123) <= 0.0001);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "123..";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 123.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
}

static void test_strtod_converts_string_to_number_for_exponent(void)
{
  const char *s;
  char *endptr;
  s = "12.34E9";
  errno = 0;
  CU_ASSERT(fabs(retrolibc_strtod(s, &endptr) - 12.34e9) <= 0.0001e9);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 7);
  s = "12.34e9";
  errno = 0;
  CU_ASSERT(fabs(retrolibc_strtod(s, &endptr) - 12.34e9) <= 0.0001e9);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 7);
  s = "34.56E-12";
  errno = 0;
  CU_ASSERT(fabs(retrolibc_strtod(s, &endptr) - 34.56e-12) <= 0.0001e-12);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 9);
  s = "34.56e-12";
  errno = 0;
  CU_ASSERT(fabs(retrolibc_strtod(s, &endptr) - 34.56e-12) <= 0.0001e-12);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 9);
  s = "123.499E+23";
  errno = 0;
  CU_ASSERT(fabs(retrolibc_strtod(s, &endptr) - 123.499e23) <= 0.0001e23);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 11);
  s = "123.499e+23";
  errno = 0;
  CU_ASSERT(fabs(retrolibc_strtod(s, &endptr) - 123.499e23) <= 0.0001e23);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 11);
  s = "0E500";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 0.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
   s = "0e500";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 0.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
  s = "0E-500";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 0.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "0e-500";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 0.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
}

static void test_strtod_converts_string_to_number_for_sign(void)
{
  const char *s;
  char *endptr;
  s = "+1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 1234.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
  s = "-1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), -1234.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
}

static void test_strtod_converts_string_to_number_for_spaces(void)
{
  const char *s;
  char *endptr;
  s = " \t 1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 1234.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 7);
  s = "\t\t-1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), -1234.0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 7);
}

static void test_strtod_complains_on_overflow(void)
{
  const char *s;
  char *endptr;
  s = "1.234E500";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), HUGE_VAL);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 9);
  s = "-1.234E500";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), -HUGE_VAL);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 10);
}

static void test_strtod_complains_on_underflow(void)
{
  const char *s;
  char *endptr;
  s = "1.234E-500";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtod(s, &endptr), 0.0);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 10);
}

int add_strtod_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("strtod suite", initialize_strtod, cleanup_strtod)) == NULL) return -1;
  if(CU_add_test(suite, "strtod converts string to number", test_strtod_converts_string_to_number) == NULL) return -1;
  if(CU_add_test(suite, "strtod converts string to number for dot", test_strtod_converts_string_to_number_for_dot) == NULL) return -1;
  if(CU_add_test(suite, "strtod converts string to number for exponent", test_strtod_converts_string_to_number_for_exponent) == NULL) return -1;
  if(CU_add_test(suite, "strtod converts string to number for sign", test_strtod_converts_string_to_number_for_sign) == NULL) return -1;
  if(CU_add_test(suite, "strtod converts string to number for spaces", test_strtod_converts_string_to_number_for_spaces) == NULL) return -1;
  if(CU_add_test(suite, "strtod complains on overflow", test_strtod_complains_on_overflow) == NULL) return -1;
  if(CU_add_test(suite, "strtod complains on underflow", test_strtod_complains_on_underflow) == NULL) return -1;
  return 0;
}

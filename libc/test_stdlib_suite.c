/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <errno.h>
#include <limits.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"

#ifndef LONG_BIT
#define LONG_BIT        __WORDSIZE
#endif

static int initialize_stdlib(void)
{ return 0; }

static int cleanup_stdlib(void)
{ return 0; }

static void test_atof_converts_string_to_number(void)
{
  CU_ASSERT_EQUAL(retrolibc_atof("1234"), 1234.0);
  CU_ASSERT_EQUAL(retrolibc_atof("-1234"), -1234.0);
  CU_ASSERT(fabs(retrolibc_atof("12.34") - 12.34) <= 0.0001);
  CU_ASSERT(fabs(retrolibc_atof("-12.34") - -12.34) <= 0.0001);
}

static void test_atoi_converts_string_to_number(void)
{
  CU_ASSERT_EQUAL(retrolibc_atoi("1234"), 1234);
  CU_ASSERT_EQUAL(retrolibc_atoi("-1234"), -1234);
}

static void test_atol_converts_string_to_number(void)
{
  CU_ASSERT_EQUAL(retrolibc_atol("1234"), 1234L);
  CU_ASSERT_EQUAL(retrolibc_atol("-1234"), -1234L);
}

static void test_atoll_converts_string_to_number(void)
{
  CU_ASSERT_EQUAL(retrolibc_atoll("1234"), 1234LL);
  CU_ASSERT_EQUAL(retrolibc_atoll("-1234"), -1234LL);
}

static void test_ecvt_converts_number_to_string(void)
{
  int decpt, sign;
  CU_ASSERT_STRING_EQUAL(retrolibc_ecvt(12.34, 6, &decpt, &sign), "123400");
  CU_ASSERT_EQUAL(decpt, 2);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc_ecvt(34.56, 6, &decpt, &sign), "345600");
  CU_ASSERT_EQUAL(decpt, 2);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc_ecvt(123.499, 8, &decpt, &sign), "12349900");
  CU_ASSERT_EQUAL(decpt, 3);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc_ecvt(-12.34, 6, &decpt, &sign), "123400");
  CU_ASSERT_EQUAL(decpt, 2);
  CU_ASSERT_EQUAL(sign, 1);
  CU_ASSERT_STRING_EQUAL(retrolibc_ecvt(-34.56, 6, &decpt, &sign), "345600");
  CU_ASSERT_EQUAL(decpt, 2);
  CU_ASSERT_EQUAL(sign, 1);
  CU_ASSERT_STRING_EQUAL(retrolibc_ecvt(-123.499, 8, &decpt, &sign), "12349900");
  CU_ASSERT_EQUAL(decpt, 3);
  CU_ASSERT_EQUAL(sign, 1);
}

static void test_fcvt_converts_number_to_string(void)
{
  int decpt, sign;
  CU_ASSERT_STRING_EQUAL(retrolibc_fcvt(12.34, 6, &decpt, &sign), "12340000");
  CU_ASSERT_EQUAL(decpt, 2);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc_fcvt(34.56, 6, &decpt, &sign), "34560000");
  CU_ASSERT_EQUAL(decpt, 2);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc_fcvt(123.499, 8, &decpt, &sign), "12349900000");
  CU_ASSERT_EQUAL(decpt, 3);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc_fcvt(1.23e-10, 6, &decpt, &sign), "");
  CU_ASSERT_EQUAL(decpt, -6);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc_fcvt(-12.34, 6, &decpt, &sign), "12340000");
  CU_ASSERT_EQUAL(decpt, 2);
  CU_ASSERT_EQUAL(sign, 1);
  CU_ASSERT_STRING_EQUAL(retrolibc_fcvt(-34.56, 6, &decpt, &sign), "34560000");
  CU_ASSERT_EQUAL(decpt, 2);
  CU_ASSERT_EQUAL(sign, 1);
  CU_ASSERT_STRING_EQUAL(retrolibc_fcvt(-123.499, 8, &decpt, &sign), "12349900000");
  CU_ASSERT_EQUAL(decpt, 3);
  CU_ASSERT_EQUAL(sign, 1);
  CU_ASSERT_STRING_EQUAL(retrolibc_fcvt(-1.23e-10, 6, &decpt, &sign), "");
  CU_ASSERT_EQUAL(decpt, -6);
  CU_ASSERT_EQUAL(sign, 1);
}

static void test_gcvt_converts_number_to_string(void)
{
  static char buf[101];
  CU_ASSERT_STRING_EQUAL(retrolibc_gcvt(12.34, 6, buf), "12.34");
  CU_ASSERT_STRING_EQUAL(retrolibc_gcvt(34.56, 6, buf), "34.56");
  CU_ASSERT_STRING_EQUAL(retrolibc_gcvt(-12.34, 6, buf), "-12.34");
  CU_ASSERT_STRING_EQUAL(retrolibc_gcvt(-34.56, 6, buf), "-34.56");
}

int add_stdlib_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("stdlib suite", initialize_stdlib, cleanup_stdlib)) == NULL) return -1;
  if(CU_add_test(suite, "atof converts string to number", test_atof_converts_string_to_number) == NULL) return -1;
  if(CU_add_test(suite, "atoi converts string to number", test_atoi_converts_string_to_number) == NULL) return -1;
  if(CU_add_test(suite, "atol converts string to number", test_atol_converts_string_to_number) == NULL) return -1;
  if(CU_add_test(suite, "atoll converts string to number", test_atoll_converts_string_to_number) == NULL) return -1;
  if(CU_add_test(suite, "ecvt converts number to string", test_ecvt_converts_number_to_string) == NULL) return -1;
  if(CU_add_test(suite, "fcvt converts number to string", test_fcvt_converts_number_to_string) == NULL) return -1;
  if(CU_add_test(suite, "gcvt converts number to string", test_gcvt_converts_number_to_string) == NULL) return -1;
  return 0;
}

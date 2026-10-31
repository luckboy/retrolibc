/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <CUnit/Basic.h>
#include "retrolibc.h"

static int initialize_efcvt(void)
{ return 0; }

static int cleanup_efcvt(void)
{ return 0; }

static void test___efcvt_converts_number_to_string(void)
{
  char buf[501];
  int decpt, sign;
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(1234.0, 6, &decpt, &sign, buf, 500, 1), "123400");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(3456.0, 6, &decpt, &sign, buf, 500, 1), "345600");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(123499.0, 8, &decpt, &sign, buf, 500, 1), "12349900");
  CU_ASSERT_EQUAL(decpt, 6);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(0.0, 6, &decpt, &sign, buf, 500, 1), "000000");
  CU_ASSERT_EQUAL(decpt, 1);
  CU_ASSERT_EQUAL(sign, 0);
}

static void test___efcvt_converts_number_to_string_for_dot(void)
{
  char buf[501];
  int decpt, sign;
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(12.34, 6, &decpt, &sign, buf, 500, 1), "123400");
  CU_ASSERT_EQUAL(decpt, 2);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(34.56, 6, &decpt, &sign, buf, 500, 1), "345600");
  CU_ASSERT_EQUAL(decpt, 2);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(123.499, 8, &decpt, &sign, buf, 500, 1), "12349900");
  CU_ASSERT_EQUAL(decpt, 3);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(0.123, 5, &decpt, &sign, buf, 500, 1), "12300");
  CU_ASSERT_EQUAL(decpt, 0);
  CU_ASSERT_EQUAL(sign, 0);
}

static void test___efcvt_converts_number_to_string_for_exponent(void)
{
  char buf[501];
  int decpt, sign;
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(12.34e9, 6, &decpt, &sign, buf, 500, 1), "123400");
  CU_ASSERT_EQUAL(decpt, 11);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(34.56e-12, 6, &decpt, &sign, buf, 500, 1), "345600");
  CU_ASSERT_EQUAL(decpt, -10);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(123.499e23, 8, &decpt, &sign, buf, 500, 1), "12349900");
  CU_ASSERT_EQUAL(decpt, 26);
  CU_ASSERT_EQUAL(sign, 0);
}

static void test___efcvt_converts_number_to_string_for_sign(void)
{
  char buf[501];
  int decpt, sign;
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(-1234.0, 6, &decpt, &sign, buf, 500, 1), "123400");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 1);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(-3456.0, 6, &decpt, &sign, buf, 500, 1), "345600");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 1);
}

static void test___efcvt_converts_number_to_string_for_rounding(void)
{
  char buf[501];
  int decpt, sign;
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(1234.566, 6, &decpt, &sign, buf, 500, 1), "123457");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(3456.544, 6, &decpt, &sign, buf, 500, 1), "345654");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(1234.445, 6, &decpt, &sign, buf, 500, 1), "123445");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(999.9996, 6, &decpt, &sign, buf, 500, 1), "100000");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 0);
}

static void test___efcvt_converts_number_to_string_for_fcvt(void)
{
  char buf[501];
  int decpt, sign;
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(1234.0, 6, &decpt, &sign, buf, 500, 0), "1234000000");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(0.0123, 5, &decpt, &sign, buf, 500, 0), "1230");
  CU_ASSERT_EQUAL(decpt, -1);
  CU_ASSERT_EQUAL(sign, 0);
}

static void test___efcvt_converts_number_to_string_for_rounding_and_fcvt(void)
{
  char buf[501];
  int decpt, sign;
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(1234.566, 2, &decpt, &sign, buf, 500, 0), "123457");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(3456.544, 2, &decpt, &sign, buf, 500, 0), "345654");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(1234.445, 2, &decpt, &sign, buf, 500, 0), "123445");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(999.996, 2, &decpt, &sign, buf, 500, 0), "100000");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 0);
}

static void test___efcvt_converts_number_to_string_for_max_number_of_digits(void)
{
  char buf[501];
  int decpt, sign;
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(1234.0, 6, &decpt, &sign, buf, 4, 1), "1234");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 0);
  CU_ASSERT_STRING_EQUAL(retrolibc___efcvt(3456.0, 6, &decpt, &sign, buf, 4, 1), "3456");
  CU_ASSERT_EQUAL(decpt, 4);
  CU_ASSERT_EQUAL(sign, 0);
}

int add_efcvt_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("efcvt suite", initialize_efcvt, cleanup_efcvt)) == NULL) return -1;
  if(CU_add_test(suite, "__efcvt converts number to string", test___efcvt_converts_number_to_string) == NULL) return -1;
  if(CU_add_test(suite, "__efcvt converts number to string for dot", test___efcvt_converts_number_to_string_for_dot) == NULL) return -1;
  if(CU_add_test(suite, "__efcvt converts number to string for exponent", test___efcvt_converts_number_to_string_for_exponent) == NULL) return -1;
  if(CU_add_test(suite, "__efcvt converts number to string for sign", test___efcvt_converts_number_to_string_for_sign) == NULL) return -1;
  if(CU_add_test(suite, "__efcvt converts number to string for rounding", test___efcvt_converts_number_to_string_for_rounding) == NULL) return -1;
  if(CU_add_test(suite, "__efcvt converts number to string for fcvt", test___efcvt_converts_number_to_string_for_fcvt) == NULL) return -1;
  if(CU_add_test(suite, "__efcvt converts number to string for rounding and fcvt", test___efcvt_converts_number_to_string_for_rounding_and_fcvt) == NULL) return -1;
  if(CU_add_test(suite, "__efcvt converts number to string for max number of digits", test___efcvt_converts_number_to_string_for_max_number_of_digits) == NULL) return -1;
  return 0;
}

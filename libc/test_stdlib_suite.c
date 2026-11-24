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

char **retrolibc_environ;

static void test_a64l_converts_radix_64_string_to_number(void)
{
  CU_ASSERT_EQUAL(retrolibc_a64l("."), 0L);
  CU_ASSERT_EQUAL(retrolibc_a64l("/"), 1L);
  CU_ASSERT_EQUAL(retrolibc_a64l("0"), 2L);
  CU_ASSERT_EQUAL(retrolibc_a64l("9"), 11L);
  CU_ASSERT_EQUAL(retrolibc_a64l("A"), 12L);
  CU_ASSERT_EQUAL(retrolibc_a64l("Z"), 37L);
  CU_ASSERT_EQUAL(retrolibc_a64l("a"), 38L);
  CU_ASSERT_EQUAL(retrolibc_a64l("z"), 63L);
  CU_ASSERT_EQUAL(retrolibc_a64l("./"), 64L);
  CU_ASSERT_EQUAL(retrolibc_a64l("C34j"), 12345678L);
  CU_ASSERT_EQUAL(retrolibc_a64l("mutEz1"), -12345678L);
}

static void test_abs_calculates_absolute_value(void)
{
  CU_ASSERT_EQUAL(retrolibc_abs(1234), 1234);
  CU_ASSERT_EQUAL(retrolibc_abs(-3456), 3456);
}

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

static void test_div_calculates_quotient_and_remainder(void)
{
  retrolibc_div_t div = retrolibc_div(1234, 123);
  CU_ASSERT_EQUAL(div.quot, 1234 / 123);
  CU_ASSERT_EQUAL(div.rem, 1234 % 123);
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

static void test_getenv_returns_value(void)
{
  static char *start_environ[] = {
    "PATH=/bin:/sbin:/usr/bin/:/usr/sbin",
    "ABC=abc",
    "DEF=def",
    "GHI=ghi",
    NULL
  };
  retrolibc_environ = start_environ;
  CU_ASSERT_STRING_EQUAL(retrolibc_getenv("PATH"), "/bin:/sbin:/usr/bin/:/usr/sbin");
  CU_ASSERT_STRING_EQUAL(retrolibc_getenv("ABC"), "abc");
  CU_ASSERT_STRING_EQUAL(retrolibc_getenv("DEF"), "def");
  CU_ASSERT_STRING_EQUAL(retrolibc_getenv("GHI"), "ghi");
}

static void test_getenv_returns_null(void)
{
  static char *start_environ[] = {
    "PATH=/bin:/sbin:/usr/bin/:/usr/sbin",
    "DEF",
    "GHI=ghi",
    NULL
  };
  retrolibc_environ = start_environ;
  CU_ASSERT_PTR_NULL(retrolibc_getenv("ABC"));
  CU_ASSERT_PTR_NULL(retrolibc_getenv("DEF"));
}

static void test_l64a_converts_number_to_radix_64_string(void)
{
  CU_ASSERT_STRING_EQUAL(retrolibc_l64a(0L), "");
  CU_ASSERT_STRING_EQUAL(retrolibc_l64a(1L), "/");
  CU_ASSERT_STRING_EQUAL(retrolibc_l64a(2L), "0");
  CU_ASSERT_STRING_EQUAL(retrolibc_l64a(11L), "9");
  CU_ASSERT_STRING_EQUAL(retrolibc_l64a(12L), "A");
  CU_ASSERT_STRING_EQUAL(retrolibc_l64a(37L), "Z");
  CU_ASSERT_STRING_EQUAL(retrolibc_l64a(38L), "a");
  CU_ASSERT_STRING_EQUAL(retrolibc_l64a(63L), "z");
  CU_ASSERT_STRING_EQUAL(retrolibc_l64a(64L), "./");
  CU_ASSERT_STRING_EQUAL(retrolibc_l64a(12345678L), "C34j");
  CU_ASSERT_STRING_EQUAL(retrolibc_l64a(-12345678L), "mutEz1");
}

static void test_labs_calculates_absolute_value(void)
{
  CU_ASSERT_EQUAL(retrolibc_labs(1234L), 1234L);
  CU_ASSERT_EQUAL(retrolibc_labs(-3456L), 3456L);
}

static void test_ldiv_calculates_quotient_and_remainder(void)
{
  retrolibc_ldiv_t div = retrolibc_ldiv(1234L, 123L);
  CU_ASSERT_EQUAL(div.quot, 1234L / 123L);
  CU_ASSERT_EQUAL(div.rem, 1234L % 123L);
}

static void test_llabs_calculates_absolute_value(void)
{
  CU_ASSERT_EQUAL(retrolibc_llabs(1234LL), 1234LL);
  CU_ASSERT_EQUAL(retrolibc_llabs(-3456LL), 3456LL);
}

static void test_lldiv_calculates_quotient_and_remainder(void)
{
  retrolibc_lldiv_t div = retrolibc_lldiv(1234LL, 123LL);
  CU_ASSERT_EQUAL(div.quot, 1234LL / 123LL);
  CU_ASSERT_EQUAL(div.rem, 1234LL % 123LL);
}

static void test_putenv_sets_existent_environment_variable(void)
{
  static char *start_environ[] = {
    "PATH=/bin:/sbin:/usr/bin/:/usr/sbin",
    "ABC=abc",
    "DEF=def",
    "GHI=ghi",
    NULL
  };
  retrolibc_environ = start_environ;
  CU_ASSERT_EQUAL(retrolibc_putenv("DEF=def123"), 0);
  CU_ASSERT_PTR_EQUAL(retrolibc_environ, start_environ);
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[0], "PATH=/bin:/sbin:/usr/bin/:/usr/sbin");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[1], "ABC=abc");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[2], "DEF=def123");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[3], "GHI=ghi");
  CU_ASSERT_PTR_NULL(retrolibc_environ[4]);
  CU_ASSERT_EQUAL(retrolibc_putenv("GHI=ghi456"), 0);
  CU_ASSERT_PTR_EQUAL(retrolibc_environ, start_environ);
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[0], "PATH=/bin:/sbin:/usr/bin/:/usr/sbin");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[1], "ABC=abc");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[2], "DEF=def123");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[3], "GHI=ghi456");
  CU_ASSERT_PTR_NULL(retrolibc_environ[4]);
  free(retrolibc_environ[3]);
  free(retrolibc_environ[2]);
}

static void test_putenv_adds_new_environment_variable(void)
{
  static char *start_environ[] = {
    "PATH=/bin:/sbin:/usr/bin/:/usr/sbin",
    "ABC=abc",
    "DEF=def",
    "GHI=ghi",
    NULL
  };
  retrolibc_environ = start_environ;
  CU_ASSERT_EQUAL(retrolibc_putenv("JKL=jkl"), 0);
  CU_ASSERT_PTR_NOT_EQUAL(retrolibc_environ, start_environ);
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[0], "PATH=/bin:/sbin:/usr/bin/:/usr/sbin");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[1], "ABC=abc");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[2], "DEF=def");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[3], "GHI=ghi");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[4], "JKL=jkl");
  CU_ASSERT_PTR_NULL(retrolibc_environ[5]);
  CU_ASSERT_EQUAL(retrolibc_putenv("MNO=mno"), 0);
  CU_ASSERT_PTR_NOT_EQUAL(retrolibc_environ, start_environ);
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[0], "PATH=/bin:/sbin:/usr/bin/:/usr/sbin");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[1], "ABC=abc");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[2], "DEF=def");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[3], "GHI=ghi");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[4], "JKL=jkl");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[5], "MNO=mno");
  CU_ASSERT_PTR_NULL(retrolibc_environ[6]);
  free(retrolibc_environ[5]);
  free(retrolibc_environ[4]);
  free(retrolibc_environ);
}

static void test_putenv_removes_existent_environment_variable(void)
{
  static char *start_environ[] = {
    "PATH=/bin:/sbin:/usr/bin/:/usr/sbin",
    "ABC=abc",
    "DEF=def",
    "GHI=ghi",
    NULL
  };
  retrolibc_environ = start_environ;
  CU_ASSERT_EQUAL(retrolibc_putenv("ABC"), 0);
  CU_ASSERT_PTR_EQUAL(retrolibc_environ, start_environ);
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[0], "PATH=/bin:/sbin:/usr/bin/:/usr/sbin");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[1], "DEF=def");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[2], "GHI=ghi");
  CU_ASSERT_PTR_NULL(retrolibc_environ[3]);
  CU_ASSERT_EQUAL(retrolibc_putenv("GHI"), 0);
  CU_ASSERT_PTR_EQUAL(retrolibc_environ, start_environ);
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[0], "PATH=/bin:/sbin:/usr/bin/:/usr/sbin");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[1], "DEF=def");
  CU_ASSERT_PTR_NULL(retrolibc_environ[2]);
}

static void test_putenv_does_not_remove_non_existent_environment_variable(void)
{
  static char *start_environ[] = {
    "PATH=/bin:/sbin:/usr/bin/:/usr/sbin",
    "ABC=abc",
    "DEF=def",
    "GHI=ghi",
    NULL
  };
  retrolibc_environ = start_environ;
  CU_ASSERT_EQUAL(retrolibc_putenv("JKL"), 0);
  CU_ASSERT_PTR_EQUAL(retrolibc_environ, start_environ);
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[0], "PATH=/bin:/sbin:/usr/bin/:/usr/sbin");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[1], "ABC=abc");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[2], "DEF=def");
  CU_ASSERT_STRING_EQUAL(retrolibc_environ[3], "GHI=ghi");
  CU_ASSERT_PTR_NULL(retrolibc_environ[4]);
}

static void test_rand_r_generates_random_number(void)
{
  unsigned seed = 0x12345678;
  CU_ASSERT_EQUAL(retrolibc_rand_r(&seed), 0xb719151);
  CU_ASSERT_EQUAL(retrolibc_rand_r(&seed), 0x6f47bdb6);
  CU_ASSERT_EQUAL(retrolibc_rand_r(&seed), 0x2e1d6ab7);
}

int add_stdlib_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("stdlib suite", initialize_stdlib, cleanup_stdlib)) == NULL) return -1;
  if(CU_add_test(suite, "a64l converts radix-64 string to number", test_a64l_converts_radix_64_string_to_number) == NULL) return -1;
  if(CU_add_test(suite, "abs calculates absolute value", test_abs_calculates_absolute_value) == NULL) return -1;
  if(CU_add_test(suite, "atof converts string to number", test_atof_converts_string_to_number) == NULL) return -1;
  if(CU_add_test(suite, "atoi converts string to number", test_atoi_converts_string_to_number) == NULL) return -1;
  if(CU_add_test(suite, "atol converts string to number", test_atol_converts_string_to_number) == NULL) return -1;
  if(CU_add_test(suite, "atoll converts string to number", test_atoll_converts_string_to_number) == NULL) return -1;  
  if(CU_add_test(suite, "div calculates quotient and remainder", test_div_calculates_quotient_and_remainder) == NULL) return -1;
  if(CU_add_test(suite, "ecvt converts number to string", test_ecvt_converts_number_to_string) == NULL) return -1;
  if(CU_add_test(suite, "fcvt converts number to string", test_fcvt_converts_number_to_string) == NULL) return -1;
  if(CU_add_test(suite, "gcvt converts number to string", test_gcvt_converts_number_to_string) == NULL) return -1;
  if(CU_add_test(suite, "getenv returns value", test_getenv_returns_value) == NULL) return -1;
  if(CU_add_test(suite, "getenv returns null", test_getenv_returns_null) == NULL) return -1;
  if(CU_add_test(suite, "l64a converts number to radix-64 string", test_l64a_converts_number_to_radix_64_string) == NULL) return -1;
  if(CU_add_test(suite, "labs calculates absolute value", test_labs_calculates_absolute_value) == NULL) return -1;
  if(CU_add_test(suite, "ldiv calculates quotient and remainder", test_ldiv_calculates_quotient_and_remainder) == NULL) return -1;
  if(CU_add_test(suite, "llabs calculates absolute value", test_llabs_calculates_absolute_value) == NULL) return -1;
  if(CU_add_test(suite, "lldiv calculates quotient and remainder", test_lldiv_calculates_quotient_and_remainder) == NULL) return -1;
  if(CU_add_test(suite, "putenv sets existent environment variable", test_putenv_sets_existent_environment_variable) == NULL) return -1;
  if(CU_add_test(suite, "putenv adds new environment variable", test_putenv_adds_new_environment_variable) == NULL) return -1;
  if(CU_add_test(suite, "putenv removes existent environment variable", test_putenv_removes_existent_environment_variable) == NULL) return -1;
  if(CU_add_test(suite, "putenv doesn't remove non existent environment variable", test_putenv_does_not_remove_non_existent_environment_variable) == NULL) return -1;
  if(CU_add_test(suite, "rand_r generates random number", test_rand_r_generates_random_number) == NULL) return -1;  
  return 0;
}

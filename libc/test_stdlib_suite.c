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

int add_stdlib_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("stdlib suite", initialize_stdlib, cleanup_stdlib)) == NULL) return -1;
  if(CU_add_test(suite, "atoi converts string to number", test_atoi_converts_string_to_number) == NULL) return -1;
  if(CU_add_test(suite, "atol converts string to number", test_atol_converts_string_to_number) == NULL) return -1;
  if(CU_add_test(suite, "atoll converts string to number", test_atoll_converts_string_to_number) == NULL) return -1;
  return 0;
}

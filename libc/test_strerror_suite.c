/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <errno.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"

static int initialize_strerror(void)
{ return 0; }

static int cleanup_strerror(void)
{ return 0; }


static void test_strerror_returns_error_message(void)
{
  CU_ASSERT_STRING_EQUAL(retrolibc_strerror(0), "Success");
  CU_ASSERT_STRING_NOT_EQUAL(retrolibc_strerror(EPERM), "Unknown Error");
  CU_ASSERT_STRING_NOT_EQUAL(retrolibc_strerror(ENOENT), "Unknown Error");
  CU_ASSERT_STRING_NOT_EQUAL(retrolibc_strerror(EIO), "Unknown Error");
}

static void test_strerror_returns_unknown_error_message(void)
{
  CU_ASSERT_STRING_EQUAL(retrolibc_strerror(-1), "Unknown Error");
  CU_ASSERT_STRING_EQUAL(retrolibc_strerror(retrolibc_sys_nerr), "Unknown Error");
}

int add_strerror_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("strerror suite", initialize_strerror, cleanup_strerror)) == NULL) return -1;
  if(CU_add_test(suite, "strerror returns error message", test_strerror_returns_error_message) == NULL) return -1;
  if(CU_add_test(suite, "strerror returns unknown error message", test_strerror_returns_unknown_error_message) == NULL) return -1;
  return 0;
}

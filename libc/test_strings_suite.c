/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <string.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"

static int initialize_strings(void)
{ return 0; }

static int cleanup_strings(void)
{ return 0; }

static void test_bcmp_returns_zero(void)
{ CU_ASSERT_EQUAL(retrolibc_bcmp("abc\0def\0", "abc\0def\0", 8), 0); }

static void test_bcmp_returns_number_that_is_less_than_zero(void)
{ CU_ASSERT_EQUAL(retrolibc_bcmp("abc\0Def\0", "abc\0def\0", 8), 'D' - 'd'); }

static void test_bcmp_returns_number_that_is_greater_than_zero(void)
{ CU_ASSERT_EQUAL(retrolibc_bcmp("abc\0def\0", "abc\0dEf\0", 8), 'e' - 'E'); }

static void test_bcopy_copies_memory(void)
{
  static char buf[11] = "XXXXXXXXXX";
  retrolibc_bcopy("abc\0def\0", buf, 8);
  CU_ASSERT_EQUAL(memcmp(buf, "abc\0def\0XX", 10), 0);
}

static void test_bzero_fills_memory_with_0(void)
{
  static char buf[11] = "XXXXXXXXXX";
  retrolibc_bzero(buf, 8);
  CU_ASSERT_EQUAL(memcmp(buf, "\0\0\0\0\0\0\0\0XX", 10), 0);
}

static void test_ffs_returns_first_bit_index(void)
{
  CU_ASSERT_EQUAL(retrolibc_ffs(1), 1);
  CU_ASSERT_EQUAL(retrolibc_ffs(5), 1);
  CU_ASSERT_EQUAL(retrolibc_ffs(2), 2);
  CU_ASSERT_EQUAL(retrolibc_ffs(10), 2);
  CU_ASSERT_EQUAL(retrolibc_ffs(0x12300), 9);
  CU_ASSERT_EQUAL(retrolibc_ffs(0x80000000), 32);
}

static void test_ffs_returns_0(void)
{ CU_ASSERT_EQUAL(retrolibc_ffs(0), 0); }

static void test_index_returns_pointer_to_occurrence(void)
{
  char *s = "abcdef";
  char *s2 = "abcabc";
  CU_ASSERT_PTR_EQUAL(retrolibc_index(s, 'd'), s + 3);
  CU_ASSERT_PTR_EQUAL(retrolibc_index(s2, 'c'), s2 + 2);
  CU_ASSERT_PTR_EQUAL(retrolibc_index(s, '\0'), s + 6);
}

static void test_index_returns_null(void)
{
  char *s = "abcdef";
  CU_ASSERT_PTR_NULL(retrolibc_index(s, 'X'));
}

static void test_rindex_returns_pointer_to_occurrence(void)
{
  char *s = "abcdef";
  char *s2 = "abcabc";
  CU_ASSERT_PTR_EQUAL(retrolibc_rindex(s, 'd'), s + 3);
  CU_ASSERT_PTR_EQUAL(retrolibc_rindex(s2, 'b'), s2 + 4);
  CU_ASSERT_PTR_EQUAL(retrolibc_rindex(s, '\0'), s + 6);
}

static void test_rindex_returns_null(void)
{
  char *s = "abcdef";
  CU_ASSERT_PTR_NULL(retrolibc_rindex(s, 'X'));
}


static void test_strcasecmp_returns_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strcasecmp("abcdef", "abcdef"), 0);
  CU_ASSERT_EQUAL(retrolibc_strcasecmp("abcdef\0@hi", "abcdef\0ghi"), 0);
  CU_ASSERT_EQUAL(retrolibc_strcasecmp("ABCdef", "abcDEF"), 0);
  CU_ASSERT_EQUAL(retrolibc_strcasecmp("abcDEF\0@hi", "ABCdef\0ghi"), 0);
}

static void test_strcasecmp_returns_number_that_is_less_than_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strcasecmp("abc1ef", "abcDef"), '1' - 'd');
  CU_ASSERT_EQUAL(retrolibc_strcasecmp("abc", "abcDEF"), -'d');
}

static void test_strcasecmp_returns_number_that_is_greater_than_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strcasecmp("abcdEf", "abcd1f"), 'e' - '1');
  CU_ASSERT_EQUAL(retrolibc_strcasecmp("abcDEF", "abc"), 'd');
}

static void test_strncasecmp_returns_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abcdef", "abcdef", 8), 0);
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abcdef\0@hi", "abcdef\0ghi", 8), 0);
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abcdef", "abcdef", 3), 0);
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abcdef", "abcdef", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("ABCdef", "abcDEF", 8), 0);
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abcDEF\0@hi", "ABCdef\0ghi", 8), 0);
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("ABCdef", "abcDEF", 3), 0);
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abcDEF", "ABCdef", 0), 0);
}

static void test_strncasecmp_returns_number_that_is_less_than_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abc1ef", "abcDef", 8), '1' - 'd');
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abc", "abcDef", 8), -'d');
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abc1ef", "abcDef", 4), '1' - 'd');
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abc", "abcDef", 4), -'d');
}

static void test_strncasecmp_returns_number_that_is_greater_than_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abcdEf", "abcd1f", 8), 'e' - '1');
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abcDef", "abc", 8), 'd');
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abcdEf", "abcd1f", 5), 'e' - '1');
  CU_ASSERT_EQUAL(retrolibc_strncasecmp("abcDef", "abc", 5), 'd');
}

int add_strings_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("strings suite", initialize_strings, cleanup_strings)) == NULL) return -1;
  if(CU_add_test(suite, "bcmp returns zero", test_bcmp_returns_zero) == NULL) return -1;
  if(CU_add_test(suite, "bcmp returns number that is less than zero", test_bcmp_returns_number_that_is_less_than_zero) == NULL) return -1;
  if(CU_add_test(suite, "bcmp returns number that is greater than zero", test_bcmp_returns_number_that_is_greater_than_zero) == NULL) return -1;
  if(CU_add_test(suite, "bcopy copies memory", test_bcopy_copies_memory) == NULL) return -1;
  if(CU_add_test(suite, "bzero fills memory with 0", test_bzero_fills_memory_with_0) == NULL) return -1;
  if(CU_add_test(suite, "ffs returns bit index", test_ffs_returns_first_bit_index) == NULL) return -1;
  if(CU_add_test(suite, "ffs returns 0", test_ffs_returns_0) == NULL) return -1;
  if(CU_add_test(suite, "index returns pointer to occurrence", test_index_returns_pointer_to_occurrence) == NULL) return -1;
  if(CU_add_test(suite, "index returns null", test_index_returns_null) == NULL) return -1;
  if(CU_add_test(suite, "rindex returns pointer to occurrence", test_rindex_returns_pointer_to_occurrence) == NULL) return -1;
  if(CU_add_test(suite, "rindex returns null", test_rindex_returns_null) == NULL) return -1;
  if(CU_add_test(suite, "strcasecmp returns zero", test_strcasecmp_returns_zero) == NULL) return -1;
  if(CU_add_test(suite, "strcasecmp returns number that is less than zero", test_strcasecmp_returns_number_that_is_less_than_zero) == NULL) return -1;
  if(CU_add_test(suite, "strcasecmp returns number that is greater than zero", test_strcasecmp_returns_number_that_is_greater_than_zero) == NULL) return -1;
  if(CU_add_test(suite, "strncasecmp returns zero", test_strncasecmp_returns_zero) == NULL) return -1;
  if(CU_add_test(suite, "strncasecmp returns number that is less than zero", test_strncasecmp_returns_number_that_is_less_than_zero) == NULL) return -1;
  if(CU_add_test(suite, "strncasecmp returns number that is greater than zero", test_strncasecmp_returns_number_that_is_greater_than_zero) == NULL) return -1;
  return 0;
}

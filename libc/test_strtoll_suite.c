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

static int initialize_strtoll(void)
{ return 0; }

static int cleanup_strtoll(void)
{ return 0; }

static void test_strtoll_converts_string_to_number_for_base_10(void)
{
  const char *s;
  char *endptr;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), 1234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "3456A";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), 3456LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "123499a";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), 123499LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "01234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), 1234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
  s = "0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), 0LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 1);
}

static void test_strtoll_converts_string_to_number_for_base_2(void)
{
  const char *s;
  char *endptr;
  s = "1010";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 2), 10LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "10112";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 2), 11LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "01112";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 2), 7LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 2), 0LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 1);
}

static void test_strtoll_converts_string_to_number_for_base_8(void)
{
  const char *s;
  char *endptr;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 8), 01234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "34568";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 8), 03456LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "1234778";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 8), 0123477LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "01234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 8), 01234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
  s = "0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 8), 0LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 1);
}

static void test_strtoll_converts_string_to_number_for_base_16(void)
{
  const char *s;
  char *endptr;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 16), 0x1234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "12aFg";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 16), 0x12afLL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "fA1299G";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 16), 0xfa1299LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "0X1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 16), 0x1234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "0x1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 16), 0x1234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 16), 0x0LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 1);
}

static void test_strtoll_converts_string_to_number_for_base_36(void)
{
  const char *s;
  char *endptr;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 36), 49360LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "123Z";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 36), 49391LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "123z";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 36), 49391LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 36), 0LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 1);
}

static void test_strtoll_converts_string_to_number_for_base_0(void)
{
  const char *s;
  char *endptr;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 0), 1234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "01234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 0), 01234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
  s = "0X1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 0), 0x1234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "0x1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 0), 0x1234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 0), 0LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 1);
  s = "0x0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 0), 0x0LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 3);
}

static void test_strtoll_converts_string_to_number_for_sign(void)
{
  const char *s;
  char *endptr;
  s = "+1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), 1234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
  s = "-1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), -1234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
}

static void test_strtoll_converts_string_to_number_for_spaces(void)
{
  const char *s;
  char *endptr;
  s = " \t 1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), 1234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 7);
  s = "\t\t-1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), -1234LL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 7);
}

static void test_strtoll_returns_max_number_without_error(void)
{
  const char *s;
  char *endptr;
  s = "9223372036854775807";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), LLONG_MAX);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 19);
  s = "7fffffffffffffff";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 16), LLONG_MAX);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 16);
}

static void test_strtoll_returns_min_number_without_error(void)
{
  const char *s;
  char *endptr;
  s = "-9223372036854775808";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), LLONG_MIN);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 20);
  s = "-8000000000000000";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 16), LLONG_MIN);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 17);
}

static void test_strtoll_converts_string_to_number_without_endptr(void)
{
  const char *s;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, NULL, 10), 1234LL);
  CU_ASSERT_EQUAL(errno, 0);
}

static void test_strtoll_complains_on_overflow(void)
{
  const char *s;
  char *endptr;
  s = "18446744073709551616";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), LLONG_MAX);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 20);
  s = "10000000000000000";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 16), LLONG_MAX);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 17);
  s = "1000000000000000000000000";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), LLONG_MAX);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 25);
  s = "9223372036854775808";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), LLONG_MAX);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 19);
  s = "8000000000000000";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 16), LLONG_MAX);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 16);
  s = "-18446744073709551616";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), LLONG_MIN);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 21);
  s = "-10000000000000000";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 16), LLONG_MIN);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 18);
  s = "-1000000000000000000000000";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), LLONG_MIN);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 26);
  s = "-9223372036854775809";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 10), LLONG_MIN);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 20);
  s = "-8000000000000001";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 16), LLONG_MIN);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 17);
}

static void test_strtoll_complains_on_invalid_base(void)
{
  const char *s;
  char *endptr;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 1), 0);
  CU_ASSERT_EQUAL(errno, EINVAL);
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoll(s, &endptr, 37), 0);
  CU_ASSERT_EQUAL(errno, EINVAL);
}

static void test_strtoull_converts_string_to_number_for_base_10(void)
{
  const char *s;
  char *endptr;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 10), 1234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "3456A";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 10), 3456ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "123499a";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 10), 123499ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "01234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 10), 1234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
  s = "0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 10), 0ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 1);
}

static void test_strtoull_converts_string_to_number_for_base_2(void)
{
  const char *s;
  char *endptr;
  s = "1010";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 2), 10ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "10112";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 2), 11ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "01112";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 2), 7ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 2), 0ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 1);
}

static void test_strtoull_converts_string_to_number_for_base_8(void)
{
  const char *s;
  char *endptr;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 8), 01234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "34568";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 8), 03456ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "1234778";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 8), 0123477ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "01234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 8), 01234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
  s = "0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 8), 0ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 1);
}

static void test_strtoull_converts_string_to_number_for_base_16(void)
{
  const char *s;
  char *endptr;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 16), 0x1234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "12aFg";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 16), 0x12afULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "fA1299G";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 16), 0xfa1299ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "0X1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 16), 0x1234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "0x1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 16), 0x1234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 16), 0x0ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 1);
}

static void test_strtoull_converts_string_to_number_for_base_36(void)
{
  const char *s;
  char *endptr;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 36), 49360ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "123Z";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 36), 49391ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "123z";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 36), 49391ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 36), 0ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 1);
}

static void test_strtoull_converts_string_to_number_for_base_0(void)
{
  const char *s;
  char *endptr;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 0), 1234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 4);
  s = "01234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 0), 01234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
  s = "0X1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 0), 0x1234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "0x1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 0), 0x1234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 6);
  s = "0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 0), 0ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 1);
  s = "0x0";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 0), 0x0ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 3);
}

static void test_strtoull_converts_string_to_number_for_sign(void)
{
  const char *s;
  char *endptr;
  s = "+1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 10), 1234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
  s = "-1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 10), (unsigned long long) (-1234LL));
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 5);
}

static void test_strtoull_converts_string_to_number_for_spaces(void)
{
  const char *s;
  char *endptr;
  s = " \t 1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 10), 1234ULL);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 7);
  s = "\t\t-1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 10), (unsigned long long) (-1234LL));
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 7);
}

static void test_strtoull_returns_max_number_without_error(void)
{
  const char *s;
  char *endptr;
  s = "18446744073709551615";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 10), ULLONG_MAX);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 20);
  s = "ffffffffffffffff";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 16), ULLONG_MAX);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_EQUAL(endptr, s + 16);
}

static void test_strtoull_converts_string_to_number_without_endptr(void)
{
  const char *s;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, NULL, 10), 1234ULL);
  CU_ASSERT_EQUAL(errno, 0);
}

static void test_strtoull_complains_on_overflow(void)
{
  const char *s;
  char *endptr;
  s = "18446744073709551616";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 10), ULLONG_MAX);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 20);
  s = "10000000000000000";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 16), ULLONG_MAX);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 17);
  s = "1000000000000000000000000";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 10), ULLONG_MAX);
  CU_ASSERT_EQUAL(errno, ERANGE);
  CU_ASSERT_PTR_EQUAL(endptr, s + 25);
}

static void test_strtoull_complains_on_invalid_base(void)
{
  const char *s;
  char *endptr;
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 1), 0);
  CU_ASSERT_EQUAL(errno, EINVAL);
  s = "1234";
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_strtoull(s, &endptr, 37), 0);
  CU_ASSERT_EQUAL(errno, EINVAL);
}

int add_strtoll_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("strtoll suite", initialize_strtoll, cleanup_strtoll)) == NULL) return -1;
  if(CU_add_test(suite, "strtoll converts string to number for base 10", test_strtoll_converts_string_to_number_for_base_10) == NULL) return -1;
  if(CU_add_test(suite, "strtoll converts string to number for base 2", test_strtoll_converts_string_to_number_for_base_2) == NULL) return -1;
  if(CU_add_test(suite, "strtoll converts string to number for base 8", test_strtoll_converts_string_to_number_for_base_8) == NULL) return -1;
  if(CU_add_test(suite, "strtoll converts string to number for base 16", test_strtoll_converts_string_to_number_for_base_16) == NULL) return -1;
  if(CU_add_test(suite, "strtoll converts string to number for base 36", test_strtoll_converts_string_to_number_for_base_36) == NULL) return -1;
  if(CU_add_test(suite, "strtoll converts string to number for base 0", test_strtoll_converts_string_to_number_for_base_0) == NULL) return -1;
  if(CU_add_test(suite, "strtoll converts string to number for sign", test_strtoll_converts_string_to_number_for_sign) == NULL) return -1;
  if(CU_add_test(suite, "strtoll converts string to number for spaces", test_strtoll_converts_string_to_number_for_spaces) == NULL) return -1;
  if(CU_add_test(suite, "strtoll returns max number without error", test_strtoll_returns_max_number_without_error) == NULL) return -1;
  if(CU_add_test(suite, "strtoll returns min number without error", test_strtoll_returns_min_number_without_error) == NULL) return -1;
  if(CU_add_test(suite, "strtoll converts string to number without endptr", test_strtoll_converts_string_to_number_without_endptr) == NULL) return -1;
  if(CU_add_test(suite, "strtoll complains on overflow", test_strtoll_complains_on_overflow) == NULL) return -1;
  if(CU_add_test(suite, "strtoll complains on invalid base", test_strtoll_complains_on_invalid_base) == NULL) return -1;
  if(CU_add_test(suite, "strtoull converts string to number for base 10", test_strtoull_converts_string_to_number_for_base_10) == NULL) return -1;
  if(CU_add_test(suite, "strtoull converts string to number for base 2", test_strtoull_converts_string_to_number_for_base_2) == NULL) return -1;
  if(CU_add_test(suite, "strtoull converts string to number for base 8", test_strtoull_converts_string_to_number_for_base_8) == NULL) return -1;
  if(CU_add_test(suite, "strtoull converts string to number for base 16", test_strtoull_converts_string_to_number_for_base_16) == NULL) return -1;
  if(CU_add_test(suite, "strtoull converts string to number for base 36", test_strtoull_converts_string_to_number_for_base_36) == NULL) return -1;
  if(CU_add_test(suite, "strtoull converts string to number for base 0", test_strtoull_converts_string_to_number_for_base_0) == NULL) return -1;
  if(CU_add_test(suite, "strtoull converts string to number for sign", test_strtoull_converts_string_to_number_for_sign) == NULL) return -1;
  if(CU_add_test(suite, "strtoull converts string to number for spaces", test_strtoull_converts_string_to_number_for_spaces) == NULL) return -1;
  if(CU_add_test(suite, "strtoull returns max number without error", test_strtoull_returns_max_number_without_error) == NULL) return -1;
  if(CU_add_test(suite, "strtoull converts string to number without endptr", test_strtoull_converts_string_to_number_without_endptr) == NULL) return -1;
  if(CU_add_test(suite, "strtoull complains on overflow", test_strtoull_complains_on_overflow) == NULL) return -1;
  if(CU_add_test(suite, "strtoull complains on invalid base", test_strtoull_complains_on_invalid_base) == NULL) return -1;
  return 0;
}

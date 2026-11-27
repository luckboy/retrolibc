/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <CUnit/Basic.h>
#include "retrolibc.h"

static int initialize_fnmatch(void)
{ return 0; }

static int cleanup_fnmatch(void)
{ return 0; }

static void test_fnmatch_matches_string(void)
{
  CU_ASSERT_EQUAL(retrolibc_fnmatch("abcdef", "abcdef", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("abcdef", "Abcdef", 0), RETROLIBC_FNM_NOMATCH);
}

static void test_fnmatch_matches_string_for_star(void)
{
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "abc", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "defabc", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "abcdefabc", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "/abc", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", ".abc", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "abcdef", 0), RETROLIBC_FNM_NOMATCH);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "defabcdef", 0), RETROLIBC_FNM_NOMATCH);
}

static void test_fnmatch_matches_string_for_star_and_fnm_pathname(void)
{
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "abc", RETROLIBC_FNM_PATHNAME), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "defabc", RETROLIBC_FNM_PATHNAME), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "abcdefabc", RETROLIBC_FNM_PATHNAME), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "/abc", RETROLIBC_FNM_PATHNAME), RETROLIBC_FNM_NOMATCH);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", ".abc", RETROLIBC_FNM_PATHNAME), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "abcdef", RETROLIBC_FNM_PATHNAME), RETROLIBC_FNM_NOMATCH);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "defabcdef", RETROLIBC_FNM_PATHNAME), RETROLIBC_FNM_NOMATCH);
}

static void test_fnmatch_matches_string_for_star_and_fnm_period(void)
{
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "abc", RETROLIBC_FNM_PERIOD), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "defabc", RETROLIBC_FNM_PERIOD), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "abcdefabc", RETROLIBC_FNM_PERIOD), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "/abc", RETROLIBC_FNM_PERIOD), 0);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", ".abc", RETROLIBC_FNM_PERIOD), RETROLIBC_FNM_NOMATCH);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "abcdef", RETROLIBC_FNM_PERIOD), RETROLIBC_FNM_NOMATCH);
  CU_ASSERT_EQUAL(retrolibc_fnmatch("*abc", "defabcdef", RETROLIBC_FNM_PERIOD), RETROLIBC_FNM_NOMATCH);
}

static void test_fnmatch_does_not_match_string_without_basename(void)
{
  const char *pattern = "abc/def/ghi";
  CU_ASSERT_PTR_EQUAL(retrolibc_fnmatch(pattern, "abc/def", RETROLIBC_FNM_PATHNAME), RETROLIBC_FNM_NOMATCH);
}

int add_fnmatch_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("fnmatch suite", initialize_fnmatch, cleanup_fnmatch)) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch matches string", test_fnmatch_matches_string) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch matches string for *", test_fnmatch_matches_string_for_star) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch matches string for * and FNM_PATHNAME", test_fnmatch_matches_string_for_star_and_fnm_pathname) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch matches string for * and FNM_PERIOD", test_fnmatch_matches_string_for_star_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch doesn't match string without basename", test_fnmatch_does_not_match_string_without_basename) == NULL) return -1;
  return 0;
}

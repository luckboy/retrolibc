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

static int initialize_fnmatch_glob(void)
{ return 0; }

static int cleanup_fnmatch_glob(void)
{ return 0; }

static void test_fnmatch_glob_matches_string(void)
{
  const char *pattern = "abcdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "Abcdef", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_question(void)
{
  const char *pattern = "a?cdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aBcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "a1cdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "a/cdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "Abcdef", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_first_question(void)
{
  const char *pattern = "?bcdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "Abcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "1bcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "/bcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, ".bcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "aBcdef", 0, 1));
}


static void test_fnmatch_glob_matches_string_for_first_star(void)
{
  const char *pattern = "*abc";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "defabc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdefabc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "/abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, ".abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abcdef", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "defabcdef", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_middle_star(void)
{
  const char *pattern = "a*bc";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "adefbc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdefbc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "a/bc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abcdef", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "adefbcdef", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_last_star(void)
{
  const char *pattern = "abc*";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcabcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc/", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "defabc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "defabcdef", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_star_star(void)
{
  const char *pattern = "a**bc";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "adefbc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdefbc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "a/bc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abcdef", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "adefbcdef", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_brackets(void)
{
  const char *pattern = "a[abc]c";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aac", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "acc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "adc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "aBc", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_brackets_and_caret(void)
{
  const char *pattern = "a[^abc]c";
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "aac", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "acc", 0, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "adc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aBc", 0, 1), pattern + strlen(pattern));
}

static void test_fnmatch_glob_matches_string_for_brackets_and_exclamation(void)
{
  const char *pattern = "a[!abc]c";
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "aac", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "acc", 0, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "adc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aBc", 0, 1), pattern + strlen(pattern));
}

static void test_fnmatch_glob_matches_string_for_brackets_and_range(void)
{
  const char *pattern = "a[ab-de]c";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aac", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "acc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "adc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aec", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "afc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "aBc", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_brackets_and_range_and_first_escape(void)
{
  const char *pattern = "a[a\\b-de]c";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aac", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "acc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "adc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aec", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "afc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "aBc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "a\\c", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_brackets_and_range_and_second_escape(void)
{
  const char *pattern = "a[ab-\\de]c";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aac", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "acc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "adc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aec", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "afc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "aBc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "a\\c", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_brackets_and_range_and_2_escapes(void)
{
  const char *pattern = "a[a\\b-\\de]c";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aac", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "acc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "adc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aec", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "afc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "aBc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "a\\c", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_brackets_and_slash_and_dot(void)
{
  const char *pattern = "[a/.c]bc";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "/bc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, ".bc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "cbc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "dbc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "Abc", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_escapes(void)
{
  const char *pattern = "a\\*c\\?\\[f";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "a*c?[f", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abcdef", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_escapes_in_brackets(void)
{
  const char *pattern = "[\\^\\!\\]]";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "^", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "!", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "]", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "a", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "b", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_example_from_sus(void)
{
  const char *pattern = "[\\^!]";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "^", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "!", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "a", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "b", 0, 1));
}

static void test_fnmatch_glob_matches_string_for_question_and_fnm_pathname(void)
{
  const char *pattern = "?bcdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "Abcdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "1bcdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "/bcdef", RETROLIBC_FNM_PATHNAME, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, ".bcdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "aBcdef", RETROLIBC_FNM_PATHNAME, 1));
}

static void test_fnmatch_glob_matches_string_for_star_and_fnm_pathname(void)
{
  const char *pattern = "*abc";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "defabc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdefabc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "/abc", RETROLIBC_FNM_PATHNAME, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, ".abc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abcdef", RETROLIBC_FNM_PATHNAME, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "defabcdef", RETROLIBC_FNM_PATHNAME, 1));
}

static void test_fnmatch_glob_matches_string_for_brackets_and_slash_and_dot_and_fnm_pathname(void)
{
  const char *pattern = "[a/.c]bc";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "/bc", RETROLIBC_FNM_PATHNAME, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, ".bc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "cbc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "dbc", RETROLIBC_FNM_PATHNAME, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "Abc", RETROLIBC_FNM_PATHNAME, 1));
}

static void test_fnmatch_glob_matches_string_for_escapes_and_fnm_pathname(void)
{
  const char *pattern = "a\\*c\\?\\[f";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "a*c?[f", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abcdef", RETROLIBC_FNM_PATHNAME, 1));
}

static void test_fnmatch_glob_matches_string_for_escapes_in_brackets_and_fnm_pathname(void)
{
  const char *pattern = "[\\^\\!\\]]";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "^", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "!", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "]", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "a", RETROLIBC_FNM_PATHNAME, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "b", RETROLIBC_FNM_PATHNAME, 1));
}

static void test_fnmatch_glob_matches_string_for_slash_after_other_character_and_fnm_pathname(void)
{
  const char *pattern = "a?cdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aBcdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "a1cdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "a/cdef", RETROLIBC_FNM_PATHNAME, 1));
}

static void test_fnmatch_glob_matches_string_for_question_and_fnm_period(void)
{
  const char *pattern = "?bcdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "Abcdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "1bcdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "/bcdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, ".bcdef", RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "aBcdef", RETROLIBC_FNM_PERIOD, 1));
}

static void test_fnmatch_glob_matches_string_for_star_and_fnm_period(void)
{
  const char *pattern = "*abc";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "defabc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdefabc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "/abc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, ".abc", RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abcdef", RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "defabcdef", RETROLIBC_FNM_PERIOD, 1));
}

static void test_fnmatch_glob_matches_string_for_brackets_and_slash_and_dot_and_fnm_period(void)
{
  const char *pattern = "[a/.c]bc";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "/bc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, ".bc", RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "cbc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "dbc", RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "Abc", RETROLIBC_FNM_PERIOD, 1));
}

static void test_fnmatch_glob_matches_string_for_escapes_and_fnm_period(void)
{
  const char *pattern = "a\\*c\\?\\[f";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "a*c?[f", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abcdef", RETROLIBC_FNM_PERIOD, 1));
}

static void test_fnmatch_glob_matches_string_for_escapes_in_brackets_and_fnm_period(void)
{
  const char *pattern = "[\\^\\!\\]]";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "^", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "!", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "]", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "a", RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "b", RETROLIBC_FNM_PERIOD, 1));
}

static void test_fnmatch_glob_matches_string_for_dot_after_other_character_and_fnm_period(void)
{
  const char *pattern = "a?cdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "aBcdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "a1cdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "a.cdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
}

static void test_fnmatch_glob_matches_string_for_question_and_fnm_pathname_and_fnm_period(void)
{
  const char *pattern = "?bcdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "Abcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "1bcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "/bcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, ".bcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "aBcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
}

static void test_fnmatch_glob_matches_string_for_star_and_fnm_pathname_and_fnm_period(void)
{
  const char *pattern = "*abc";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "defabc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdefabc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "/abc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, ".abc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "defabcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
}

static void test_fnmatch_glob_matches_string_for_brackets_and_slash_and_dot_and_fnm_pathname_and_fnm_period(void)
{
  const char *pattern = "[a/.c]bc";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "/bc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, ".bc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "cbc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "dbc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "Abc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
}

static void test_fnmatch_glob_matches_string_for_escapes_and_fnm_pathname_and_fnm_period(void)
{
  const char *pattern = "a\\*c\\?\\[f";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "a*c?[f", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
}

static void test_fnmatch_glob_matches_string_for_escapes_in_brackets_and_fnm_pathname_and_fnm_period(void)
{
  const char *pattern = "[\\^\\!\\]]";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "^", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "!", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "]", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "a", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "b", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
}

static void test_fnmatch_glob_matches_string_for_dot_after_slash_and_fnm_pathname_and_fnm_period(void)
{
  const char *pattern = "abc/?ef";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc/def", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc/Def", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc/1ef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abc/.ef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
}

static void test_fnmatch_glob_matches_string_for_escapes_and_fnm_noescape(void)
{
  const char *pattern = "a\\*c\\?\\[f";
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "a*c?[f", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abcdef", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "a\\bc\\d\\f", RETROLIBC_FNM_NOESCAPE, 1), pattern + strlen(pattern));
}

static void test_fnmatch_glob_matches_string_for_escapes_in_brackets_and_fnm_noescape(void)
{
  const char *pattern = "[\\^\\!\\]]";
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "^", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "!", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "]", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "a", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "b", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "\\]", RETROLIBC_FNM_NOESCAPE, 1), pattern + strlen(pattern));
}

static void test_fnmatch_glob_returns_pointer_to_slash(void)
{
  const char *pattern = "abc/def/ghi";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc/def", RETROLIBC_FNM_PATHNAME, 1), strrchr(pattern, '/'));
}

static void test_fnmatch_glob_returns_pointer_to_slash_for_slash_escapes(void)
{
  const char *pattern = "abc\\/def\\/ghi";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abc/def", RETROLIBC_FNM_PATHNAME, 1), strrchr(pattern, '/') - 1);
}

static void test_fnmatch_glob_matches_string_for_question_and_fnm_period_and_is_first_char_that_is_0(void)
{
  const char *pattern = "?bcdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "abcdef", RETROLIBC_FNM_PERIOD, 0), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "Abcdef", RETROLIBC_FNM_PERIOD, 0), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "1bcdef", RETROLIBC_FNM_PERIOD, 0), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, "/bcdef", RETROLIBC_FNM_PERIOD, 0), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___fnmatch_glob(pattern, ".bcdef", RETROLIBC_FNM_PERIOD, 0), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "aBcdef", RETROLIBC_FNM_PERIOD, 0));
}

static void test_fnmatch_glob_matches_string_for_0_escape(void)
{
  const char *pattern = "\\";
  CU_ASSERT_PTR_NULL(retrolibc___fnmatch_glob(pattern, "abc", 0, 1));
}

int add_fnmatch_glob_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("fnmatch_glob suite", initialize_fnmatch_glob, cleanup_fnmatch_glob)) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string", test_fnmatch_glob_matches_string) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for ?", test_fnmatch_glob_matches_string_for_question) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for first ?", test_fnmatch_glob_matches_string_for_first_question) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for first *", test_fnmatch_glob_matches_string_for_first_star) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for middle *", test_fnmatch_glob_matches_string_for_middle_star) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for last *", test_fnmatch_glob_matches_string_for_last_star) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for **", test_fnmatch_glob_matches_string_for_star_star) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for brackets", test_fnmatch_glob_matches_string_for_brackets) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for brackets and ^", test_fnmatch_glob_matches_string_for_brackets_and_caret) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for brackets and !", test_fnmatch_glob_matches_string_for_brackets_and_exclamation) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for brackets and range", test_fnmatch_glob_matches_string_for_brackets_and_range) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for brackets and range and first escape", test_fnmatch_glob_matches_string_for_brackets_and_range_and_first_escape) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for brackets and range and second escape", test_fnmatch_glob_matches_string_for_brackets_and_range_and_second_escape) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for brackets and range and 2 escapes", test_fnmatch_glob_matches_string_for_brackets_and_range_and_2_escapes) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for brackets and / and .", test_fnmatch_glob_matches_string_for_brackets_and_slash_and_dot) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for escapes", test_fnmatch_glob_matches_string_for_escapes) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for escapes in brackets", test_fnmatch_glob_matches_string_for_escapes_in_brackets) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for example from SUS", test_fnmatch_glob_matches_string_for_example_from_sus) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for ? and FNM_PATHNAME", test_fnmatch_glob_matches_string_for_question_and_fnm_pathname) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for * and FNM_PATHNAME", test_fnmatch_glob_matches_string_for_star_and_fnm_pathname) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for brackets and / and . and FNM_PATHNAME", test_fnmatch_glob_matches_string_for_brackets_and_slash_and_dot_and_fnm_pathname) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for escapes and FNM_PATHNAME", test_fnmatch_glob_matches_string_for_escapes_and_fnm_pathname) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for escapes in brackets and FNM_PATHNAME", test_fnmatch_glob_matches_string_for_escapes_in_brackets_and_fnm_pathname) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for / after other character and FNM_PATHNAME", test_fnmatch_glob_matches_string_for_slash_after_other_character_and_fnm_pathname) == NULL) return -1; 
  if(CU_add_test(suite, "fnmatch_glob matches string for ? and FNM_PERIOD", test_fnmatch_glob_matches_string_for_question_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for * and FNM_PERIOD", test_fnmatch_glob_matches_string_for_star_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for brackets and / and . and FNM_PPERIOD", test_fnmatch_glob_matches_string_for_brackets_and_slash_and_dot_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for escapes and FNM_PATHNAME", test_fnmatch_glob_matches_string_for_escapes_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for escapes in brackets and FNM_PERIOD", test_fnmatch_glob_matches_string_for_escapes_in_brackets_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for . after other character and FNM_PERIOD", test_fnmatch_glob_matches_string_for_dot_after_other_character_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for ? and FNM_PATHNAME and FNM_PERIOD", test_fnmatch_glob_matches_string_for_question_and_fnm_pathname_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for * and FNM_PATHNAME and FNM_PERIOD", test_fnmatch_glob_matches_string_for_star_and_fnm_pathname_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for brackets and / and . and FNM_PATHNAME and FNM_PERIOD", test_fnmatch_glob_matches_string_for_brackets_and_slash_and_dot_and_fnm_pathname_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for escapes and FNM_PATHNAME and FNM_PERIOD", test_fnmatch_glob_matches_string_for_escapes_and_fnm_pathname_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for escapes in brackets and FNM_PATHNAME and FNM_PERIOD", test_fnmatch_glob_matches_string_for_escapes_in_brackets_and_fnm_pathname_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for . after / and FNM_PATHNAME and FNM_PERIOD", test_fnmatch_glob_matches_string_for_dot_after_slash_and_fnm_pathname_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for escapes and FNM_NOESCAPE", test_fnmatch_glob_matches_string_for_escapes_and_fnm_noescape) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for escapes in brackets and FNM_NOESCAPE", test_fnmatch_glob_matches_string_for_escapes_in_brackets_and_fnm_noescape) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob returns pointer to /", test_fnmatch_glob_returns_pointer_to_slash) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob returns pointer to / for / escapes", test_fnmatch_glob_returns_pointer_to_slash_for_slash_escapes) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch_glob matches string for ? and FNM_PERIOD and is_first_char that is 0", test_fnmatch_glob_matches_string_for_question_and_fnm_period_and_is_first_char_that_is_0) == NULL) return -1;
  if(CU_add_test(suite, "fnmatch glob matches string for 0 escape", test_fnmatch_glob_matches_string_for_0_escape) == NULL) return -1;
  return 0;
}

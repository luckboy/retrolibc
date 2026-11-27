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

static int initialize_glob_fnmatch(void)
{ return 0; }

static int cleanup_glob_fnmatch(void)
{ return 0; }

static void test_glob_fnmatch_matches_string(void)
{
  const char *pattern = "abcdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "Abcdef", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_question(void)
{
  const char *pattern = "a?cdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aBcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "a1cdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "a/cdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "Abcdef", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_first_question(void)
{
  const char *pattern = "?bcdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "Abcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "1bcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "/bcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, ".bcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "aBcdef", 0, 1));
}


static void test_glob_fnmatch_matches_string_for_first_star(void)
{
  const char *pattern = "*abc";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "defabc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdefabc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "/abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, ".abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abcdef", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "defabcdef", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_middle_star(void)
{
  const char *pattern = "a*bc";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "adefbc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdefbc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "a/bc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abcdef", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "adefbcdef", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_last_star(void)
{
  const char *pattern = "abc*";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcabcdef", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc/", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "defabc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "defabcdef", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_star_star(void)
{
  const char *pattern = "a**bc";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "adefbc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdefbc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "a/bc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abcdef", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "adefbcdef", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_brackets(void)
{
  const char *pattern = "a[abc]c";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aac", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "acc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "adc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "aBc", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_brackets_and_caret(void)
{
  const char *pattern = "a[^abc]c";
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "aac", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "acc", 0, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "adc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aBc", 0, 1), pattern + strlen(pattern));
}

static void test_glob_fnmatch_matches_string_for_brackets_and_exclamation(void)
{
  const char *pattern = "a[!abc]c";
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "aac", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "acc", 0, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "adc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aBc", 0, 1), pattern + strlen(pattern));
}

static void test_glob_fnmatch_matches_string_for_brackets_and_range(void)
{
  const char *pattern = "a[ab-de]c";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aac", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "acc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "adc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aec", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "afc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "aBc", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_brackets_and_range_and_first_escape(void)
{
  const char *pattern = "a[a\\b-de]c";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aac", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "acc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "adc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aec", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "afc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "aBc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "a\\c", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_brackets_and_range_and_second_escape(void)
{
  const char *pattern = "a[ab-\\de]c";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aac", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "acc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "adc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aec", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "afc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "aBc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "a\\c", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_brackets_and_range_and_2_escapes(void)
{
  const char *pattern = "a[a\\b-\\de]c";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aac", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "acc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "adc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aec", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "afc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "aBc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "a\\c", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_brackets_and_slash_and_dot(void)
{
  const char *pattern = "[a/.c]bc";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "/bc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, ".bc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "cbc", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "dbc", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "Abc", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_escapes(void)
{
  const char *pattern = "a\\*c\\?\\[f";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "a*c?[f", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abcdef", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_escapes_in_brackets(void)
{
  const char *pattern = "[\\^\\!\\]]";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "^", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "!", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "]", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "a", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "b", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_example_from_sus(void)
{
  const char *pattern = "[\\^!]";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "^", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "!", 0, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "a", 0, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "b", 0, 1));
}

static void test_glob_fnmatch_matches_string_for_question_and_fnm_pathname(void)
{
  const char *pattern = "?bcdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "Abcdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "1bcdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "/bcdef", RETROLIBC_FNM_PATHNAME, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, ".bcdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "aBcdef", RETROLIBC_FNM_PATHNAME, 1));
}

static void test_glob_fnmatch_matches_string_for_star_and_fnm_pathname(void)
{
  const char *pattern = "*abc";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "defabc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdefabc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "/abc", RETROLIBC_FNM_PATHNAME, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, ".abc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abcdef", RETROLIBC_FNM_PATHNAME, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "defabcdef", RETROLIBC_FNM_PATHNAME, 1));
}

static void test_glob_fnmatch_matches_string_for_brackets_and_slash_and_dot_and_fnm_pathname(void)
{
  const char *pattern = "[a/.c]bc";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "/bc", RETROLIBC_FNM_PATHNAME, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, ".bc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "cbc", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "dbc", RETROLIBC_FNM_PATHNAME, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "Abc", RETROLIBC_FNM_PATHNAME, 1));
}

static void test_glob_fnmatch_matches_string_for_escapes_and_fnm_pathname(void)
{
  const char *pattern = "a\\*c\\?\\[f";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "a*c?[f", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abcdef", RETROLIBC_FNM_PATHNAME, 1));
}

static void test_glob_fnmatch_matches_string_for_escapes_in_brackets_and_fnm_pathname(void)
{
  const char *pattern = "[\\^\\!\\]]";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "^", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "!", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "]", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "a", RETROLIBC_FNM_PATHNAME, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "b", RETROLIBC_FNM_PATHNAME, 1));
}

static void test_glob_fnmatch_matches_string_for_slash_after_other_character_and_fnm_pathname(void)
{
  const char *pattern = "a?cdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aBcdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "a1cdef", RETROLIBC_FNM_PATHNAME, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "a/cdef", RETROLIBC_FNM_PATHNAME, 1));
}

static void test_glob_fnmatch_matches_string_for_question_and_fnm_period(void)
{
  const char *pattern = "?bcdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "Abcdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "1bcdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "/bcdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, ".bcdef", RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "aBcdef", RETROLIBC_FNM_PERIOD, 1));
}

static void test_glob_fnmatch_matches_string_for_star_and_fnm_period(void)
{
  const char *pattern = "*abc";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "defabc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdefabc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "/abc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, ".abc", RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abcdef", RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "defabcdef", RETROLIBC_FNM_PERIOD, 1));
}

static void test_glob_fnmatch_matches_string_for_brackets_and_slash_and_dot_and_fnm_period(void)
{
  const char *pattern = "[a/.c]bc";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "/bc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, ".bc", RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "cbc", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "dbc", RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "Abc", RETROLIBC_FNM_PERIOD, 1));
}

static void test_glob_fnmatch_matches_string_for_escapes_and_fnm_period(void)
{
  const char *pattern = "a\\*c\\?\\[f";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "a*c?[f", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abcdef", RETROLIBC_FNM_PERIOD, 1));
}

static void test_glob_fnmatch_matches_string_for_escapes_in_brackets_and_fnm_period(void)
{
  const char *pattern = "[\\^\\!\\]]";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "^", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "!", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "]", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "a", RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "b", RETROLIBC_FNM_PERIOD, 1));
}

static void test_glob_fnmatch_matches_string_for_dot_after_other_character_and_fnm_period(void)
{
  const char *pattern = "a?cdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "aBcdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "a1cdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "a.cdef", RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
}

static void test_glob_fnmatch_matches_string_for_question_and_fnm_pathname_and_fnm_period(void)
{
  const char *pattern = "?bcdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "Abcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "1bcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "/bcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, ".bcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "aBcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
}

static void test_glob_fnmatch_matches_string_for_star_and_fnm_pathname_and_fnm_period(void)
{
  const char *pattern = "*abc";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "defabc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdefabc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "/abc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, ".abc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "defabcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
}

static void test_glob_fnmatch_matches_string_for_brackets_and_slash_and_dot_and_fnm_pathname_and_fnm_period(void)
{
  const char *pattern = "[a/.c]bc";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "/bc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, ".bc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "cbc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "dbc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "Abc", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
}

static void test_glob_fnmatch_matches_string_for_escapes_and_fnm_pathname_and_fnm_period(void)
{
  const char *pattern = "a\\*c\\?\\[f";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "a*c?[f", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abcdef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
}

static void test_glob_fnmatch_matches_string_for_escapes_in_brackets_and_fnm_pathname_and_fnm_period(void)
{
  const char *pattern = "[\\^\\!\\]]";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "^", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "!", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "]", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "a", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "b", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
}

static void test_glob_fnmatch_matches_string_for_dot_after_slash_and_fnm_pathname_and_fnm_period(void)
{
  const char *pattern = "abc/?ef";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc/def", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc/Def", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc/1ef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abc/.ef", RETROLIBC_FNM_PATHNAME | RETROLIBC_FNM_PERIOD, 1));
}

static void test_glob_fnmatch_matches_string_for_escapes_and_fnm_noescape(void)
{
  const char *pattern = "a\\*c\\?\\[f";
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "a*c?[f", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abcdef", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "a\\bc\\d\\f", RETROLIBC_FNM_NOESCAPE, 1), pattern + strlen(pattern));
}

static void test_glob_fnmatch_matches_string_for_escapes_in_brackets_and_fnm_noescape(void)
{
  const char *pattern = "[\\^\\!\\]]";
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "^", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "!", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "]", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "a", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "b", RETROLIBC_FNM_NOESCAPE, 1));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "\\]", RETROLIBC_FNM_NOESCAPE, 1), pattern + strlen(pattern));
}

static void test_glob_fnmatch_returns_pointer_to_slash(void)
{
  const char *pattern = "abc/def/ghi";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc/def", RETROLIBC_FNM_PATHNAME, 1), strrchr(pattern, '/'));
}

static void test_glob_fnmatch_returns_pointer_to_slash_for_slash_escapes(void)
{
  const char *pattern = "abc\\/def\\/ghi";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abc/def", RETROLIBC_FNM_PATHNAME, 1), strrchr(pattern, '/') - 1);
}

static void test_glob_fnmatch_matches_string_for_question_and_fnm_period_and_is_first_char_that_is_0(void)
{
  const char *pattern = "?bcdef";
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "abcdef", RETROLIBC_FNM_PERIOD, 0), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "Abcdef", RETROLIBC_FNM_PERIOD, 0), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "1bcdef", RETROLIBC_FNM_PERIOD, 0), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, "/bcdef", RETROLIBC_FNM_PERIOD, 0), pattern + strlen(pattern));
  CU_ASSERT_PTR_EQUAL(retrolibc___glob_fnmatch(pattern, ".bcdef", RETROLIBC_FNM_PERIOD, 0), pattern + strlen(pattern));
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "aBcdef", RETROLIBC_FNM_PERIOD, 0));
}

static void test_glob_fnmatch_matches_string_for_0_escape(void)
{
  const char *pattern = "\\";
  CU_ASSERT_PTR_NULL(retrolibc___glob_fnmatch(pattern, "abc", 0, 1));
}

int add_glob_fnmatch_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("glob_fnmatch suite", initialize_glob_fnmatch, cleanup_glob_fnmatch)) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string", test_glob_fnmatch_matches_string) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for ?", test_glob_fnmatch_matches_string_for_question) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for first ?", test_glob_fnmatch_matches_string_for_first_question) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for first *", test_glob_fnmatch_matches_string_for_first_star) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for middle *", test_glob_fnmatch_matches_string_for_middle_star) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for last *", test_glob_fnmatch_matches_string_for_last_star) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for **", test_glob_fnmatch_matches_string_for_star_star) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for brackets", test_glob_fnmatch_matches_string_for_brackets) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for brackets and ^", test_glob_fnmatch_matches_string_for_brackets_and_caret) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for brackets and !", test_glob_fnmatch_matches_string_for_brackets_and_exclamation) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for brackets and range", test_glob_fnmatch_matches_string_for_brackets_and_range) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for brackets and range and first escape", test_glob_fnmatch_matches_string_for_brackets_and_range_and_first_escape) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for brackets and range and second escape", test_glob_fnmatch_matches_string_for_brackets_and_range_and_second_escape) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for brackets and range and 2 escapes", test_glob_fnmatch_matches_string_for_brackets_and_range_and_2_escapes) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for brackets and / and .", test_glob_fnmatch_matches_string_for_brackets_and_slash_and_dot) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for escapes", test_glob_fnmatch_matches_string_for_escapes) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for escapes in brackets", test_glob_fnmatch_matches_string_for_escapes_in_brackets) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for example from SUS", test_glob_fnmatch_matches_string_for_example_from_sus) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for ? and FNM_PATHNAME", test_glob_fnmatch_matches_string_for_question_and_fnm_pathname) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for * and FNM_PATHNAME", test_glob_fnmatch_matches_string_for_star_and_fnm_pathname) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for brackets and / and . and FNM_PATHNAME", test_glob_fnmatch_matches_string_for_brackets_and_slash_and_dot_and_fnm_pathname) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for escapes and FNM_PATHNAME", test_glob_fnmatch_matches_string_for_escapes_and_fnm_pathname) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for escapes in brackets and FNM_PATHNAME", test_glob_fnmatch_matches_string_for_escapes_in_brackets_and_fnm_pathname) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for / after other character and FNM_PATHNAME", test_glob_fnmatch_matches_string_for_slash_after_other_character_and_fnm_pathname) == NULL) return -1; 
  if(CU_add_test(suite, "glob_fnmatch matches string for ? and FNM_PERIOD", test_glob_fnmatch_matches_string_for_question_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for * and FNM_PERIOD", test_glob_fnmatch_matches_string_for_star_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for brackets and / and . and FNM_PPERIOD", test_glob_fnmatch_matches_string_for_brackets_and_slash_and_dot_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for escapes and FNM_PATHNAME", test_glob_fnmatch_matches_string_for_escapes_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for escapes in brackets and FNM_PERIOD", test_glob_fnmatch_matches_string_for_escapes_in_brackets_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for . after other character and FNM_PERIOD", test_glob_fnmatch_matches_string_for_dot_after_other_character_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for ? and FNM_PATHNAME and FNM_PERIOD", test_glob_fnmatch_matches_string_for_question_and_fnm_pathname_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for * and FNM_PATHNAME and FNM_PERIOD", test_glob_fnmatch_matches_string_for_star_and_fnm_pathname_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for brackets and / and . and FNM_PATHNAME and FNM_PERIOD", test_glob_fnmatch_matches_string_for_brackets_and_slash_and_dot_and_fnm_pathname_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for escapes and FNM_PATHNAME and FNM_PERIOD", test_glob_fnmatch_matches_string_for_escapes_and_fnm_pathname_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for escapes in brackets and FNM_PATHNAME and FNM_PERIOD", test_glob_fnmatch_matches_string_for_escapes_in_brackets_and_fnm_pathname_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for . after / and FNM_PATHNAME and FNM_PERIOD", test_glob_fnmatch_matches_string_for_dot_after_slash_and_fnm_pathname_and_fnm_period) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for escapes and FNM_NOESCAPE", test_glob_fnmatch_matches_string_for_escapes_and_fnm_noescape) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for escapes in brackets and FNM_NOESCAPE", test_glob_fnmatch_matches_string_for_escapes_in_brackets_and_fnm_noescape) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch returns pointer to /", test_glob_fnmatch_returns_pointer_to_slash) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch returns pointer to / for / escapes", test_glob_fnmatch_returns_pointer_to_slash_for_slash_escapes) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for ? and FNM_PERIOD and is_first_char that is 0", test_glob_fnmatch_matches_string_for_question_and_fnm_period_and_is_first_char_that_is_0) == NULL) return -1;
  if(CU_add_test(suite, "glob_fnmatch matches string for 0 escape", test_glob_fnmatch_matches_string_for_0_escape) == NULL) return -1;
  return 0;
}

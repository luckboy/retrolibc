/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"

static int initialize_string(void)
{ return 0; }

static int cleanup_string(void)
{ return 0; }

static void test_memccpy_copies_memory_and_returns_null(void)
{
  static char buf[11] = "XXXXXXXXXX";
  CU_ASSERT_PTR_NULL(retrolibc_memccpy(buf, "abc\0def\0", 'X', 8));
  CU_ASSERT_EQUAL(memcmp(buf, "abc\0def\0XX", 10), 0);
}

static void test_memccpy_copies_memory_and_returns_pointer(void)
{
  static char buf[11] = "XXXXXXXXXX";
  CU_ASSERT_PTR_EQUAL(retrolibc_memccpy(buf, "abc\0def\0", 'd', 8), (void *) (buf + 4));
  CU_ASSERT_EQUAL(memcmp(buf, "abc\0dXXXXXX", 10), 0);
}

static void test_memchr_returns_pointer_to_occurrence(void)
{
  char *s = "abc\0def\0";
  char *s2 = "abc\0abc\0";
  CU_ASSERT_PTR_EQUAL(retrolibc_memchr(s, 'e', 8), (void *) (s + 5));
  CU_ASSERT_PTR_EQUAL(retrolibc_memchr(s2, 'b', 8), (void *) (s2 + 1));
}

static void test_memchr_returns_null(void)
{
  char *s = "abc\0def\0";
  CU_ASSERT_PTR_NULL(retrolibc_memchr(s, 'X', 8));
}

static void test_memcmp_returns_zero(void)
{ CU_ASSERT_EQUAL(retrolibc_memcmp("abc\0def\0", "abc\0def\0", 8), 0); }

static void test_memcmp_returns_number_that_is_less_than_zero(void)
{ CU_ASSERT_EQUAL(retrolibc_memcmp("abc\0Def\0", "abc\0def\0", 8), 'D' - 'd'); }

static void test_memcmp_returns_number_that_is_greater_than_zero(void)
{ CU_ASSERT_EQUAL(retrolibc_memcmp("abc\0def\0", "abc\0dEf\0", 8), 'e' - 'E'); }

static void test_memcpy_copies_memory(void)
{
  static char buf[11] = "XXXXXXXXXX";
  CU_ASSERT_PTR_EQUAL(retrolibc_memcpy(buf, "abc\0def\0", 8), (void *) buf);
  CU_ASSERT_EQUAL(memcmp(buf, "abc\0def\0XX", 10), 0);
}

static void test_memmove_moves_memory(void)
{
  static char buf[11] = "XXXXXXXXXX";
  CU_ASSERT_PTR_EQUAL(retrolibc_memmove(buf, "abc\0def\0", 8), (void *) buf);
  CU_ASSERT_EQUAL(memcmp(buf, "abc\0def\0XX", 10), 0);
}

static void test_memmove_moves_memory_with_overflap(void)
{
  static char buf[13] = "abc\0def\0XXXX";
  CU_ASSERT_PTR_EQUAL(retrolibc_memmove(buf + 4, buf, 8), (void *) (buf + 4));
  CU_ASSERT_EQUAL(memcmp(buf, "abc\0abc\0def\0", 12), 0);
}

static void test_memset_fills_memory_with_character(void)
{
  static char buf[11] = "XXXXXXXXXX";
  CU_ASSERT_PTR_EQUAL(retrolibc_memset(buf, 'Y', 8), (void *) buf);
  CU_ASSERT_EQUAL(memcmp(buf, "YYYYYYYYXX", 10), 0);
}

static void test_strcat_concatenates_two_strings(void)
{
  static char buf[11] = "abcdef\0XXX";
  CU_ASSERT_PTR_EQUAL(retrolibc_strcat(buf, "ghi"), buf);
  CU_ASSERT_STRING_EQUAL(buf, "abcdefghi");  
}

static void test_strchr_returns_pointer_to_occurrence(void)
{
  char *s = "abcdef";
  char *s2 = "abcabc";
  CU_ASSERT_PTR_EQUAL(retrolibc_strchr(s, 'd'), s + 3);
  CU_ASSERT_PTR_EQUAL(retrolibc_strchr(s2, 'c'), s2 + 2);
  CU_ASSERT_PTR_EQUAL(retrolibc_strchr(s, '\0'), s + 6);
}

static void test_strchr_returns_null(void)
{
  char *s = "abcdef";
  CU_ASSERT_PTR_NULL(retrolibc_strchr(s, 'X'));
}

static void test_strcmp_returns_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strcmp("abcdef", "abcdef"), 0);
  CU_ASSERT_EQUAL(retrolibc_strcmp("abcdef\0Ghi", "abcdef\0ghi"), 0);
}

static void test_strcmp_returns_number_that_is_less_than_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strcmp("abcDef", "abcdef"), 'D' - 'd');
  CU_ASSERT_EQUAL(retrolibc_strcmp("abcdef", "abc"), 'd');
}

static void test_strcmp_returns_number_that_is_greater_than_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strcmp("abcdef", "abcdEf"), 'e' - 'E');
  CU_ASSERT_EQUAL(retrolibc_strcmp("abc", "abcdef"), -'d');
}

static void test_strcoll_returns_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strcoll("abcdef", "abcdef"), 0);
  CU_ASSERT_EQUAL(retrolibc_strcoll("abcdef\0Ghi", "abcdef\0ghi"), 0);
}

static void test_strcoll_returns_number_that_is_less_than_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strcoll("abcDef", "abcdef"), 'D' - 'd');
  CU_ASSERT_EQUAL(retrolibc_strcoll("abcdef", "abc"), 'd');
}

static void test_strcoll_returns_number_that_is_greater_than_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strcoll("abcdef", "abcdEf"), 'e' - 'E');
  CU_ASSERT_EQUAL(retrolibc_strcoll("abc", "abcdef"), -'d');
}

static void test_strcpy_copies_string(void)
{
  static char buf[11] = "XXXXXXXXXX";
  CU_ASSERT_PTR_EQUAL(retrolibc_strcpy(buf, "abcdef"), buf);
  CU_ASSERT_EQUAL(memcmp(buf, "abcdef\0XXX", 10), 0);
}

static void test_strcspn_returns_length_of_complementary_substring(void)
{
  CU_ASSERT_EQUAL(retrolibc_strcspn("abcdef", "def"), 3);
  CU_ASSERT_EQUAL(retrolibc_strcspn("abcdef", "gfe"), 4);
  CU_ASSERT_EQUAL(retrolibc_strcspn("abcdef", "ghi"), 6);
}

static void test_strncat_returns_lenght_of_string(void)
{
  CU_ASSERT_EQUAL(retrolibc_strlen("abcdef"), 6);
  CU_ASSERT_EQUAL(retrolibc_strlen("abc"), 3);
  CU_ASSERT_EQUAL(retrolibc_strlen(""), 0);
}

static void test_strncat_concatenates_two_strings(void)
{
  static char buf[11] = "abcdef\0XXX";
  CU_ASSERT_PTR_EQUAL(retrolibc_strncat(buf, "ghi", 5), buf);
  CU_ASSERT_STRING_EQUAL(buf, "abcdefghi");
}

static void test_strncat_concatenates_two_strings_for_too_large_string(void)
{
  static char buf[11] = "abcdef\0XXX";
  CU_ASSERT_PTR_EQUAL(retrolibc_strncat(buf, "ghijkl", 3), buf);
  CU_ASSERT_STRING_EQUAL(buf, "abcdefghi");
}

static void test_strncmp_returns_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strncmp("abcdef", "abcdef", 8), 0);
  CU_ASSERT_EQUAL(retrolibc_strncmp("abcdef\0Ghi", "abcdef\0ghi", 8), 0);
  CU_ASSERT_EQUAL(retrolibc_strncmp("abcDef", "abcdef", 3), 0);
  CU_ASSERT_EQUAL(retrolibc_strncmp("abcdef", "abcdef", 0), 0);
}

static void test_strncmp_returns_number_that_is_less_than_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strncmp("abcDef", "abcdef", 8), 'D' - 'd');
  CU_ASSERT_EQUAL(retrolibc_strncmp("abcdef", "abc", 8), 'd');
  CU_ASSERT_EQUAL(retrolibc_strncmp("abcDef", "abcdef", 4), 'D' - 'd');
  CU_ASSERT_EQUAL(retrolibc_strncmp("abcdef", "abc", 4), 'd');
}

static void test_strncmp_returns_number_that_is_greater_than_zero(void)
{
  CU_ASSERT_EQUAL(retrolibc_strncmp("abcdef", "abcdEf", 8), 'e' - 'E');
  CU_ASSERT_EQUAL(retrolibc_strncmp("abc", "abcdef", 8), -'d');
  CU_ASSERT_EQUAL(retrolibc_strncmp("abcdef", "abcdEf", 5), 'e' - 'E');
  CU_ASSERT_EQUAL(retrolibc_strncmp("abc", "abcdef", 5), -'d');
}

static void test_strncpy_copies_string(void)
{
  static char buf[11] = "XXXXXXXXXX";
  CU_ASSERT_PTR_EQUAL(retrolibc_strncpy(buf, "abcdef", 10), buf);
  CU_ASSERT_EQUAL(memcmp(buf, "abcdef\0\0\0\0", 10), 0);
}

static void test_strncpy_copies_string_that_is_too_large(void)
{
  static char buf[11] = "XXXXXXXXXX";
  CU_ASSERT_PTR_EQUAL(retrolibc_strncpy(buf, "abcdef", 3), buf);
  CU_ASSERT_EQUAL(memcmp(buf, "abcXXXXXXX", 10), 0);
}

static void test_strpbrk_returns_pointer_to_occurrence(void)
{
  char *s = "abcdef";
  CU_ASSERT_EQUAL(retrolibc_strpbrk(s, "def"), s + 3);
  CU_ASSERT_EQUAL(retrolibc_strpbrk(s, "gfe"), s + 4);
}

static void test_strpbrk_returns_null(void)
{ CU_ASSERT_PTR_NULL(retrolibc_strpbrk("abcdef", "ghi")); }

static void test_strrchr_returns_pointer_to_occurrence(void)
{
  char *s = "abcdef";
  char *s2 = "abcabc";
  CU_ASSERT_PTR_EQUAL(retrolibc_strrchr(s, 'd'), s + 3);
  CU_ASSERT_PTR_EQUAL(retrolibc_strrchr(s2, 'b'), s2 + 4);
  CU_ASSERT_PTR_EQUAL(retrolibc_strrchr(s, '\0'), s + 6);
}

static void test_strrchr_returns_null(void)
{
  char *s = "abcdef";
  CU_ASSERT_PTR_NULL(retrolibc_strrchr(s, 'X'));
}

static void test_strspn_returns_length_of_substring(void)
{
  CU_ASSERT_EQUAL(retrolibc_strspn("abcdef", "abc"), 3);
  CU_ASSERT_EQUAL(retrolibc_strspn("abcdef", "def"), 0);
  CU_ASSERT_EQUAL(retrolibc_strspn("abcdef", "ghi"), 0);
}

static void test_strstr_returns_pointer_to_occurrence(void)
{
  char *s = "abcdefghidef";
  CU_ASSERT_PTR_EQUAL(retrolibc_strstr(s, "def"), s + 3);
  CU_ASSERT_PTR_EQUAL(retrolibc_strstr(s, "ghi"), s + 6);
}

static void test_strstr_returns_null(void)
{ CU_ASSERT_PTR_NULL(retrolibc_strstr("abcdefghidef", "jkl")); }

static void test_strtok_splits_string_to_tokens(void)
{
  char buf[20] = "  abc\t\tdef \t ghi\n\n";
  CU_ASSERT_PTR_EQUAL(retrolibc_strtok(buf, " \t\n"), buf + 2);
  CU_ASSERT_PTR_EQUAL(retrolibc_strtok(NULL, " \t\n"), buf + 7);
  CU_ASSERT_PTR_EQUAL(retrolibc_strtok(NULL, " \t\n"), buf + 13);
  CU_ASSERT_PTR_NULL(retrolibc_strtok(NULL, " \t\n"));
  CU_ASSERT_EQUAL(memcmp(buf, "  abc\0\tdef\0\t ghi\0\n", 10), 0);
}

static void test_strtok_r_splits_string_to_tokens(void)
{
  char *lasts;
  char buf[20] = "  abc\t\tdef \t ghi\n\n";
  CU_ASSERT_PTR_EQUAL(retrolibc_strtok_r(buf, " \t\n", &lasts), buf + 2);
  CU_ASSERT_PTR_EQUAL(retrolibc_strtok_r(NULL, " \t\n", &lasts), buf + 7);
  CU_ASSERT_PTR_EQUAL(retrolibc_strtok_r(NULL, " \t\n", &lasts), buf + 13);
  CU_ASSERT_PTR_NULL(retrolibc_strtok_r(NULL, " \t\n", &lasts));
  CU_ASSERT_EQUAL(memcmp(buf, "  abc\0\tdef\0\t ghi\0\n", 10), 0);
}

static void test_strxfrm_copies_string(void)
{
  static char buf[11] = "XXXXXXXXXX";
  CU_ASSERT_EQUAL(retrolibc_strxfrm(buf, "abcdef", 10), 6);
  CU_ASSERT_EQUAL(memcmp(buf, "abcdef\0\0\0\0", 10), 0);
}

static void test_strxfrm_copies_string_that_is_too_large(void)
{
  static char buf[11] = "XXXXXXXXXX";
  CU_ASSERT_EQUAL(retrolibc_strxfrm(buf, "abcdef", 3), 3);
  CU_ASSERT_EQUAL(memcmp(buf, "abcXXXXXXX", 10), 0);
}

int add_string_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("string suite", initialize_string, cleanup_string)) == NULL) return -1;
  if(CU_add_test(suite, "memccpy copies memory and returns null", test_memccpy_copies_memory_and_returns_null) == NULL) return -1;
  if(CU_add_test(suite, "memccpy copies memory and returns pointer", test_memccpy_copies_memory_and_returns_pointer) == NULL) return -1;
  if(CU_add_test(suite, "memchr returns pointer to occurrence", test_memchr_returns_pointer_to_occurrence) == NULL) return -1;
  if(CU_add_test(suite, "memchr returns null", test_memchr_returns_null) == NULL) return -1;
  if(CU_add_test(suite, "memcmp returns zero", test_memcmp_returns_zero) == NULL) return -1;
  if(CU_add_test(suite, "memcmp returns number that is less than zero", test_memcmp_returns_number_that_is_less_than_zero) == NULL) return -1;
  if(CU_add_test(suite, "memcmp returns number that is greater than zero", test_memcmp_returns_number_that_is_greater_than_zero) == NULL) return -1;
  if(CU_add_test(suite, "memcpy copies memory", test_memcpy_copies_memory) == NULL) return -1;
  if(CU_add_test(suite, "memmove moves memory", test_memmove_moves_memory) == NULL) return -1;
  if(CU_add_test(suite, "memmove moves memory with overflap", test_memmove_moves_memory_with_overflap) == NULL) return -1;  
  if(CU_add_test(suite, "memset fills memory with character", test_memset_fills_memory_with_character) == NULL) return -1;
  if(CU_add_test(suite, "strcat concatenates two strings",  test_strcat_concatenates_two_strings) == NULL) return -1;
  if(CU_add_test(suite, "strchr returns pointer to occurrence", test_strchr_returns_pointer_to_occurrence) == NULL) return -1;
  if(CU_add_test(suite, "strchr returns null", test_strchr_returns_null) == NULL) return -1;
  if(CU_add_test(suite, "strcmp returns zero", test_strcmp_returns_zero) == NULL) return -1;
  if(CU_add_test(suite, "strcmp returns number that is less than zero", test_strcmp_returns_number_that_is_less_than_zero) == NULL) return -1;
  if(CU_add_test(suite, "strcmp returns number that is greater than zero", test_strcmp_returns_number_that_is_greater_than_zero) == NULL) return -1;
  if(CU_add_test(suite, "strcoll returns zero", test_strcoll_returns_zero) == NULL) return -1;
  if(CU_add_test(suite, "strcoll returns number that is less than zero", test_strcoll_returns_number_that_is_less_than_zero) == NULL) return -1;
  if(CU_add_test(suite, "strcoll returns number that is greater than zero", test_strcoll_returns_number_that_is_greater_than_zero) == NULL) return -1;
  if(CU_add_test(suite, "strcpy copies string", test_strcpy_copies_string) == NULL) return -1;
  if(CU_add_test(suite, "strcspn returns length of complementary substring", test_strcspn_returns_length_of_complementary_substring) == NULL) return -1;
  if(CU_add_test(suite, "strncat returns lenght of string", test_strncat_returns_lenght_of_string) == NULL) return -1;
  if(CU_add_test(suite, "strncat concatenates two strings",  test_strncat_concatenates_two_strings) == NULL) return -1;
  if(CU_add_test(suite, "strncat concatenates two strings for too large string",  test_strncat_concatenates_two_strings_for_too_large_string) == NULL) return -1;
  if(CU_add_test(suite, "strncmp returns zero", test_strncmp_returns_zero) == NULL) return -1;
  if(CU_add_test(suite, "strncmp returns number that is less than zero", test_strncmp_returns_number_that_is_less_than_zero) == NULL) return -1;
  if(CU_add_test(suite, "strncmp returns number that is greater than zero", test_strncmp_returns_number_that_is_greater_than_zero) == NULL) return -1;
  if(CU_add_test(suite, "strncpy copies string", test_strncpy_copies_string) == NULL) return -1;
  if(CU_add_test(suite, "strncpy copies string that is too large", test_strncpy_copies_string_that_is_too_large) == NULL) return -1;
  if(CU_add_test(suite, "strpbrk returns pointer to occurrence", test_strpbrk_returns_pointer_to_occurrence) == NULL) return -1;
  if(CU_add_test(suite, "strpbrk returns null", test_strpbrk_returns_null) == NULL) return -1;
  if(CU_add_test(suite, "strrchr returns pointer to occurrence", test_strrchr_returns_pointer_to_occurrence) == NULL) return -1;
  if(CU_add_test(suite, "strrchr returns null", test_strrchr_returns_null) == NULL) return -1;
  if(CU_add_test(suite, "strcspn returns length of substring", test_strspn_returns_length_of_substring) == NULL) return -1;
  if(CU_add_test(suite, "strstr returns pointer to occurrence", test_strstr_returns_pointer_to_occurrence) == NULL) return -1;
  if(CU_add_test(suite, "strstr returns null", test_strstr_returns_null) == NULL) return -1;
  if(CU_add_test(suite, "strtok splits string to tokens", test_strtok_splits_string_to_tokens) == NULL) return -1;
  if(CU_add_test(suite, "strtok_r splits string to tokens", test_strtok_r_splits_string_to_tokens) == NULL) return -1;
  if(CU_add_test(suite, "strxfrm copies string", test_strxfrm_copies_string) == NULL) return -1;
  if(CU_add_test(suite, "strxfrm copies string that is too large", test_strxfrm_copies_string_that_is_too_large) == NULL) return -1;
  return 0;
}

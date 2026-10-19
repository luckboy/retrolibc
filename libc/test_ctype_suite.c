/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <ctype.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"

static int initialize_ctype(void)
{ return 0; }

static int cleanup_ctype(void)
{ return 0; }

static void test_isalnum_checks_character(void)
{
  static char buf[128];
  static char expected_buf[128];
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isalnum(c) ? '1' : '0');
  for(c = 0; c < 128; c++) buf[c] = (retrolibc_isalnum(c) ? '1' : '0');
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
}

static void test_isalpha_checks_character(void)
{
  static char buf[128];
  static char expected_buf[128];
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isalpha(c) ? '1' : '0');
  for(c = 0; c < 128; c++) buf[c] = (retrolibc_isalpha(c) ? '1' : '0');
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
}

static void test_isascii_checks_character(void)
{
  static char buf[256];
  static char expected_buf[256];
  int c;
  for(c = 0; c < 256; c++) expected_buf[c] = (isascii(c) ? '1' : '0');
  for(c = 0; c < 256; c++) buf[c] = (retrolibc_isascii(c) ? '1' : '0');
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 256), 0);
}

static void test_isdigit_checks_character(void)
{
  static char buf[128];
  static char expected_buf[128];
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isdigit(c) ? '1' : '0');
  for(c = 0; c < 128; c++) buf[c] = (retrolibc_isdigit(c) ? '1' : '0');
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
}

static void test_iscntrl_checks_character(void)
{
  static char buf[128];
  static char expected_buf[128];
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (iscntrl(c) ? '1' : '0');
  for(c = 0; c < 128; c++) buf[c] = (retrolibc_iscntrl(c) ? '1' : '0');
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
}

static void test_isgraph_checks_character(void)
{
  static char buf[128];
  static char expected_buf[128];
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isgraph(c) ? '1' : '0');
  for(c = 0; c < 128; c++) buf[c] = (retrolibc_isgraph(c) ? '1' : '0');
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
}

static void test_islower_checks_character(void)
{
  static char buf[128];
  static char expected_buf[128];
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (islower(c) ? '1' : '0');
  for(c = 0; c < 128; c++) buf[c] = (retrolibc_islower(c) ? '1' : '0');
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
}

static void test_isprint_checks_character(void)
{
  static char buf[128];
  static char expected_buf[128];
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isprint(c) ? '1' : '0');
  for(c = 0; c < 128; c++) buf[c] = (retrolibc_isprint(c) ? '1' : '0');
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
}

static void test_ispunct_checks_character(void)
{
  static char buf[128];
  static char expected_buf[128];
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (ispunct(c) ? '1' : '0');
  for(c = 0; c < 128; c++) buf[c] = (retrolibc_ispunct(c) ? '1' : '0');
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
}

static void test_isspace_checks_character(void)
{
  static char buf[128];
  static char expected_buf[128];
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isspace(c) ? '1' : '0');
  for(c = 0; c < 128; c++) buf[c] = (retrolibc_isspace(c) ? '1' : '0');
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
}

static void test_isupper_checks_character(void)
{
  static char buf[128];
  static char expected_buf[128];
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isupper(c) ? '1' : '0');
  for(c = 0; c < 128; c++) buf[c] = (retrolibc_isupper(c) ? '1' : '0');
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
}

static void test_isxdigit_checks_character(void)
{
  static char buf[128];
  static char expected_buf[128];
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isxdigit(c) ? '1' : '0');
  for(c = 0; c < 128; c++) buf[c] = (retrolibc_isxdigit(c) ? '1' : '0');
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
}

static void test_toascii_converts_character(void)
{
  static char buf[256];
  static char expected_buf[256];
  int c;
  for(c = 0; c < 256; c++) expected_buf[c] = toascii(c);
  for(c = 0; c < 256; c++) buf[c] = retrolibc_toascii(c);
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 256), 0);
}

static void test_tolower_converts_character(void)
{
  static char buf[128];
  static char expected_buf[128];
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = tolower(c);
  for(c = 0; c < 128; c++) buf[c] = retrolibc_tolower(c);
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
}

static void test_toupper_converts_character(void)
{
  static char buf[128];
  static char expected_buf[128];
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = tolower(c);
  for(c = 0; c < 128; c++) buf[c] = retrolibc_tolower(c);
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
}

int add_ctype_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("ctype suite", initialize_ctype, cleanup_ctype)) == NULL) return -1;
  if(CU_add_test(suite, "isalnum checks character", test_isalnum_checks_character) == NULL) return -1;
  if(CU_add_test(suite, "isalpha checks character", test_isalpha_checks_character) == NULL) return -1;
  if(CU_add_test(suite, "isascii checks character", test_isascii_checks_character) == NULL) return -1;
  if(CU_add_test(suite, "iscntrl checks character", test_iscntrl_checks_character) == NULL) return -1;
  if(CU_add_test(suite, "isdigit checks character", test_isdigit_checks_character) == NULL) return -1;
  if(CU_add_test(suite, "isgraph checks character", test_isgraph_checks_character) == NULL) return -1;
  if(CU_add_test(suite, "islower checks character", test_islower_checks_character) == NULL) return -1;
  if(CU_add_test(suite, "isprint checks character", test_isprint_checks_character) == NULL) return -1;
  if(CU_add_test(suite, "ispunct checks character", test_ispunct_checks_character) == NULL) return -1;
  if(CU_add_test(suite, "isspace checks character", test_isspace_checks_character) == NULL) return -1;
  if(CU_add_test(suite, "isupper checks character", test_isupper_checks_character) == NULL) return -1;
  if(CU_add_test(suite, "isxdigit checks character", test_isxdigit_checks_character) == NULL) return -1;
  if(CU_add_test(suite, "toascii converts character", test_toascii_converts_character) == NULL) return -1;
  if(CU_add_test(suite, "tolower converts character", test_tolower_converts_character) == NULL) return -1;
  if(CU_add_test(suite, "toupper converts character", test_toupper_converts_character) == NULL) return -1;
  return 0;
}

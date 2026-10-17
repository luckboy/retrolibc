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

unsigned test_seed(void);

static int initialize_bsearch(void)
{
  srand(test_seed());
  return 0;
}

static int cleanup_bsearch(void)
{ return 0; }

static int cmp(const void *s, const void *s2)
{ return *((const int *) s) - *((const int *) s2); }

static void test_bsearch_returns_pointer_to_element(void)
{
  static int array[1000];
  size_t i, j;
  int key, x;
  j = rand() / (RAND_MAX * 1.0) * 999.0;
  x = rand() / (RAND_MAX * 1.0) * 1000.0;
  for(i = 0; i < 1000; i++) {
    array[i] = x;
    x += rand() / (RAND_MAX * 1.0) * 999.0 + 1;
  }
  key = array[j];
  CU_ASSERT_PTR_EQUAL(retrolibc_bsearch(&key, array, 1000, sizeof(int), cmp), (void *) (array + j));
  j = rand() / (RAND_MAX * 1.0) * 999.0;
  x = rand() / (RAND_MAX * 1.0) * 1000.0;
  for(i = 0; i < 1000; i++) {
    array[i] = x;
    x += rand() / (RAND_MAX * 1.0) * 999.0 + 1;
  }
  key = array[j];
  CU_ASSERT_PTR_EQUAL(retrolibc_bsearch(&key, array, 1000, sizeof(int), cmp), (void *) (array + j));
}

static void test_bsearch_returns_null(void)
{
  static int array[1000];
  size_t i, j;
  int key, x;
  j = rand() / (RAND_MAX * 1.0) * 1000.0;
  x = rand() / (RAND_MAX * 1.0) * 1000.0;
  for(i = 0; i < 1000; i++) {
    if(j == i) {
      key = x;
      x += rand() / (RAND_MAX * 1.0) * 999.0 + 1;
    }
    array[i] = x;
    x += rand() / (RAND_MAX * 1.0) * 999.0 + 1;
  }
  if(j == 1000) key = x;
  CU_ASSERT_PTR_NULL(retrolibc_bsearch(&key, array, 1000, sizeof(int), cmp));
  j = rand() / (RAND_MAX * 1.0) * 1000.0;
  x = rand() / (RAND_MAX * 1.0) * 1000.0;
  for(i = 0; i < 1000; i++) {
    if(j == i) {
      key = x;
      x += rand() / (RAND_MAX * 1.0) * 999.0 + 1;
    }
    array[i] = x;
    x += rand() / (RAND_MAX * 1.0) * 999.0 + 1;
  }
  if(j == 1000) key = x;
  CU_ASSERT_PTR_NULL(retrolibc_bsearch(&key, array, 1000, sizeof(int), cmp));
}

static void test_bsearch_returns_pointer_to_element_for_array_with_two_elements(void)
{
  static int array[2] = { 1, 2 };
  int key = 1;
  CU_ASSERT_PTR_EQUAL(retrolibc_bsearch(&key, array, 2, sizeof(int), cmp), (void *) (array + 0));
  key = 2;
  CU_ASSERT_PTR_EQUAL(retrolibc_bsearch(&key, array, 2, sizeof(int), cmp), (void *) (array + 1));
}

static void test_bsearch_returns_null_for_array_with_two_elements(void)
{
  static int array[2] = { 1, 3 };
  int key = 0;
  CU_ASSERT_PTR_NULL(retrolibc_bsearch(&key, array, 2, sizeof(int), cmp));
  key = 2;
  CU_ASSERT_PTR_NULL(retrolibc_bsearch(&key, array, 2, sizeof(int), cmp));
}

static void test_bsearch_returns_pointer_to_element_for_array_with_one_element(void)
{
  static int array[1] = { 1 };
  int key = 1;
  CU_ASSERT_PTR_EQUAL(retrolibc_bsearch(&key, array, 1, sizeof(int), cmp), (void *) (array + 0));
}

static void test_bsearch_returns_null_for_array_with_one_element(void)
{
  static int array[1] = { 1 };
  int key = 0;
  CU_ASSERT_PTR_NULL(retrolibc_bsearch(&key, array, 1, sizeof(int), cmp));
  key = 2;
  CU_ASSERT_PTR_NULL(retrolibc_bsearch(&key, array, 1, sizeof(int), cmp));
}

static void test_bsearch_returns_null_for_array_without_elements(void)
{
  static int array[1] = { 1 };
  int key = 1;
  CU_ASSERT_PTR_NULL(retrolibc_bsearch(&key, array, 0, sizeof(int), cmp));
}

int add_bsearch_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("bsearch suite", initialize_bsearch, cleanup_bsearch)) == NULL) return -1;
  if(CU_add_test(suite, "bsearch returns pointer to element", test_bsearch_returns_pointer_to_element) == NULL) return -1;
  if(CU_add_test(suite, "bsearch returns null", test_bsearch_returns_null) == NULL) return -1;
  if(CU_add_test(suite, "bsearch returns pointer to element for array with two elements", test_bsearch_returns_pointer_to_element_for_array_with_two_elements) == NULL) return -1;
  if(CU_add_test(suite, "bsearch returns null for array with two elements", test_bsearch_returns_null_for_array_with_two_elements) == NULL) return -1;
  if(CU_add_test(suite, "bsearch returns pointer to element for array with one element", test_bsearch_returns_pointer_to_element_for_array_with_one_element) == NULL) return -1;
  if(CU_add_test(suite, "bsearch returns null for array with one element", test_bsearch_returns_null_for_array_with_one_element) == NULL) return -1;
  if(CU_add_test(suite, "bsearch returns null for array without elements", test_bsearch_returns_null_for_array_without_elements) == NULL) return -1;
  return 0;
}

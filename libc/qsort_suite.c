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

static int initialize_qsort(void)
{
  srand(test_seed());
  return 0;
}

static int cleanup_qsort(void)
{ return 0; }

static int cmp(const void *s, const void *s2)
{ return *((const int *) s) - *((const int *) s2); }

static void test_qsort_sorts_array(void)
{
  static int array[1000];
  static int expected_array[1000];
  size_t i;
  for(i = 0; i < 1000; i++) array[i] = expected_array[i] = rand() / (RAND_MAX * 1.0) * 1000000.0;
  qsort(expected_array, 1000, sizeof(int), cmp);
  retrolibc_qsort(array, 1000, sizeof(int), cmp);
  CU_ASSERT_EQUAL(memcmp(expected_array, array, 1000 * sizeof(int)), 0);
  for(i = 0; i < 1000; i++) array[i] = expected_array[i] = rand() / (RAND_MAX * 1.0) * 1000000.0;
  qsort(expected_array, 1000, sizeof(int), cmp);
  retrolibc_qsort(array, 1000, sizeof(int), cmp);
  CU_ASSERT_EQUAL(memcmp(expected_array, array, 1000 * sizeof(int)), 0);
}

static void test_qsort_sorts_array_with_two_elements()
{
  static int array[2] = { 1, 2 };
  static int expected_array[2] = { 1, 2 };
  static int array2[2] = { 2, 1 };
  static int expected_array2[2] = { 1, 2 };
  retrolibc_qsort(array, 2, sizeof(int), cmp);
  CU_ASSERT_EQUAL(memcmp(expected_array, array, 2 * sizeof(int)), 0);
  retrolibc_qsort(array2, 2, sizeof(int), cmp);
  CU_ASSERT_EQUAL(memcmp(expected_array2, array2, 2 * sizeof(int)), 0);
}

static void test_qsort_sorts_array_with_one_element()
{
  static int array[1] = { 1 };
  static int expected_array[1] = { 1 };
  retrolibc_qsort(array, 1, sizeof(int), cmp);
  CU_ASSERT_EQUAL(memcmp(expected_array, array, 1 * sizeof(int)), 0);
}

static void test_qsort_sorts_array_without_elements()
{
  static int array[1] = { 1 };
  static int expected_array[1] = { 1 };
  retrolibc_qsort(array, 0, sizeof(int), cmp);
}

int add_qsort_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("qsort suite", initialize_qsort, cleanup_qsort)) == NULL) return -1;
  if(CU_add_test(suite, "qsort sorts array", test_qsort_sorts_array) == NULL) return -1;
  if(CU_add_test(suite, "qsort sorts array with two elements", test_qsort_sorts_array_with_two_elements) == NULL) return -1;
  if(CU_add_test(suite, "qsort sorts sorted array with one element", test_qsort_sorts_array_with_one_element) == NULL) return -1;
  if(CU_add_test(suite, "qsort sorts sorted array without elements", test_qsort_sorts_array_without_elements) == NULL) return -1;
  return 0;
}

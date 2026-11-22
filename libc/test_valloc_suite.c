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
#include "priv_malloc.h"

void initialize_heap(void);

static int initialize_valloc(void)
{ return 0; }

static int cleanup_valloc(void)
{ return 0; }

void test_valloc_allocates_page_aligned_memory(void)
{
  void *p;
  initialize_heap();
  p = retrolibc_valloc(10);
  memset(p, 'X', 10);
  CU_ASSERT_PTR_NOT_NULL(p);
  CU_ASSERT_EQUAL(PAGE_ALIGN((uintptr_t) p), (uintptr_t) p);
  if(p != NULL) retrolibc_free(p);
}

void test_realloc_reallocates_memory_for_pointer_from_valloc(void)
{
  void *p;
  void *new_p;
  char *new_s;
  size_t i;
  int is_x;
  initialize_heap();
  p = retrolibc_valloc(10);
  memset(p, 'X', 10);
  new_p = retrolibc_realloc(p, 20);
  CU_ASSERT_PTR_NOT_NULL(new_p);
  new_s = (char *) new_p;
  is_x = 1;
  for(i = 0; i < 10; i++) is_x &= (new_s[i] == 'X');
  CU_ASSERT_NOT_EQUAL(is_x, 0);
  if(new_p != NULL) retrolibc_free(new_p);
}

void test_realloc_reallocates_memory_for_pointer_from_valloc_and_page_size_times_2(void)
{
  void *p;
  void *new_p;
  char *new_s;
  size_t i;
  int is_x;
  initialize_heap();
  p = retrolibc_valloc(10);
  memset(p, 'X', 10);
  new_p = retrolibc_realloc(p, PAGE_SIZE * 2);
  CU_ASSERT_PTR_NOT_NULL(new_p);
  new_s = (char *) new_p;
  is_x = 1;
  for(i = 0; i < 10; i++) is_x &= (new_s[i] == 'X');
  CU_ASSERT_NOT_EQUAL(is_x, 0);
  if(new_p != NULL) retrolibc_free(new_p);
}

int add_valloc_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("valloc suite", initialize_valloc, cleanup_valloc)) == NULL) return -1;
  if(CU_add_test(suite, "valloc allocates page aligned memory", test_valloc_allocates_page_aligned_memory) == NULL) return -1;
  if(CU_add_test(suite, "realloc reallocates memory for pointer from valloc", test_realloc_reallocates_memory_for_pointer_from_valloc) == NULL) return -1;
  if(CU_add_test(suite, "realloc reallocates memory for pointer from valloc and PAGE_SIZE * 2", test_realloc_reallocates_memory_for_pointer_from_valloc_and_page_size_times_2) == NULL) return -1;
  return 0;
}

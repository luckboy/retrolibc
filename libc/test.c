/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <stdlib.h>
#include <time.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"

static unsigned static_test_seed;

unsigned test_seed(void)
{ return static_test_seed; }

int add_ctype_suite(void);
int add_stdlib_suite(void);
int add_string_suite(void);
int add_bsearch_suite(void);
int add_malloc_suite(void);
int add_qsort_suite(void);
int add_strtol_suite(void);

int main(int argc, char **argv)
{
  static_test_seed = (argc >= 2 ? strtoul(argv[1], NULL, 16) : time(NULL));
  printf("test seed: %x\n", static_test_seed);
  if(CU_initialize_registry() != CUE_SUCCESS) return CU_get_error();
  if(add_ctype_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_stdlib_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_string_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_bsearch_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_malloc_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_qsort_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_strtol_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  CU_basic_run_tests();
  CU_cleanup_registry();
  printf("test seed: %x\n", static_test_seed);
  return CU_get_error();
}

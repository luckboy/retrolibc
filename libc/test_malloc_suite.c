/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"
#include "priv_malloc.h"

#ifndef LONG_BIT
#define LONG_BIT        __WORDSIZE
#endif

void initialize_retrolibc_malloc(void);
struct free_mem_header *retrolibc_first_free_mem(void);
struct free_mem_header *retrolibc_last_free_mem(void);

static char brk_mem[10000] __attribute__((aligned(8)));
static char *brk_addr;
static int brk_count_to_err;
static int sbrk_count_to_err;

static void initialize_heap(void)
{
  memset(brk_mem, 0, 10000);
  brk_addr = brk_mem;
  brk_count_to_err = INT_MAX;
  sbrk_count_to_err = INT_MAX;
  initialize_retrolibc_malloc();
}

int mock_brk(void *addr)
{
  if(brk_count_to_err <= 0) {
    errno = ENOMEM;
    return -1;
  }
  brk_count_to_err--;
  brk_addr = addr;
  return 0;
}

void *mock_sbrk(intptr_t inc)
{
  void *r; 
  if(sbrk_count_to_err <= 0) {
    errno = ENOMEM;
    return (void *) (-1);
  }
  sbrk_count_to_err--;
  r = brk_addr;
  brk_addr += inc;
  return r;
}

#define MEM(p)          (((char *) (p)) - sizeof(struct alloc_mem_header))
#define MEM_SIZE(size)  ((size) + sizeof(struct alloc_mem_header))
#define ALLOC_MEM(p)    ((struct alloc_mem_header *) MEM((p)))
#define FREE_MEM(p)     ((struct free_mem_header *) MEM((p)))

static int initialize_malloc(void)
{ return 0; }

static int cleanup_malloc(void)
{ return 0; }

void test_malloc_allocates_memory(void)
{
  void *p;
  initialize_heap();
  p = retrolibc_malloc(10);
  CU_ASSERT_PTR_NOT_NULL(p);
  CU_ASSERT_PTR_EQUAL(MEM(p), brk_mem);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16));
}

void test_malloc_allocates_memory_3_times(void)
{
  void *p;
  void *p2;
  void *p3;
  initialize_heap();
  p = retrolibc_malloc(10);
  CU_ASSERT_PTR_NOT_NULL(p);
  CU_ASSERT_PTR_EQUAL(MEM(p), brk_mem);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16));
  p2 = retrolibc_malloc(20);
  CU_ASSERT_PTR_NOT_NULL(p2);
  CU_ASSERT_PTR_EQUAL(MEM(p2), brk_mem + MEM_SIZE(16));
  CU_ASSERT_EQUAL(ALLOC_MEM(p2)->size, MEM_SIZE(24));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) + MEM_SIZE(24));
  p3 = retrolibc_malloc(30);
  CU_ASSERT_PTR_NOT_NULL(p3);
  CU_ASSERT_PTR_EQUAL(MEM(p3), brk_mem + MEM_SIZE(16) + MEM_SIZE(24));
  CU_ASSERT_EQUAL(ALLOC_MEM(p3)->size, MEM_SIZE(32));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) + MEM_SIZE(24) + MEM_SIZE(32));
}

void test_malloc_allocates_memory_after_memory_freeing(void)
{
  void *p;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(20);
  retrolibc_malloc(30);
  retrolibc_free(p);
  new_p = retrolibc_malloc(24);
  CU_ASSERT_PTR_EQUAL(new_p, p);
  CU_ASSERT_PTR_EQUAL(MEM(p), brk_mem + MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) + MEM_SIZE(24) + MEM_SIZE(32));
}

void test_malloc_allocates_memory_after_3_memory_freeings_for_left(void)
{
  void *p;
  void *p2;
  void *p3;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(30);
  retrolibc_malloc(10);
  p2 = retrolibc_malloc(10);
  retrolibc_malloc(10);
  p3 = retrolibc_malloc(20);
  retrolibc_malloc(10);
  retrolibc_free(p);
  retrolibc_free(p2);
  retrolibc_free(p3);
  new_p = retrolibc_malloc(32);
  CU_ASSERT_PTR_EQUAL(new_p, p);
  CU_ASSERT_PTR_EQUAL(MEM(p), brk_mem + MEM_SIZE(16));
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(32));
  CU_ASSERT_EQUAL(FREE_MEM(p2)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(FREE_MEM(p2)->prev);
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p2)->next, FREE_MEM(p3));
  CU_ASSERT_EQUAL(FREE_MEM(p3)->size, MEM_SIZE(24));
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p3)->prev, FREE_MEM(p2));
  CU_ASSERT_PTR_NULL(FREE_MEM(p3)->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p2));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), FREE_MEM(p3));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 5 + MEM_SIZE(32) + MEM_SIZE(24));
}

void test_malloc_allocates_memory_after_3_memory_freeings_for_center(void)
{
  void *p;
  void *p2;
  void *p3;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  retrolibc_malloc(10);
  p2 = retrolibc_malloc(30);
  retrolibc_malloc(10);
  p3 = retrolibc_malloc(20);
  retrolibc_malloc(10);
  retrolibc_free(p);
  retrolibc_free(p2);
  retrolibc_free(p3);
  new_p = retrolibc_malloc(32);
  CU_ASSERT_PTR_EQUAL(new_p, p2);
  CU_ASSERT_PTR_EQUAL(MEM(p2), brk_mem + MEM_SIZE(16) * 3);
  CU_ASSERT_EQUAL(ALLOC_MEM(p2)->size, MEM_SIZE(32));
  CU_ASSERT_EQUAL(FREE_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->prev);
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p)->next, FREE_MEM(p3));
  CU_ASSERT_EQUAL(FREE_MEM(p3)->size, MEM_SIZE(24));
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p3)->prev, FREE_MEM(p));
  CU_ASSERT_PTR_NULL(FREE_MEM(p3)->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), FREE_MEM(p3));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 5 + MEM_SIZE(32) + MEM_SIZE(24));
}

void test_malloc_allocates_memory_after_3_memory_freeings_for_right(void)
{
  void *p;
  void *p2;
  void *p3;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  retrolibc_malloc(10);
  p2 = retrolibc_malloc(20);
  retrolibc_malloc(10);
  p3 = retrolibc_malloc(30);
  retrolibc_malloc(10);
  retrolibc_free(p);
  retrolibc_free(p2);
  retrolibc_free(p3);
  new_p = retrolibc_malloc(32);
  CU_ASSERT_PTR_EQUAL(new_p, p3);
  CU_ASSERT_PTR_EQUAL(MEM(p3), brk_mem + MEM_SIZE(16) * 4 + MEM_SIZE(24));
  CU_ASSERT_EQUAL(ALLOC_MEM(p3)->size, MEM_SIZE(32));
  CU_ASSERT_EQUAL(FREE_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->prev);
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p)->next, FREE_MEM(p2));
  CU_ASSERT_EQUAL(FREE_MEM(p2)->size, MEM_SIZE(24));
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p2)->prev, FREE_MEM(p));
  CU_ASSERT_PTR_NULL(FREE_MEM(p2)->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), FREE_MEM(p2));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 5 + MEM_SIZE(32) + MEM_SIZE(24));
}

void test_malloc_allocates_memory_and_adds_free_smaller_memory(void)
{
  void *p;
  void *p2;
  void *p3;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  retrolibc_malloc(10);
  p2 = retrolibc_malloc(32 + sizeof(struct free_mem_header) + 8);
  retrolibc_malloc(10);
  p3 = retrolibc_malloc(20);
  retrolibc_malloc(10);
  retrolibc_free(p);
  retrolibc_free(p2);
  retrolibc_free(p3);
  new_p = retrolibc_malloc(32);
  CU_ASSERT_PTR_EQUAL(new_p, p2);
  CU_ASSERT_PTR_EQUAL(MEM(p2), brk_mem + MEM_SIZE(16) * 3);
  CU_ASSERT_EQUAL(ALLOC_MEM(p2)->size, MEM_SIZE(32));
  CU_ASSERT_EQUAL(FREE_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->prev);
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p)->next, FREE_MEM(p3));
  CU_ASSERT_EQUAL(FREE_MEM(p3)->size, MEM_SIZE(24));
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p3)->prev, FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p3)->next, (struct free_mem_header *) (p2 + 32));
  CU_ASSERT_EQUAL(((struct free_mem_header *) (p2 + 32))->size, sizeof(struct free_mem_header) + 8);
  CU_ASSERT_PTR_EQUAL(((struct free_mem_header *) (p2 + 32))->prev, FREE_MEM(p3));
  CU_ASSERT_PTR_NULL(((struct free_mem_header *) (p2 + 32))->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), (struct free_mem_header *) (p2 + 32));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 5 + MEM_SIZE(32 + sizeof(struct free_mem_header) + 8) + MEM_SIZE(24));
}

void test_malloc_allocates_memory_and_adds_free_smallest_memory(void)
{
  void *p;
  void *p2;
  void *p3;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  retrolibc_malloc(10);
  p2 = retrolibc_malloc(32 + sizeof(struct free_mem_header));
  retrolibc_malloc(10);
  p3 = retrolibc_malloc(20);
  retrolibc_malloc(10);
  retrolibc_free(p);
  retrolibc_free(p2);
  retrolibc_free(p3);
  new_p = retrolibc_malloc(32);
  CU_ASSERT_PTR_EQUAL(new_p, p2);
  CU_ASSERT_PTR_EQUAL(MEM(p2), brk_mem + MEM_SIZE(16) * 3);
  CU_ASSERT_EQUAL(ALLOC_MEM(p2)->size, MEM_SIZE(32));
  CU_ASSERT_EQUAL(FREE_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->prev);
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p)->next, FREE_MEM(p3));
  CU_ASSERT_EQUAL(FREE_MEM(p3)->size, MEM_SIZE(24));
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p3)->prev, FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p3)->next, (struct free_mem_header *) (p2 + 32));
  CU_ASSERT_EQUAL(((struct free_mem_header *) (p2 + 32))->size, sizeof(struct free_mem_header));
  CU_ASSERT_PTR_EQUAL(((struct free_mem_header *) (p2 + 32))->prev, FREE_MEM(p3));
  CU_ASSERT_PTR_NULL(((struct free_mem_header *) (p2 + 32))->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), (struct free_mem_header *) (p2 + 32));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 5 + MEM_SIZE(32 + sizeof(struct free_mem_header)) + MEM_SIZE(24));
}

void test_malloc_allocates_memory_for_first_match(void)
{
  void *p;
  void *p2;
  void *p3;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(24 + sizeof(struct free_mem_header));
  retrolibc_malloc(10);
  p2 = retrolibc_malloc(10);
  retrolibc_malloc(10);
  p3 = retrolibc_malloc(20);
  retrolibc_malloc(10);
  retrolibc_free(p);
  retrolibc_free(p2);
  retrolibc_free(p3);
  new_p = retrolibc_malloc(20);
  CU_ASSERT_PTR_EQUAL(new_p, p);
  CU_ASSERT_PTR_EQUAL(MEM(p), brk_mem + MEM_SIZE(16));
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(24));
  CU_ASSERT_EQUAL(FREE_MEM(p2)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(FREE_MEM(p2)->prev);
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p2)->next, FREE_MEM(p3));
  CU_ASSERT_EQUAL(FREE_MEM(p3)->size, MEM_SIZE(24));
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p3)->prev, FREE_MEM(p2));
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p3)->next, (struct free_mem_header *) (p + 24));
  CU_ASSERT_EQUAL(((struct free_mem_header *) (p + 24))->size, sizeof(struct free_mem_header));
  CU_ASSERT_PTR_EQUAL(((struct free_mem_header *) (p + 24))->prev, FREE_MEM(p3));
  CU_ASSERT_PTR_NULL(((struct free_mem_header *) (p + 24))->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p2));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), (struct free_mem_header *) (p + 24));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 5 + MEM_SIZE(24 + sizeof(struct free_mem_header)) + MEM_SIZE(24));
}

void test_malloc_aligns_heap(void)
{
  void *p;
  initialize_heap();
  brk_addr += 7;
  p = retrolibc_malloc(10);
  CU_ASSERT_PTR_NOT_NULL(p);
  CU_ASSERT_PTR_EQUAL(MEM(p), brk_mem + 8);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + 8 + MEM_SIZE(16));
}

void test_malloc_complains_on_out_of_memory_for_counter_0(void)
{
  void *p;
  initialize_heap();
  sbrk_count_to_err = 0;
  errno = 0;
  p = retrolibc_malloc(10);
  CU_ASSERT_EQUAL(errno, ENOMEM);
  CU_ASSERT_PTR_NULL(p);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem);
}

void test_malloc_complains_on_out_of_memory_for_counter_1(void)
{
  void *p;
  initialize_heap();
  sbrk_count_to_err = 1;
  errno = 0;
  p = retrolibc_malloc(10);
  CU_ASSERT_EQUAL(errno, ENOMEM);
  CU_ASSERT_PTR_NULL(p);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem);
}

void test_malloc_allocates_larger_memory(void)
{
  void *p;
  initialize_heap();
  p = retrolibc_malloc(PAGE_SIZE);
  memset(p, 'X', PAGE_SIZE);
  CU_ASSERT_PTR_NOT_NULL(p);
  CU_ASSERT_PTR_NOT_EQUAL(MEM(p), brk_mem);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, PAGE_SIZE * 2);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem);
  munmap(MEM(p), ALLOC_MEM(p)->size);
}

void test_realloc_allocates_memory_for_null(void)
{
  void *p;
  initialize_heap();
  p = retrolibc_realloc(NULL, 10);
  CU_ASSERT_PTR_NOT_NULL(p);
  CU_ASSERT_PTR_EQUAL(MEM(p), brk_mem);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16));
}

void test_realloc_allocates_memory_3_times_for_null(void)
{
  void *p;
  void *p2;
  void *p3;
  initialize_heap();
  p = retrolibc_realloc(NULL, 10);
  CU_ASSERT_PTR_NOT_NULL(p);
  CU_ASSERT_PTR_EQUAL(MEM(p), brk_mem);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16));
  p2 = retrolibc_realloc(NULL, 20);
  CU_ASSERT_PTR_NOT_NULL(p2);
  CU_ASSERT_PTR_EQUAL(MEM(p2), brk_mem + MEM_SIZE(16));
  CU_ASSERT_EQUAL(ALLOC_MEM(p2)->size, MEM_SIZE(24));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) + MEM_SIZE(24));
  p3 = retrolibc_realloc(NULL, 30);
  CU_ASSERT_PTR_NOT_NULL(p3);
  CU_ASSERT_PTR_EQUAL(MEM(p3), brk_mem + MEM_SIZE(16) + MEM_SIZE(24));
  CU_ASSERT_EQUAL(ALLOC_MEM(p3)->size, MEM_SIZE(32));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) + MEM_SIZE(24) + MEM_SIZE(32));
}

void test_realloc_increases_memory_area_size_for_right(void)
{
  void *p;
  void *p2;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  p2 = retrolibc_malloc(sizeof(struct free_mem_header) + 8);
  retrolibc_malloc(10);
  retrolibc_free(p2);
  new_p = retrolibc_realloc(p, 16 + sizeof(struct alloc_mem_header));
  CU_ASSERT_PTR_EQUAL(new_p, p);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(16 + sizeof(struct alloc_mem_header)));
  CU_ASSERT_EQUAL(((struct free_mem_header *) (((char *) p) + 16 + sizeof(struct alloc_mem_header)))->size, sizeof(struct free_mem_header) + 8);
  CU_ASSERT_PTR_NULL(((struct free_mem_header *) (((char *) p) + 16 + sizeof(struct alloc_mem_header)))->prev);
  CU_ASSERT_PTR_NULL(((struct free_mem_header *) (((char *) p) + 16 + sizeof(struct alloc_mem_header)))->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), ((struct free_mem_header *) (((char *) p) + 16 + sizeof(struct alloc_mem_header))));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), ((struct free_mem_header *) (((char *) p) + 16 + sizeof(struct alloc_mem_header))));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 3 + MEM_SIZE(sizeof(struct free_mem_header) + 8));
}  

void test_realloc_increases_memory_area_size_for_smallest_right(void)
{
  void *p;
  void *p2;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  p2 = retrolibc_malloc(sizeof(struct free_mem_header));
  retrolibc_malloc(10);
  retrolibc_free(p2);
  new_p = retrolibc_realloc(p, 16 + sizeof(struct alloc_mem_header));
  CU_ASSERT_PTR_EQUAL(new_p, p);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(16 + sizeof(struct alloc_mem_header)));
  CU_ASSERT_EQUAL(((struct free_mem_header *) (((char *) p) + 16 + sizeof(struct alloc_mem_header)))->size, sizeof(struct free_mem_header));
  CU_ASSERT_PTR_NULL(((struct free_mem_header *) (((char *) p) + 16 + sizeof(struct alloc_mem_header)))->prev);
  CU_ASSERT_PTR_NULL(((struct free_mem_header *) (((char *) p) + 16 + sizeof(struct alloc_mem_header)))->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), ((struct free_mem_header *) (((char *) p) + 16 + sizeof(struct alloc_mem_header))));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), ((struct free_mem_header *) (((char *) p) + 16 + sizeof(struct alloc_mem_header))));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 3 + MEM_SIZE(sizeof(struct free_mem_header)));
}

void test_realloc_increases_memory_area_size_and_removes_right(void)
{
  void *p;
  void *p2;
  void *new_p;
  initialize_heap();
#if LONG_BIT == 64
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  p2 = retrolibc_malloc(20);
  retrolibc_malloc(10);
  retrolibc_free(p2);
  new_p = retrolibc_realloc(p, 48);
  CU_ASSERT_PTR_EQUAL(new_p, p);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(48));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 3 + MEM_SIZE(24));
#else
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  p2 = retrolibc_malloc(10);
  retrolibc_malloc(10);
  retrolibc_free(p2);
  new_p = retrolibc_realloc(p, 40);
  CU_ASSERT_PTR_EQUAL(new_p, p);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(40));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 4);
#endif
}

void test_realloc_copies_memory_to_other_location_for_right(void)
{
  static char buf[10];
  static char buf2[64 - 10];
  void *p;
  void *p2;
  void *new_p;
  memset(buf, 'X', 10);
  memset(buf2, 0, 64 - 10);
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  p2 = retrolibc_malloc(10);
  retrolibc_malloc(10);
  retrolibc_free(p2);
  memset(p, 'X', 10);
  new_p = retrolibc_realloc(p, 60);
  CU_ASSERT_PTR_NOT_EQUAL(new_p, p);
  CU_ASSERT_EQUAL(memcmp(new_p, buf, 10), 0);
  CU_ASSERT_EQUAL(memcmp(((char *) new_p) + 10, buf2, 64 - 10), 0);
  CU_ASSERT_PTR_EQUAL(MEM(new_p), brk_mem + MEM_SIZE(16) * 4);
  CU_ASSERT_EQUAL(ALLOC_MEM(new_p)->size, MEM_SIZE(64));
  CU_ASSERT_EQUAL(FREE_MEM(p)->size, MEM_SIZE(16) * 2);
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->prev);
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 4 + MEM_SIZE(64));
}

void test_realloc_copies_memory_to_other_location(void)
{
  static char buf[10];
  static char buf2[24 - 10];
  void *p;
  void *new_p;
  memset(buf, 'X', 10);
  memset(buf2, 0, 24 - 10);
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  retrolibc_malloc(10);
  memset(p, 'X', 10);
  new_p = retrolibc_realloc(p, 20);
  CU_ASSERT_PTR_NOT_EQUAL(new_p, p);
  CU_ASSERT_EQUAL(memcmp(new_p, buf, 10), 0);
  CU_ASSERT_EQUAL(memcmp(((char *) new_p) + 10, buf2, 24 - 10), 0);
  CU_ASSERT_EQUAL(ALLOC_MEM(new_p)->size, MEM_SIZE(24));
  CU_ASSERT_EQUAL(FREE_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->prev);
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 3 + MEM_SIZE(24));
}

void test_realloc_indeed_allocates_memory_by_increasing(void)
{
  void *p;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  new_p = retrolibc_realloc(p, 30);
  CU_ASSERT_PTR_EQUAL(new_p, p);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(32));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) + MEM_SIZE(32));
}

void test_realloc_does_not_increases_and_decreases_memory_area_size(void)
{
  void *p;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  retrolibc_malloc(10);
  new_p = retrolibc_realloc(p, 10);
  CU_ASSERT_PTR_EQUAL(new_p, p);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 3);
}

void test_realloc_decreases_memory_area_size_for_right(void)
{
  void *p;
  void *p2;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(20);
  p2 = retrolibc_malloc(10);
  retrolibc_malloc(10);
  retrolibc_free(p2);
  new_p = retrolibc_realloc(p, 16);
  CU_ASSERT_PTR_EQUAL(new_p, p);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_EQUAL(((struct free_mem_header *) (((char *) p) + 16))->size, sizeof(struct alloc_mem_header) + 24);
  CU_ASSERT_PTR_NULL(((struct free_mem_header *) (((char *) p) + 16))->prev);
  CU_ASSERT_PTR_NULL(((struct free_mem_header *) (((char *) p) + 16))->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), ((struct free_mem_header *) (((char *) p) + 16)));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), ((struct free_mem_header *) (((char *) p) + 16)));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 3 + MEM_SIZE(24));
}

void test_realloc_decreases_memory_area_size(void)
{
  void *p;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(60);
  retrolibc_malloc(10);
  new_p = retrolibc_realloc(p, 16);
  CU_ASSERT_PTR_EQUAL(new_p, p);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_EQUAL(((struct free_mem_header *) (((char *) p) + 16))->size, 64 - 16);
  CU_ASSERT_PTR_NULL(((struct free_mem_header *) (((char *) p) + 16))->prev);
  CU_ASSERT_PTR_NULL(((struct free_mem_header *) (((char *) p) + 16))->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), ((struct free_mem_header *) (((char *) p) + 16)));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), ((struct free_mem_header *) (((char *) p) + 16)));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 2 + MEM_SIZE(64));
}

void test_realloc_decreases_memory_area_size_to_smallest(void)
{
  void *p;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(16 + sizeof(struct free_mem_header));
  retrolibc_malloc(10);
  new_p = retrolibc_realloc(p, 16);
  CU_ASSERT_PTR_EQUAL(new_p, p);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_EQUAL(((struct free_mem_header *) (((char *) p) + 16))->size, sizeof(struct free_mem_header));
  CU_ASSERT_PTR_NULL(((struct free_mem_header *) (((char *) p) + 16))->prev);
  CU_ASSERT_PTR_NULL(((struct free_mem_header *) (((char *) p) + 16))->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), ((struct free_mem_header *) (((char *) p) + 16)));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), ((struct free_mem_header *) (((char *) p) + 16)));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 2 + MEM_SIZE(16 + sizeof(struct free_mem_header)));
}

void test_realloc_indeed_frees_memory_by_decreasing(void)
{
  void *p;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(30);
  new_p = retrolibc_realloc(p, 10);
  CU_ASSERT_PTR_EQUAL(new_p, p);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 2);
}

void test_realloc_complains_on_out_of_memory_for_null_and_counter_0(void)
{
  void *p;
  initialize_heap();
  sbrk_count_to_err = 0;
  errno = 0;
  p = retrolibc_realloc(NULL, 10);
  CU_ASSERT_EQUAL(errno, ENOMEM);
  CU_ASSERT_PTR_NULL(p);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem);
}

void test_realloc_complains_on_out_of_memory_for_null_and_counter_1(void)
{
  void *p;
  initialize_heap();
  sbrk_count_to_err = 1;
  errno = 0;
  p = retrolibc_realloc(NULL, 10);
  CU_ASSERT_EQUAL(errno, ENOMEM);
  CU_ASSERT_PTR_NULL(p);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem);
}

void test_realloc_complains_on_out_of_memory_for_pointer_and_sbrk(void)
{
  void *p;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  sbrk_count_to_err = 0;
  new_p = retrolibc_realloc(p, 30);
  CU_ASSERT_PTR_NULL(new_p);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 2);
}

void test_realloc_complains_on_out_of_memory_for_pointer_and_brk(void)
{
  void *p;
  void *new_p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  brk_count_to_err = 0;
  new_p = retrolibc_realloc(p, 30);
  CU_ASSERT_PTR_NULL(new_p);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 2);
}

void test_realloc_allocates_larger_memory_for_null(void)
{
  void *p;
  initialize_heap();
  p = retrolibc_realloc(NULL, PAGE_SIZE);
  CU_ASSERT_PTR_NOT_NULL(p);
  CU_ASSERT_PTR_NOT_EQUAL(MEM(p), brk_mem);
  CU_ASSERT_EQUAL(ALLOC_MEM(p)->size, PAGE_SIZE * 2);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem);
  munmap(MEM(p), ALLOC_MEM(p)->size);
}

void test_realloc_reallocates_larger_memory_by_increasing_for_pointer(void)
{
  void *p;
  void *new_p;
  char *new_s;
  size_t i;
  int is_x;
  initialize_heap();
  p = retrolibc_malloc(PAGE_SIZE);
  memset(p, 'X', PAGE_SIZE);
  new_p = retrolibc_realloc(p, PAGE_SIZE * 2);
  CU_ASSERT_PTR_NOT_NULL(new_p);
  new_s = (char *) new_p;
  is_x = 1;
  for(i = 0; i < PAGE_SIZE; i++) is_x &= (new_s[i] == 'X');
  CU_ASSERT_NOT_EQUAL(is_x, 0);
  CU_ASSERT_EQUAL(ALLOC_MEM(new_p)->size, PAGE_SIZE * 3);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem);
  munmap(MEM(new_p), ALLOC_MEM(new_p)->size);
}

void test_realloc_reallocates_larger_memory_by_decreasing_for_pointer(void)
{
  void *p;
  void *new_p;
  char *new_s;
  size_t i, is_x;
  initialize_heap();
  p = retrolibc_malloc(PAGE_SIZE * 2);
  memset(p, 'X', PAGE_SIZE);
  new_p = retrolibc_realloc(p, PAGE_SIZE);
  CU_ASSERT_PTR_NOT_NULL(new_p);
  new_s = (char *) new_p;
  is_x = 1;
  for(i = 0; i < PAGE_SIZE; i++) is_x &= (new_s[i] == 'X');
  CU_ASSERT_NOT_EQUAL(is_x, 0);
  CU_ASSERT_EQUAL(ALLOC_MEM(new_p)->size, PAGE_SIZE * 2);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem);
  munmap(MEM(new_p), ALLOC_MEM(new_p)->size);
}

void test_realloc_copies_smaller_memory_area_to_large_memory_area(void)
{
  void *p;
  void *new_p;
  char *new_s;
  size_t i;
  int is_x;
  initialize_heap();
  p = retrolibc_malloc(10);
  memset(p, 'X', 10);
  new_p = retrolibc_realloc(p, PAGE_SIZE);
  CU_ASSERT_PTR_NOT_NULL(new_p);
  new_s = (char *) new_p;
  is_x = 1;
  for(i = 0; i < 10; i++) is_x &= (new_s[i] == 'X');
  CU_ASSERT_NOT_EQUAL(is_x, 0);
  CU_ASSERT_EQUAL(ALLOC_MEM(new_p)->size, PAGE_SIZE * 2);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem);
  munmap(MEM(new_p), ALLOC_MEM(new_p)->size);
}

void test_free_frees_memory(void)
{
  void *p;
  initialize_heap();
  p = retrolibc_malloc(10);
  retrolibc_free(p);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem);
}

void test_free_frees_memory_after_3_memory_allocations(void)
{
  void *p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(20);
  retrolibc_malloc(30);
  retrolibc_free(p);
  CU_ASSERT_EQUAL(FREE_MEM(p)->size, MEM_SIZE(24));
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->prev);
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) + MEM_SIZE(24) + MEM_SIZE(32));
}

void test_free_frees_memory_3_times_after_memory_allocations(void)
{
  void *p;
  void *p2;
  void *p3;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  retrolibc_malloc(10);
  p2 = retrolibc_malloc(10);
  retrolibc_malloc(10);
  p3 = retrolibc_malloc(10);
  retrolibc_malloc(10);
  retrolibc_free(p);
  CU_ASSERT_EQUAL(FREE_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->prev);
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 7);
  retrolibc_free(p2);
  CU_ASSERT_EQUAL(FREE_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->prev);
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p)->next, FREE_MEM(p2));
  CU_ASSERT_EQUAL(FREE_MEM(p2)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p2)->prev, FREE_MEM(p));
  CU_ASSERT_PTR_NULL(FREE_MEM(p2)->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), FREE_MEM(p2));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 7);
  retrolibc_free(p3);
  CU_ASSERT_EQUAL(FREE_MEM(p)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->prev);
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p)->next, FREE_MEM(p2));
  CU_ASSERT_EQUAL(FREE_MEM(p2)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p2)->prev, FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p2)->next, FREE_MEM(p3));
  CU_ASSERT_EQUAL(FREE_MEM(p3)->size, MEM_SIZE(16));
  CU_ASSERT_PTR_EQUAL(FREE_MEM(p3)->prev, FREE_MEM(p2));
  CU_ASSERT_PTR_NULL(FREE_MEM(p3)->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), FREE_MEM(p3));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 7);
}

void test_free_frees_memory_for_left_and_right(void)
{
  void *p;
  void *p2;
  void *p3;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  p2 = retrolibc_malloc(10);
  p3 = retrolibc_malloc(10);
  retrolibc_malloc(10);
  retrolibc_free(p);
  retrolibc_free(p3);
  retrolibc_free(p2);
  CU_ASSERT_EQUAL(FREE_MEM(p)->size, MEM_SIZE(16) * 3);
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->prev);
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 5);
}

void test_free_indeed_frees_memory_for_left(void)
{
  void *p;
  void *p2;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  p2 = retrolibc_malloc(10);
  retrolibc_free(p);
  retrolibc_free(p2);
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16));
}

void test_free_frees_memory_for_right(void)
{
  void *p;
  void *p2;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  p2 = retrolibc_malloc(10);
  retrolibc_malloc(10);
  retrolibc_free(p2);
  retrolibc_free(p);
  CU_ASSERT_EQUAL(FREE_MEM(p)->size, MEM_SIZE(16) * 2);
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->prev);
  CU_ASSERT_PTR_NULL(FREE_MEM(p)->next);
  CU_ASSERT_PTR_EQUAL(retrolibc_first_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(retrolibc_last_free_mem(), FREE_MEM(p));
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16) * 4);
}

void test_free_indeed_frees_memory(void)
{
  void *p;
  initialize_heap();
  retrolibc_malloc(10);
  p = retrolibc_malloc(10);
  retrolibc_free(p);
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem + MEM_SIZE(16));
}

void test_free_frees_larger_memory(void)
{
  void *p;
  initialize_heap();
  p = retrolibc_malloc(PAGE_SIZE);
  retrolibc_free(p);
  CU_ASSERT_PTR_NULL(retrolibc_first_free_mem());
  CU_ASSERT_PTR_NULL(retrolibc_last_free_mem());
  CU_ASSERT_PTR_EQUAL(brk_addr, brk_mem);
}

int add_malloc_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("malloc suite", initialize_malloc, cleanup_malloc)) == NULL) return -1;
  if(CU_add_test(suite, "malloc allocates memory", test_malloc_allocates_memory) == NULL) return -1;
  if(CU_add_test(suite, "malloc allocates memory 3 times", test_malloc_allocates_memory_3_times) == NULL) return -1;
  if(CU_add_test(suite, "malloc allocates memory after memory freeing", test_malloc_allocates_memory_after_memory_freeing) == NULL) return -1;
  if(CU_add_test(suite, "malloc allocates memory after 3 memory freeings for left", test_malloc_allocates_memory_after_3_memory_freeings_for_left) == NULL) return -1;
  if(CU_add_test(suite, "malloc allocates memory after 3 memory freeings for center", test_malloc_allocates_memory_after_3_memory_freeings_for_center) == NULL) return -1;
  if(CU_add_test(suite, "malloc allocates memory after 3 memory freeings for right", test_malloc_allocates_memory_after_3_memory_freeings_for_right) == NULL) return -1;
  if(CU_add_test(suite, "malloc allocates memory and adds free smaller memory", test_malloc_allocates_memory_and_adds_free_smaller_memory) == NULL) return -1;
  if(CU_add_test(suite, "malloc allocates memory and adds free smallest memory", test_malloc_allocates_memory_and_adds_free_smallest_memory) == NULL) return -1;
  if(CU_add_test(suite, "malloc allocates memory for first match", test_malloc_allocates_memory_for_first_match) == NULL) return -1;
  if(CU_add_test(suite, "malloc aligns heap", test_malloc_aligns_heap) == NULL) return -1;
  if(CU_add_test(suite, "malloc complains on out of memory for counter 0", test_malloc_complains_on_out_of_memory_for_counter_0) == NULL) return -1;
  if(CU_add_test(suite, "malloc complains on out of memory for counter 1", test_malloc_complains_on_out_of_memory_for_counter_1) == NULL) return -1;
  if(CU_add_test(suite, "malloc allocates larger memory", test_malloc_allocates_larger_memory) == NULL) return -1;
  if(CU_add_test(suite, "realloc allocates memory for null", test_realloc_allocates_memory_for_null) == NULL) return -1;
  if(CU_add_test(suite, "realloc allocates memory 3 times for null", test_realloc_allocates_memory_3_times_for_null) == NULL) return -1;
  if(CU_add_test(suite, "realloc increases memory area size for right", test_realloc_increases_memory_area_size_for_right) == NULL) return -1;
  if(CU_add_test(suite, "realloc increases memory area size for smallest right", test_realloc_increases_memory_area_size_for_smallest_right) == NULL) return -1;
  if(CU_add_test(suite, "realloc increases memory area size and removes right", test_realloc_increases_memory_area_size_and_removes_right) == NULL) return -1;
  if(CU_add_test(suite, "realloc copies memory to other location for right", test_realloc_copies_memory_to_other_location_for_right) == NULL) return -1;
  if(CU_add_test(suite, "realloc copies memory to other location", test_realloc_copies_memory_to_other_location) == NULL) return -1;
  if(CU_add_test(suite, "realloc indeed allocates memory by increasing", test_realloc_indeed_allocates_memory_by_increasing) == NULL) return -1;
  if(CU_add_test(suite, "realloc does not increases and decreases memory area size", test_realloc_does_not_increases_and_decreases_memory_area_size) == NULL) return -1;
  if(CU_add_test(suite, "realloc decreases memory area size for right", test_realloc_decreases_memory_area_size_for_right) == NULL) return -1;
  if(CU_add_test(suite, "realloc decreases memory area size", test_realloc_decreases_memory_area_size) == NULL) return -1;
  if(CU_add_test(suite, "realloc decreases memory area size to smallest", test_realloc_decreases_memory_area_size_to_smallest) == NULL) return -1;
  if(CU_add_test(suite, "realloc indeed frees memory by decreasing", test_realloc_indeed_frees_memory_by_decreasing) == NULL) return -1;
  if(CU_add_test(suite, "realloc complains on out of memory for null and counter 0", test_realloc_complains_on_out_of_memory_for_null_and_counter_0) == NULL) return -1;
  if(CU_add_test(suite, "realloc complains on out of memory for null and counter 1", test_realloc_complains_on_out_of_memory_for_null_and_counter_1) == NULL) return -1;
  if(CU_add_test(suite, "realloc complains on out of memory for pointer and sbrk", test_realloc_complains_on_out_of_memory_for_pointer_and_sbrk) == NULL) return -1;
  if(CU_add_test(suite, "realloc complains on out of memory for pointer and brk", test_realloc_complains_on_out_of_memory_for_pointer_and_brk) == NULL) return -1;
  if(CU_add_test(suite, "realloc allocates larger memory for null", test_realloc_allocates_larger_memory_for_null) == NULL) return -1;
  if(CU_add_test(suite, "realloc reallocates larger memory by increasing for pointer", test_realloc_reallocates_larger_memory_by_increasing_for_pointer) == NULL) return -1;
  if(CU_add_test(suite, "realloc reallocates larger memory by decreasing for pointer", test_realloc_reallocates_larger_memory_by_decreasing_for_pointer) == NULL) return -1;
  if(CU_add_test(suite, "realloc copies smaller memory area to large memory area", test_realloc_copies_smaller_memory_area_to_large_memory_area) == NULL) return -1;
  if(CU_add_test(suite, "free frees memory", test_free_frees_memory) == NULL) return -1;
  if(CU_add_test(suite, "free frees memory after 3 memory allocations", test_free_frees_memory_after_3_memory_allocations) == NULL) return -1;
  if(CU_add_test(suite, "free frees memory 3 times after memory allocations", test_free_frees_memory_3_times_after_memory_allocations) == NULL) return -1;
  if(CU_add_test(suite, "free frees memory for left and right", test_free_frees_memory_for_left_and_right) == NULL) return -1;
  if(CU_add_test(suite, "free indeed frees memory for left", test_free_indeed_frees_memory_for_left) == NULL) return -1;
  if(CU_add_test(suite, "free frees memory for right", test_free_frees_memory_for_right) == NULL) return -1;
  if(CU_add_test(suite, "free indeed frees memory", test_free_indeed_frees_memory) == NULL) return -1;
  if(CU_add_test(suite, "free frees larger memory", test_free_frees_larger_memory) == NULL) return -1;
  return 0;
}

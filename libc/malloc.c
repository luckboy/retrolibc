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
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#ifndef TEST
#include <stdlib.h>
#else
#define RETROLIBC_MALLOC
#include "retrolibc.h"
#endif
#include "priv_malloc.h"

#define ALIGN(size)             (((size) + 7UL) & ~7UL)

#define PAGE_ALIGN(size)        ((((size) + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE)

#define MIN_SMALLER_MEM_SIZE    (sizeof(struct free_mem_header))
#ifdef MAP_ANONYMOUS
#define MAX_SMALLER_MEM_SIZE    (PAGE_SIZE >> 1)
#endif

static pthread_mutex_t heap_mutex = PTHREAD_MUTEX_INITIALIZER;
static struct free_mem_header *first_free_mem = NULL;
static struct free_mem_header *last_free_mem = NULL;

#ifdef TEST

void initialize_retrolibc_malloc(void)
{
  first_free_mem = NULL;
  last_free_mem = NULL;
}

struct free_mem_header *retrolibc_first_free_mem(void)
{ return first_free_mem; }

struct free_mem_header *retrolibc_last_free_mem(void)
{ return last_free_mem; }

#endif

static void add_free_mem(struct free_mem_header *free_mem)
{
  if(last_free_mem != NULL) {
    last_free_mem->next = free_mem;
    free_mem->prev = last_free_mem;
    free_mem->next = NULL;
    last_free_mem = free_mem;
  } else {
    free_mem->prev = NULL;
    free_mem->next = NULL;
    first_free_mem = free_mem;
    last_free_mem = free_mem;
  }
}

static void remove_free_mem(struct free_mem_header *free_mem)
{
  if(free_mem->prev != NULL) {
    free_mem->prev->next = free_mem->next;
  } else {
    first_free_mem = free_mem->next;
  }
  if(free_mem->next != NULL) {
    free_mem->next->prev = free_mem->prev;
  } else {
    last_free_mem = free_mem->prev;
  }
}

static void *malloc_unlocked(size_t size);
static void *realloc_unlocked(void *p, size_t size);
static void free_unlocked(void *p);

static int align_heap(void)
{
  void *addr = sbrk(0);
  if(addr == (void *) (-1)) return -1;
  if((size_t) addr != ALIGN((size_t) addr)) {
     if(sbrk(ALIGN((size_t) addr) - ((size_t) addr)) == (void *) (-1)) return -1;
  }
  return 0;
}

static void *malloc_unlocked(size_t size)
{
  struct alloc_mem_header *alloc_mem;
  struct free_mem_header *free_mem;
  void *new_addr;
  size_t mem_size = ALIGN(sizeof(struct alloc_mem_header) + size);
  mem_size = (mem_size > MIN_SMALLER_MEM_SIZE ? mem_size : MIN_SMALLER_MEM_SIZE);
#ifdef MAP_ANONYMOUS
  if(mem_size > MAX_SMALLER_MEM_SIZE) {
    size_t mmap_size = PAGE_ALIGN(mem_size);
    new_addr = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(new_addr == MAP_FAILED) return NULL;
    alloc_mem = (struct alloc_mem_header *) new_addr;
    alloc_mem->size = mmap_size;
    return (void *) (((char *) alloc_mem) + sizeof(struct alloc_mem_header));
  }
#endif
  for(free_mem = first_free_mem; free_mem != NULL; free_mem = free_mem->next) {
    if(free_mem->size >= mem_size) {
      size_t alloc_mem_size = free_mem->size;
      remove_free_mem(free_mem);
      if(free_mem->size - mem_size >= MIN_SMALLER_MEM_SIZE) {
        struct free_mem_header *new_free_mem = (struct free_mem_header *) (((char *) free_mem) + mem_size);
        add_free_mem(new_free_mem);
        new_free_mem->size = free_mem->size - mem_size;
        alloc_mem_size = mem_size;
      }
      alloc_mem = (struct alloc_mem_header *) free_mem;
      alloc_mem->size = alloc_mem_size;
      return (void *) (((char *) alloc_mem) + sizeof(struct alloc_mem_header));
    }
  }
  if(align_heap() == -1) return NULL;
  new_addr = sbrk(mem_size);
  if(new_addr != (void *) (-1)) {
    alloc_mem = (struct alloc_mem_header *) new_addr;
    alloc_mem->size = mem_size;
    return (void *) (((char *) alloc_mem) + sizeof(struct alloc_mem_header));
  } else {
    return NULL;
  }
}

void *malloc(size_t size)
{
  void *p;
  pthread_mutex_lock(&heap_mutex);
  p = malloc_unlocked(size);
  pthread_mutex_unlock(&heap_mutex);
  return p;
}

static void *realloc_unlocked(void *p, size_t size)
{
  struct alloc_mem_header *alloc_mem = (struct alloc_mem_header *) (((char *) p) - sizeof(struct alloc_mem_header));
  struct free_mem_header *free_mem;
  struct free_mem_header *right_free_mem = NULL;
  size_t right_free_mem_size = 0;
  size_t mem_size = ALIGN(sizeof(struct alloc_mem_header) + size);
  mem_size = (mem_size > MIN_SMALLER_MEM_SIZE ? mem_size : MIN_SMALLER_MEM_SIZE);
#ifdef MAP_ANONYMOUS
  if(alloc_mem->size >= PAGE_SIZE) {
    size_t mmap_size = PAGE_ALIGN(mem_size);
    if(mmap_size > alloc_mem->size) {
#ifdef MREMAP_MAYMOVE
      void *new_addr;
      int saved_errno = errno;
      errno = 0;
      new_addr = mremap((void *) alloc_mem, alloc_mem->size, mmap_size, MREMAP_MAYMOVE);
      if(new_addr == MAP_FAILED) {
        if(errno == ENOSYS) {
          errno = saved_errno;
          size_t old_size = alloc_mem->size - sizeof(struct alloc_mem_header);
          void *new_p = malloc_unlocked(size);
          if(new_p == NULL) return NULL;
          memcpy(new_p, p, old_size);
          free_unlocked(p);
          return new_p;
        } else {
          return NULL;
        }
      }
      alloc_mem = (struct alloc_mem_header *) new_addr;
      alloc_mem->size = mmap_size;
      return (void *) (((char *) alloc_mem) + sizeof(struct alloc_mem_header));
#else
      size_t old_size = alloc_mem->size - sizeof(struct alloc_mem_header);
      void *new_p = malloc_unlocked(size);
      if(new_p == NULL) return NULL;
      memcpy(new_p, p, old_size);
      free_unlocked(p);
      return new_p;
#endif
    } else if(mmap_size < alloc_mem->size) {
#ifdef MREMAP_MAYMOVE
      void *new_addr;
      int saved_errno = errno;
      errno = 0;
      new_addr = mremap((void *) alloc_mem, alloc_mem->size, mmap_size, MREMAP_MAYMOVE);
      if(new_addr == MAP_FAILED) {
        if(errno == ENOSYS) {
          errno = saved_errno;
          return p;
        }
      }
      alloc_mem = (struct alloc_mem_header *) new_addr;
      alloc_mem->size = mmap_size;
      return (void *) (((char *) alloc_mem) + sizeof(struct alloc_mem_header));
#else
      return p;
#endif
    } else {
      return p;
    }
  }
  if(mem_size > MAX_SMALLER_MEM_SIZE) {
    size_t old_size = alloc_mem->size - sizeof(struct alloc_mem_header);
    void *new_p = malloc_unlocked(size);
    if(new_p == NULL) return NULL;
    memcpy(new_p, p, old_size);
    free_unlocked(p);
    return new_p;
  }
#endif
  if(mem_size != alloc_mem->size) {
    void *addr = sbrk(0);
    if(addr == (void *) (-1)) return NULL;
    for(free_mem = first_free_mem; free_mem != NULL; free_mem = free_mem->next) {
      if(((char *) free_mem) == ((char *) alloc_mem) + alloc_mem->size) {
        right_free_mem = free_mem;
        right_free_mem_size = right_free_mem->size;
        break;
      }
    }
    if(((char *) addr) == ((char *) alloc_mem) + alloc_mem->size)  {
      if(brk((void *) (((char *) alloc_mem) + mem_size))) return NULL;
      alloc_mem->size = mem_size;
      return p;
    }
  }
  if(mem_size > alloc_mem->size) {
    if(mem_size <= alloc_mem->size + right_free_mem_size) {
      if(right_free_mem != NULL) {
        right_free_mem_size = right_free_mem->size;
        remove_free_mem(right_free_mem);
      }
      if(alloc_mem->size + right_free_mem_size - mem_size >= MIN_SMALLER_MEM_SIZE) {
        struct free_mem_header *new_free_mem = (struct free_mem_header *) (((char *) alloc_mem) + mem_size);
        add_free_mem(new_free_mem);
        new_free_mem->size = alloc_mem->size - mem_size + right_free_mem_size;
        alloc_mem->size = mem_size;
      } else {
        alloc_mem->size += right_free_mem_size;
      }
    } else {
      size_t old_size = alloc_mem->size - sizeof(struct alloc_mem_header);
      void *new_p = malloc_unlocked(size);
      if(new_p == NULL) return NULL;
      memcpy(new_p, p, old_size);
      free_unlocked(p);
      return new_p;
    }
  } if(mem_size < alloc_mem->size) {
    if(right_free_mem != NULL) {
      right_free_mem_size = right_free_mem->size;
      remove_free_mem(right_free_mem);
    }
    if(alloc_mem->size + right_free_mem_size - mem_size >= MIN_SMALLER_MEM_SIZE) {
      struct free_mem_header *new_free_mem = (struct free_mem_header *) (((char *) alloc_mem) + mem_size);
      add_free_mem(new_free_mem);
      new_free_mem->size = alloc_mem->size - mem_size + right_free_mem_size;
      alloc_mem->size = mem_size;
    }
  }
  return p;
}

void *realloc(void *p, size_t size)
{
  void *new_p;
  if(p == NULL) return malloc(size);
  pthread_mutex_lock(&heap_mutex);
  new_p = realloc_unlocked(p, size);
  pthread_mutex_unlock(&heap_mutex);
  return new_p;
}

static void free_unlocked(void *p)
{
  struct alloc_mem_header *alloc_mem = (struct alloc_mem_header *) (((char *) p) - sizeof(struct alloc_mem_header));
  struct free_mem_header *free_mem;
  struct free_mem_header *left_free_mem = NULL;
  struct free_mem_header *right_free_mem = NULL;
#ifdef MAP_ANONYMOUS
  if(alloc_mem->size >= PAGE_SIZE) {
    munmap((void *) alloc_mem, alloc_mem->size);
    return;
  }
#endif
  for(free_mem = first_free_mem; free_mem != NULL; free_mem = free_mem->next) {
    if(((char *) free_mem) + free_mem->size == (char *) alloc_mem) left_free_mem = free_mem;
    if(((char *) free_mem) == ((char *) alloc_mem) + alloc_mem->size) right_free_mem = free_mem;
    if(left_free_mem != NULL && right_free_mem != NULL) break;
  }
  if(left_free_mem != NULL) {
    if((char *) sbrk(0) == ((char *) alloc_mem) + alloc_mem->size)  {
      remove_free_mem(left_free_mem);
      brk((void *) left_free_mem);
      return;
    }
    if(right_free_mem != NULL) {
      left_free_mem->size += alloc_mem->size + right_free_mem->size;
      remove_free_mem(right_free_mem);
    } else {
      left_free_mem->size = alloc_mem->size;
    }
  } else {
    size_t free_mem_size;
    if((char *) sbrk(0) == ((char *) alloc_mem) + alloc_mem->size)  {
      brk((void *) alloc_mem);
      return;
    }
    free_mem_size = alloc_mem->size;
    free_mem = (struct free_mem_header *) alloc_mem;
    add_free_mem(free_mem);
    free_mem->size = free_mem_size;
    if(right_free_mem != NULL) {
      free_mem->size += right_free_mem->size;
      remove_free_mem(right_free_mem);
    }
  }
}

void free(void *p)
{
  pthread_mutex_lock(&heap_mutex);
  free_unlocked(p);
  pthread_mutex_unlock(&heap_mutex);
}

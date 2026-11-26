/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <inttypes.h>
#ifndef TEST
#include <stdlib.h>
#else
#define RETROLIBC_VALLOC
#include "retrolibc.h"
#endif
#include "priv_malloc.h"

void *valloc(size_t size)
{
  struct alloc_mem_header *alloc_mem;
  void *p;
  void *new_p;
  p = malloc(size + PAGE_SIZE - 1);
  if(p == NULL) return NULL;
  new_p = (void *) PAGE_ALIGN((uintptr_t) p);
  if(new_p == p) return p;
  alloc_mem = (struct alloc_mem_header *) (((char *) new_p) - sizeof(struct alloc_mem_header));
  alloc_mem->size = (size_t) ((uintptr_t) new_p - (uintptr_t) p) | VALLOC_FLAG;
  return new_p;
}

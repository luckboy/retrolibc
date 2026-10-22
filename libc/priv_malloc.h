/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _PRIV_MALLOC_H
#define _PRIV_MALLOC_H

#include <limits.h>
#include <stddef.h>

#ifndef LONG_BIT
#define LONG_BIT        __WORDSIZE
#endif

struct alloc_mem_header
{
  size_t size;
#if LONG_BIT != 64
  unsigned __pad;
#endif
};

struct free_mem_header
{
  size_t size;
  struct free_mem_header *prev;
  struct free_mem_header *next;
#if LONG_BIT != 64
  unsigned __pad;
#endif
};

#endif

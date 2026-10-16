/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef TEST
#include <stdlib.h>
#else
#define RETROLIBC_QSORT
#include "retrolibc.h"
#endif
#include <stdlib.h>

static void swap(char *s, char *s2, size_t n)
{
  for(; n > 0; s++, s2++, n--) {
    char tmp = *s;
    *s = *s2;
    *s2 = tmp;
  }
}

static void quicksort(char *s, size_t l, size_t r, size_t width, int (*cmp)(const void *, const void *))
{
  char *pivot;
  size_t i, j;
  if(l >= r) return;
  pivot =  s + r * width;
  i = l;
  for(j = l; j <= r - 1; j++) {
    if(cmp((const void *) (s + j * width), (const void *) pivot) <= 0) {
      swap(s + i * width, s + j * width, width);
      i++;
    }
  }
  swap(s + i * width, pivot, width);
  if(i > l) quicksort(s, l, i - 1, width, cmp);
  if(i < r) quicksort(s, i + 1, r, width, cmp);
}

void qsort(void *base, size_t n, size_t width, int (*cmp)(const void *, const void *))
{ if(n > 0) quicksort((char *) base, 0, n - 1, width, cmp); }

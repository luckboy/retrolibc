/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <stdlib.h>

static void swap(char *s, char *s2, size_t n)
{
  for(; n > 0; s++, s2++, n--) {
    char tmp = *s;
    *s = *s2;
    *s2 = tmp;
  }
}

void quicksort(char *s, size_t l, size_t r, size_t width, int (*cmp)(const void *, const void *))
{
  if(l >= r) return;
  char *pivot = s + (l + r) * width;
  size_t i = l;
  size_t j = r;
  while(1) {
    for(; cmp((const void *) pivot, (const void *) (s + i * width)) > 0; i++);
    for(; cmp((const void *) pivot, (const void *) (s + j * width)) < 0; j--);
    if(i >= j) break;
    if(i != j) swap(s + i * width, s + j * width, width);
    i++;
    j--; 
  }
  quicksort(s, l, j, width, cmp);
  quicksort(s, j + 1, l, width, cmp);
}

void qsort(void *base, size_t n, size_t width, int (*cmp)(const void *, const void *))
{ if(n > 0) quicksort((char *) base, 0, n - 1, width, cmp); }

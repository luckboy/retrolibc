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
#define RETROLIBC_BSEARCH
#include "retrolibc.h"
#endif

void *bsearch(const void *key, const void *base, size_t n, size_t width, int (*cmp)(const void *, const void *))
{
  const char *s = (const char *) base;
  size_t l, r;
  if(n <= 0) return NULL;
  l = 0;
  r = n - 1;
  while(1) {
    size_t m = (l + r) >> 1;
    int q = cmp(key, (const void *) (s + m * width));
    if(q < 0) {
      if(l >= m) break;
      r = m - 1;
    } else if(q > 0) {
      if(m >= r) break;
      l = m + 1;
    } else {
      return (void *) (s + m * width);
    }
  }
  return NULL;
}

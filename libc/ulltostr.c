/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <stddef.h>
#include "priv_stdlib.h"

char *__ulltostr(char *buf, unsigned long long n, int base, int is_uppercase)
{
  char *s = buf;
  char *t = buf;
  if(base < 2 || base > 36) return NULL;
  for(; n != 0; t++, n /= base) {
    int digit = n % base;
    if(digit <= (base >= 10 ? 9 : base - 1)) {
      *t = digit + '0';
    } else {
      *t = digit - 10 + (is_uppercase ? 'A' : 'a'); 
    }
  }
  if(t == buf) {
    *t = '0';
    t++;
  }
  *t = '\0';
  for(t--; s < t; s++, t--) {
    char tmp = *s;
    *s = *t;
    *t = tmp;
  }
  return buf;
}

/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include "priv_stdlib.h"
 
char *__efcvt(double n, int ndigits, int *decpt, int *sign, char *buf, int max_ndigits, int is_ecvt)
{
  char *s;
  double exp10, d;
  int tmp_decpt;
  int tmp_sign = 0;
  if(ndigits < 0) ndigits = 0;
  if(n < 0.0) {
    n = -n;
    tmp_sign = 1;
  }
  exp10 = (n != 0.0 ? floor(log10(n)) : 0.0);
  n /= pow(10.0, exp10);
  if(n >= 10.0) {
    n /= 10.0;
    exp10 += 1.0;
  } else if(n < 1.0) {
    n *= 10.0;
    exp10 -= 1.0;
  }
  tmp_decpt = exp10 + 1;
  if(!is_ecvt) {
    if(tmp_decpt > 0 && ndigits > INT_MAX - tmp_decpt) return NULL;
    ndigits += tmp_decpt;
  }
  for(s = buf; max_ndigits > 0 && ndigits > 0; s++, max_ndigits--, ndigits--, n = (n - d) * 10.0) {
    int digit;
    d = floor(n);
    digit = (d > 9.0 ? 9 : ((int) d));
    *s = digit + '0';
  }
  *s = '\0';
  *decpt = tmp_decpt;
  *sign = tmp_sign;
  return buf;
}

/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#ifndef TEST
#include "priv_stdlib.h"
#else
#define RETROLIBC_EFCVT
#include "retrolibc.h"
#endif
 
char *__efcvt(double n, int ndigits, int *decpt, int *sign, char *buf, int max_ndigits, int is_ecvt)
{
  char *s;
  double exp10;
  int tmp_decpt;
  int tmp_sign = 0;
  int must_be_nine = 0;
  if(ndigits < 0) ndigits = 0;
  if(n < 0.0) {
    n = -n;
    tmp_sign = 1;
  }
  if(n >= HUGE_VAL) {
    *decpt = 0;
    *sign = tmp_sign;
    return (char *) "inf";
  }
  exp10 = (n != 0.0 ? floor(log10(n)) : 0.0);
  n /= pow(10.0, exp10);
  if(n >= 10.0) {
    n /= 10.0;
    exp10 += 1.0;
  } else if(n < 1.0 && n > 0.0) {
    n *= 10.0;
    exp10 -= 1.0;
  }
  n += n * DBL_EPSILON; /* correction */
  tmp_decpt = exp10 + 1;
  if(!is_ecvt) {
    if(tmp_decpt > 0 && ndigits > INT_MAX - tmp_decpt) return NULL;
    ndigits += tmp_decpt;
  }
  n += 5.0 * pow(10.0, -ndigits); /* rounding */
  if(n >= 10.0) {
    n /= 10.0;
    if(!is_ecvt) ndigits++;
    tmp_decpt++;
  }
  for(s = buf; max_ndigits > 0 && ndigits > 0; s++, max_ndigits--, ndigits--) {
    if(!must_be_nine) {
      double d = floor(n);
      if(d <= 9.0) {
        int digit = (int) d;
        *s = digit + '0';
        n = (n - d) * 10.0;
      } else {
        *s = '9';
        must_be_nine = 1;
      }
    } else {
      *s = '9';
    }
  }
  *s = '\0';
  *decpt = tmp_decpt;
  *sign = tmp_sign;
  return buf;
}

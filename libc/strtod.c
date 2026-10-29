/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>

double strtod(const char *s, char **endptr)
{
  double n, d;
  double sign = 1.0;
  double exp = 0.0;
  int is_dot = 0;
  int are_only_zeros = 1;
  int is_digit = 0;
  for(; isspace(*s); s++);
  if(*s == '+') {
    s++;
  } else if(*s == '-') {
    sign = -1.0;
    s++;
  }
  for(n = 0.0;; s++) {
    if(isdigit(*s)) {
      int digit = *s - '0';
      if(*s != '0') are_only_zeros = 0;
      if(!is_dot) {
        n = n * 10.0 + digit;
      } else {
        n += digit / d;
        d *= 10.0;
      }
      is_digit = 1;
    } else if(*s == '.') {
      if(is_dot) break;
      is_dot = 1;
      d = 10.0;
    } else {
      break;
    }
  }
  if(is_digit && (*s == 'E' || *s == 'e')) {
    double exp_sign = 1.0;
    const char *t = s + 1;
    if(*t == '+') {
      t++;
    } else if(*s == '-') {
      exp_sign = -1.0;
      t++;
    }
    if(isdigit(*t)) {
      for(s = t; isdigit(*s); s++) {
        exp = exp * 10.0 + (*s - '0');
      }
      exp *= exp_sign;
    }
  }
  if(endptr != NULL) *endptr = (char *) s;
  if(!are_only_zeros) {
    if(n >= HUGE_VAL || n == 0.0) {
      errno = ERANGE;
      return n * sign;
    }
    n *= sign * pow(10.0, exp);
    if(n >= HUGE_VAL || n <= -HUGE_VAL || n == 0.0) {
      errno = ERANGE;
    }
  }
  return n;
}

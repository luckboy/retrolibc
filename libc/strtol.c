/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#ifndef TEST 
#include <stdlib.h>
#else
#define RETROLIBC_STRTOLL
#include "retrolibc.h"
#endif

static unsigned long long strtoull_for_max(const char *s, char **endptr, int base, unsigned long long max, int is_signed)
{
  unsigned long long n, max_div_base, max_mod_base, min;
  int sign = 1;
  int is_overflow = 0;
  if(base != 0 && (base < 2 || base > 36)) {
    errno = EINVAL;
    return 0;
  }
  for(; isspace(*s); s++);
  if(*s == '+') {
    s++;
  } else if(*s == '-') {
    sign = -1;
    s++;
  }
  if(base == 0) {
    if(*s == '0') {
      if((s[1] == 'X' || s[1] == 'x') && isxdigit(s[2])) {
        base = 16;
        s += 2;
      } else {
        base = 8;
      }
    } else {
      base = 10;
    }
  } else {
    if(base == 16) {
      if(*s =='0' && (s[1] == 'X' || s[1] == 'x')) {
        base = 16;
        s += 2;
      }
    }
  }
  max_div_base = ULLONG_MAX / base;
  max_mod_base = ULLONG_MAX % base;
  for(n = 0;; s++) {
    int digit;
    if(*s >= '0' && *s <= (base >= 10 ? '9' : '0' + base - 1)) {
      digit = *s - '0';
    } else if(base > 10) {
      if(*s >= 'A' && *s <= 'A' + base - 11) {
        digit = *s - 'A' + 10;
      } else if(*s >= 'a' && *s <= 'a' + base - 11) {
        digit = *s - 'a' + 10;
      } else {
        break;
      }
    } else {
      break;
    }
    if(is_overflow) continue;
    if(n <= max_div_base && (n != max_div_base || digit <= max_mod_base)) {
      n = n * base + digit;
    } else {
      is_overflow = 1;
    }
  }
  if(endptr != NULL) *endptr = (char *) s;
  min = (is_signed ? (unsigned long long) (-((long long) max) - 1) : max);
  if(!is_overflow) {
    if(sign > 0) {
      if(n > max) {
        errno = ERANGE;
        return max;
      }
    } else {
      if(n > (is_signed ? max + 1 : max)) {
        errno = ERANGE;
        return min;
      }
    }
    return (unsigned long long) (((long long) n) * sign);
  } else {
    errno = ERANGE;
    return (unsigned long long) (sign > 0 ? max : min);
  }
}

long strtol(const char *s, char **endptr, int base)
{ return (long) strtoull_for_max(s, endptr, base, (unsigned long long) LONG_MAX, 1); }

long long strtoll(const char *s, char **endptr, int base)
{ return (long long) strtoull_for_max(s, endptr, base, (unsigned long long) LLONG_MAX, 1); }

unsigned long strtoul(const char *s, char **endptr, int base)
{ return (unsigned long) strtoull_for_max(s, endptr, base, (unsigned long long) ULONG_MAX, 0); }

unsigned long long strtoull(const char *s, char **endptr, int base)
{ return strtoull_for_max(s, endptr, base, ULLONG_MAX, 0); }

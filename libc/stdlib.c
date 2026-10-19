/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <errno.h>
#include <limits.h>
#ifndef TEST
#include <stdlib.h>
#else
#define RETROLIBC_STDLIB
#include "retrolibc.h"
#endif

#ifdef L_atoi
int atoi(const char *s)
{ return (int) strtol(s, NULL, 10); }
#endif

#ifdef L_atol
long atol(const char *s)
{ return strtol(s, NULL, 10); }
#endif

#ifdef L_atoll
long long atoll(const char *s)
{ return strtoll(s, NULL, 10); }
#endif

#ifdef L_strtol
long strtol(const char *s, char **endptr, int base)
{
  long long n = strtoll(s, endptr, base);
  if(n >= LONG_MIN && n <= LONG_MAX) {
    return (long) n;
  } else {
    errno = ERANGE;
    return n > 0 ? LONG_MAX : LONG_MIN;
  }
}
#endif

#ifdef L_strtoul
unsigned long strtoul(const char *s, char **endptr, int base)
{
  unsigned long long n = strtoull(s, endptr, base);
  if(n <= ULONG_MAX) {
    return (unsigned long) n;
  } else {
    errno = ERANGE;
    return ULONG_MAX;
  }
}
#endif

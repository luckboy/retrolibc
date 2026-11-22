/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#ifndef TEST
#include <stdlib.h>
#include "priv_stdlib.h"
#else
#define RETROLIBC_STDLIB
#include "retrolibc.h"
#endif

#ifdef L_atof
double atof(const char *s)
{ return strtod(s, NULL); }
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

#ifdef L_ecvt
char *ecvt(double n, int ndigits, int *decpt, int *sign)
{
  static char buf[DBL_DIG + 1];
  return __efcvt(n, ndigits, decpt, sign, buf, DBL_DIG, 1);
}
#endif

#ifdef L_fcvt
char *fcvt(double n, int ndigits, int *decpt, int *sign)
{
  static char buf[DBL_DIG + 1];
  char *r = __efcvt(n, ndigits, decpt, sign, buf, DBL_DIG, 0);
  if(r != NULL && *decpt < -ndigits) *decpt = -ndigits;
  return r;
}
#endif

#ifdef L_gcvt
char *gcvt(double n, int ndigits, char *buf)
{
  sprintf(buf, "%.*g", ndigits, n);
  return buf;
}
#endif

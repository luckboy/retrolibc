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
#include <string.h>
#ifndef TEST
#include <stdlib.h>
#include "priv_stdlib.h"
#else
#define RETROLIBC_STDLIB
#include "retrolibc.h"
#endif

extern char **environ;

#ifdef L_a64l
long a64l(const char *s)
{
  int n;
  size_t m;
  unsigned shift;
  for(n = 0, m = 6, shift = 0; m > 0 && *s != '\0'; s++, m--, shift += 6) {
    int digit;
    if(*s == '.') {
      digit = 0;
    } else if(*s == '/') {
      digit = 1;
    } else if(*s >= '0' && *s <= '9') {
      digit = *s - '0' + 2;
    } else if(*s >= 'A' && *s <= 'Z') {
      digit = *s - 'A' + 12;
    } else if(*s >= 'a' && *s <= 'z') {
      digit = *s - 'a' + 38;
    } else {
      break;
    }
    n += digit << shift;
  }
  return n;
}
#endif

#ifdef L_abs
int abs(int x)
{ return x >= 0 ? x : -x; }
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

#ifdef L_div
div_t div(int n, int d)
{
  div_t r;
  r.quot = n / d;
  r.rem = n % d;
  return r;
}
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

#ifdef L_getenv
char *getenv(const char *name)
{
  char **p;
  size_t len = strlen(name);
  for(p = environ; *p != NULL; p++) {
    char *s = *p;
    if(strncmp(name, s, len) == 0) return s[len] == '=' ? s + len + 1 : NULL;
  }
  return NULL;
}
#endif

#ifdef L_l64a 
char *l64a(long n)
{
  static char buf[7];
  char *s;
  int m = (int) n;
  size_t l;
  for(s = buf, l = 6; l > 0 && m != 0; s++, m >>= 6, l--) {
    int digit = m & 63;
    if(digit == 0) {
      *s = '.';
    } else if(digit == 1) {
      *s = '/';
    } else if(digit >= 2 && digit <= 11) {
      *s = digit - 2 + '0';
    } else if(digit >= 12 && digit <= 37) {
      *s = digit - 12 + 'A';
    } else {
      *s = digit - 38 + 'a';
    }
  }
  *s = '\0';
  return buf;
}
#endif

#ifdef L_labs
long labs(long x)
{ return x >= 0 ? x : -x; }
#endif

#ifdef L_ldiv
ldiv_t ldiv(long n, long d)
{
  ldiv_t r;
  r.quot = n / d;
  r.rem = n % d;
  return r;
}
#endif

#ifdef L_llabs
long long llabs(long long x)
{ return x >= 0 ? x : -x; }
#endif

#ifdef L_lldiv
lldiv_t lldiv(long long n, long long d)
{
  lldiv_t r;
  r.quot = n / d;
  r.rem = n % d;
  return r;
}
#endif

#ifdef L_putenv
int putenv(const char *s)
{
  static char **allocated_environ = NULL;
  char **p;
  char *new_s;
  size_t name_len = strcspn(s, "=");
  size_t n;
  for(p = environ, n = 0; *p != NULL; p++, n++) {
    char *old_s = *p;
    if(strncmp(s, old_s, name_len) == 0) {
      if(s[name_len] == '=') {
        if(strcmp(s, old_s) != 0) {
          new_s = strdup(s);
          if(new_s == NULL) return -1;
          *p = new_s;
        }
      } else {
        for(; (*p = p[1]) != NULL; p++);
      }
      return 0;
    }
  }
  if(s[name_len] != '=') return 0;
  new_s = strdup(s);
  if(new_s == NULL) return -1;
  if(allocated_environ == environ) {
    p = realloc(environ, (n + 2) * sizeof(char *));
    if(p == NULL) {
      free(new_s);
      return -1;
    }
  } else {
    p = malloc((n + 2) * sizeof(char *));
    if(p == NULL) {
      free(new_s);
      return -1;
    }
    memcpy(p, environ, n);
  }
  environ = allocated_environ = p;
  environ[n] = new_s;
  environ[n + 1] = NULL;
  return 0;
}
#endif

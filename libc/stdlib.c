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
        size_t m;
        for(m = 0; *p != NULL; p++, m++);
        memmove(p, p + 1, m);
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

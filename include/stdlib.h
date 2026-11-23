/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _STDLIB_H
#define _STDLIB_H

#include <bits/wait.h>
#include <stddef.h>

typedef struct
{
  int quot;
  int rem;
} div_t;

typedef struct
{
  long quot;
  long rem;
} ldiv_t;

typedef struct
{
  long long quot;
  long long rem;
} lldiv_t;

#ifdef __cplusplus
extern "C" {
#endif

long a64l(const char *s);
int abs(int x);
double atof(const char *s);
int atoi(const char *s);
long atol(const char *s);
long long atoll(const char *s);
div_t div(int n, int d);
char *ecvt(double n, int ndigits, int *decpt, int *sign);
char *fcvt(double n, int ndigits, int *decpt, int *sign);
char *gcvt(double n, int ndigits, char *buf);
char *getenv(const char *name);
char *l64a(long n);
long labs(long x);
ldiv_t ldiv(long n, long d);
long long llabs(long long x);
lldiv_t lldiv(long long n, long long d);
int putenv(const char *s);

char *getenv(const char *name);

void *bsearch(const void *key, const void *base, size_t n, size_t width, int (*cmp)(const void *, const void *));
void qsort(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *));

double strtod(const char *s, char **endptr);
long strtol(const char *s, char **endptr, int base);
long long strtoll(const char *s, char **endptr, int base);
unsigned long strtoul(const char *s, char **endptr, int base);
unsigned long long strtoull(const char *s, char **endptr, int base);

void *malloc(size_t size);
void *realloc(void *p, size_t size);
void free(void *p);
void *valloc(size_t size);

#ifdef __cplusplus
}
#endif

#endif

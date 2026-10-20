/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _STDLIB_H
#define _STDLIB_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int atoi(const char *s);
long atol(const char *s);
long long atoll(const char *s);
void *bsearch(const void *key, const void *base, size_t n, size_t width, int (*cmp)(const void *, const void *));
void qsort(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *));
long strtol(const char *s, char **endptr, int base);
long long strtoll(const char *s, char **endptr, int base);
unsigned long strtoul(const char *s, char **endptr, int base);
unsigned long long strtoull(const char *s, char **endptr, int base);

void *malloc(size_t size);
void *realloc(void *p, size_t size);
void free(void *p);

#ifdef __cplusplus
}
#endif

#endif

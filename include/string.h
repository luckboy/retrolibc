/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

char *strcat(char *d, const char *s);
char *strchr(const char *s, int c);
int strcmp(const char *s, const char *s2);
int strcoll(const char *s, const char *s2);
char *strcpy(char *d, const char *s);
size_t strcspn(const char *s, const char *r);
size_t strlen(const char *s);
char *strncat(char *d, const char *s, size_t n);
int strncmp(const char *s, const char *s2, size_t n);
char *strncpy(char *d, const char *s, size_t n);
char *strpbrk(const char *s, const char *a);
char *strrchr(const char *s, int c);
size_t strspn(const char *s, const char *a);
char *strstr(const char *s, const char *s2);

#ifdef __cplusplus
}
#endif

#endif

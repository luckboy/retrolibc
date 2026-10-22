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

void *memccpy(void *d, const void *s, int c, size_t n);
void *memchr(const void *s, int c, size_t n);
int memcmp(const void *s, const void *s2, size_t n);
void *memcpy(void *d, const void *s, size_t n);
void *memmove(void *d, const void *s, size_t n);
void *memset(void *d, int c, size_t n);
char *strcat(char *d, const char *s);
char *strchr(const char *s, int c);
int strcmp(const char *s, const char *s2);
int strcoll(const char *s, const char *s2);
char *strcpy(char *d, const char *s);
size_t strcspn(const char *s, const char *r);
char *strdup(const char *s);
char *strerror(int errnum);
size_t strlen(const char *s);
char *strncat(char *d, const char *s, size_t n);
int strncmp(const char *s, const char *s2, size_t n);
char *strncpy(char *d, const char *s, size_t n);
char *strpbrk(const char *s, const char *a);
char *strrchr(const char *s, int c);
size_t strspn(const char *s, const char *a);
char *strstr(const char *s, const char *s2);
char *strtok(char *s, const char *sep);
char *strtok_r(char *s, const char *sep, char **lasts);
size_t strxfrm(char *d, const char *s2, size_t n);

#ifdef __cplusplus
}
#endif

#endif

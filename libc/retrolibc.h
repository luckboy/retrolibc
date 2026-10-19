/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _RETROLIBC_H
#define _RETROLIBC_H

#include <stddef.h>
#include <stdlib.h>

/* ctype.c */

int retrolibc_isalnum(int c);
int retrolibc_isalpha(int c);
int retrolibc_isascii(int c);
int retrolibc_iscntrl(int c);
int retrolibc_isdigit(int c);
int retrolibc_isgraph(int c);
int retrolibc_islower(int c);
int retrolibc_isprint(int c);
int retrolibc_ispunct(int c);
int retrolibc_isspace(int c);
int retrolibc_isupper(int c);
int retrolibc_isxdigit(int c);
int retrolibc_toascii(int c);
int retrolibc_tolower(int c);
int retrolibc_toupper(int c);

#ifdef RETROLIBC_CTYPE
#define isalnum retrolibc_isalnum
#define isalpha retrolibc_isalpha
#define isascii retrolibc_isascii
#define iscntrl retrolibc_iscntrl
#define isdigit retrolibc_isdigit
#define isgraph retrolibc_isgraph
#define islower retrolibc_islower
#define isprint retrolibc_isprint
#define ispunct retrolibc_ispunct
#define isspace retrolibc_isspace
#define isupper retrolibc_isupper
#define isxdigit retrolibc_isxdigit
#define toascii retrolibc_toascii
#define tolower retrolibc_tolower
#define toupper retrolibc_toupper
#endif

/* stdlib.c */

int retrolibc_atoi(const char *s);
long retrolibc_atol(const char *s);
long long retrolibc_atoll(const char *s);
long retrolibc_strtol(const char *s, char **endptr, int base);
unsigned long retrolibc_strtoul(const char *s, char **endptr, int base);

#ifdef RETROLIBC_STDLIB
#define atoi retrolibc_atoi
#define atol retrolibc_atol
#define atoll retrolibc_atoll
#define strtol retrolibc_strtol
#define strtoul retrolibc_strtoul
#endif

/* string.c */

void *retrolibc_memccpy(void *d, const void *s, int c, size_t n);
void *retrolibc_memchr(const void *s, int c, size_t n);
int retrolibc_memcmp(const void *s, const void *s2, size_t n);
void *retrolibc_memcpy(void *d, const void *s, size_t n);
void *retrolibc_memmove(void *d, const void *s, size_t n);
void *retrolibc_memset(void *d, int c, size_t n);
char *retrolibc_strcat(char *d, const char *s);
char *retrolibc_strchr(const char *s, int c);
int retrolibc_strcmp(const char *s, const char *s2);
int retrolibc_strcoll(const char *s, const char *s2);
char *retrolibc_strcpy(char *d, const char *s);
size_t retrolibc_strcspn(const char *s, const char *r);
size_t retrolibc_strlen(const char *s);
char *retrolibc_strncat(char *d, const char *s, size_t n);
int retrolibc_strncmp(const char *s, const char *s2, size_t n);
char *retrolibc_strncpy(char *d, const char *s, size_t n);
char *retrolibc_strpbrk(const char *s, const char *a);
char *retrolibc_strrchr(const char *s, int c);
size_t retrolibc_strspn(const char *s, const char *a);
char *retrolibc_strstr(const char *s, const char *s2);
char *retrolibc_strtok(char *s, const char *sep);
char *retrolibc_strtok_r(char *s, const char *sep, char **lasts);
size_t retrolibc_strxfrm(char *d, const char *s2, size_t n);

#ifdef RETROLIBC_STRING
#define memccpy retrolibc_memccpy
#define memccpy retrolibc_memccpy
#define memchr retrolibc_memchr
#define memcmp retrolibc_memcmp
#define memcpy retrolibc_memcpy
#define memmove retrolibc_memmove
#define memset retrolibc_memset
#define strcat retrolibc_strcat
#define strchr retrolibc_strchr
#define strcmp retrolibc_strcmp
#define strcoll retrolibc_strcoll
#define strcpy retrolibc_strcpy
#define strcspn retrolibc_strcspn
#define strlen retrolibc_strlen
#define strncat retrolibc_strncat
#define strncmp retrolibc_strncmp
#define strncpy retrolibc_strncpy
#define strpbrk retrolibc_strpbrk
#define strrchr retrolibc_strrchr
#define strspn retrolibc_strspn
#define strstr retrolibc_strstr
#define strtok retrolibc_strtok
#define strtok_r retrolibc_strtok_r
#define strxfrm retrolibc_strxfrm
#endif

/* bsearch.c */

void *retrolibc_bsearch(const void *key, const void *base, size_t n, size_t width, int (*cmp)(const void *, const void *));

#ifdef RETROLIBC_BSEARCH
#define bsearch retrolibc_bsearch
#endif

/* strtoll.c */

long long retrolibc_strtoll(const char *s, char **endptr, int base);
unsigned long long retrolibc_strtoull(const char *s, char **endptr, int base);

#ifdef RETROLIBC_STRTOLL
#define strtoll retrolibc_strtoll
#define strtoull retrolibc_strtoull
#endif

/* qsort.c */

void retrolibc_qsort(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *));

#ifdef RETROLIBC_QSORT
#define qsort retrolibc_qsort
#endif

#endif

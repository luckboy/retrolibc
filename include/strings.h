/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _STRINGS_H
#define _STRINGS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int bcmp(const void *s, const void *s2, size_t n);
void bcopy(const void *s, void *d, size_t n);
void bzero(void *d, size_t n);
int ffs(int i);
char *index(const char *s, int c);
char *rindex(const char *s, int c);
int strcasecmp(const char *s, const char *s2);
int strncasecmp(const char *s, const char *s2, size_t n);

#ifdef __cplusplus
extern "C" {
#endif

#endif

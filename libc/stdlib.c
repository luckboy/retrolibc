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

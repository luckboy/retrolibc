/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _UNISTD_H
#define _UNISTD_H

#include <bits/long_bit.h>

#if __LONG_BIT == 64
#ifndef _INTPTR_T
#define _INTPTR_T
typedef long intptr_t;
#endif
#else
#ifndef _INTPTR_T
#define _INTPTR_T
typedef int intptr_t;
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

int brk(void *addr);
void *sbrk(intptr_t inc);

#ifdef __cplusplus
}
#endif

#endif

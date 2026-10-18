/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _INTTYPES_H
#define _INTTYPES_H

#include <bits/long_bit.h>

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
#if __LONG_BIT == 64
typedef long int64_t;
#else
typedef long long int64_t;
#endif

typedef unsigned char int8_t;
typedef unsigned short int16_t;
typedef unsigned int32_t;
#if __LONG_BIT == 64
typedef unsigned long int64_t;
#else
typedef unsigned long long int64_t;
#endif

#if __LONG_BIT == 64
#ifndef _INTPTR_T
#define _INTPTR_T
typedef long intptr_t;
#endif
typedef unsigned long uintptr_t;
#else
#ifndef _INTPTR_T
#define _INTPTR_T
typedef int intptr_t;
#endif
typedef unsigned uintptr_t;
#endif

#endif

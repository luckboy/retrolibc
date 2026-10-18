/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _LIMITS_H
#define _LIMITS_H

#include <bits/long_bit.h>

#define CHAR_BIT        8
#define LONG_BIT        __LONG_BIT
#define WORD_BIT        32

#define SCHAR_MAX       127
#define SCHAR_MIN       (-SCHAR_MAX - 1)
#define SHRT_MAX        32767
#define SHRT_MIN        (-SHRT_MAX - 1)
#define INT_MAX         2147483647
#define INT_MIN         (-INT_MAX - 1)
#if __LONG_BIT == 64
#define LONG_MAX        9223372036854775807L
#else
#define LONG_MAX        2147483647L
#endif
#define LONG_MIN        (-LONG_MAX - 1L)
#define LLONG_MAX       9223372036854775807LL
#define LLONG_MIN       (-LLONG_MAX - 1LL)

#define UCHAR_MAX       255
#define USHRT_MAX       65535
#define UINT_MAX        4294967295U
#if __LONG_BIT == 64
#define ULONG_MAX       18446744073709551615UL
#else
#define ULONG_MAX       4294967295UL
#endif
#define ULLONG_MAX      18446744073709551615ULL

#ifdef __CHAR_UNSIGNED__
#define CHAR_MAX        UCHAR_MAX
#define CHAR_MIN        0
#else
#define CHAR_MAX        SCHAR_MAX
#define CHAR_MIN        SCHAR_MIN
#endif

#endif

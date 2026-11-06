/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _BITS_ARCH_TYPES_H
#define _BITS_ARCH_TYPES_H

#include <bits/config.h>

typedef unsigned long long __blkcnt_t;
#if __LINUX_VERSION_CODE >= __KERNEL_VERSION(2, 6, 0)
typedef unsigned long long __dev_t;
#else
typedef unsigned short __dev_t;
#endif
typedef unsigned long long __fsblkcnt_t;
typedef unsigned long long __fsfilcnt_t;
typedef unsigned __nlink_t;
typedef long long __off_t;
typedef unsigned long long __ino_t;

#endif

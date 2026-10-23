/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H

#include <bits/long_bit.h>
#include <bits/types.h>

#ifndef _BLKCNT_T
#define _BLKCNT_T
typedef __blkcnt_t blkcnt_t;
#endif

#ifndef _BLKSIZE_T
#define _BLKSIZE_T
typedef __blksize_t blksize_t;
#endif

#ifndef _CLOCK_T
#define _CLOCK_T
typedef long clock_t;
#endif

#ifndef _DEV_T
#define _DEV_T
typedef __dev_t dev_t;
#endif

#ifndef _FSBLKCNT_T
#define _FSBLKCNT_T
typedef __fsblkcnt_t fsblkcnt_t;
#endif

#ifndef _FSFILCNT_T
#define _FSFILCNT_T
typedef __fsfilcnt_t fsfilcnt_t;
#endif

#ifndef _GID_T
#define _GID_T
typedef __gid_t gid_t;
#endif

#ifndef _ID_T
#define _ID_T
typedef __id_t id_t;
#endif

#ifndef _INO_T
#define _INO_T
typedef __ino_t ino_t;
#endif

#ifndef _KEY_T
#define _KEY_T
typedef __key_t key_t;
#endif

#ifndef _MODE_T
#define _MODE_T
typedef __mode_t mode_t;
#endif

#ifndef _NLINK_T
#define _NLINK_T
typedef __nlink_t nlink_t;
#endif

#ifndef _OFF_T
#define _OFF_T
typedef __off_t off_t;
#endif

#ifndef _PID_T
#define _PID_T
typedef __pid_t pid_t;
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_TYPE__ size_t;
#endif

#ifndef _SSIZE_T
#define _SSIZE_T
#if __LONG_BIT == 64
typedef long ssize_t;
#else
typedef int ssize_t;
#endif
#endif

#ifndef _SUSECONDS_T
#define _SUSECONDS_T
typedef __suseconds_t suseconds_t;
#endif

#ifndef _TIME_T
#define _TIME_T
typedef __time_t time_t;
#endif

#ifndef _UID_T
#define _UID_T
typedef __uid_t uid_t;
#endif

#ifndef _USECONDS_T
#define _USECONDS_T
typedef __useconds_t useconds_t;
#endif

#endif

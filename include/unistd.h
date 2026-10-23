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
#include <bits/types.h>

#ifndef SEEK_SET
#define SEEK_SET        0
#define SEEK_CUR        1
#define SEEK_END        2
#endif

#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_TYPE__ size_t;
#endif

#ifndef _SSIZE_T
#define _SSIZE_T
#if LONG_BIT == 64
typedef long ssize_t;
#else
typedef int ssize_t;
#endif
#endif

#ifndef _UID_T
#define _UID_T
typedef __uid_t uid_t;
#endif

#ifndef _GID_T
#define _GID_T
typedef __gid_t gid_t;
#endif

#ifndef _OFF_T
#define _OFF_T
typedef __off_t off_t;
#endif

#ifndef _PID_T
#define _PID_T
typedef __pid_t pid_t;
#endif

#ifndef _INTPTR_T
#define _INTPTR_T
#if __LONG_BIT == 64
typedef long intptr_t;
#else
typedef int intptr_t;
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

int brk(void *addr);
off_t lseek(int fd, off_t offset, int whence);
ssize_t read(int fd, void *buf, size_t size);
void *sbrk(intptr_t inc);
ssize_t write(int fd, const void *buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif

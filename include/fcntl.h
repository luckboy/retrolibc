/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _FCNTL_H
#define _FCNTL_H

#include <bits/fcntl.h>
#include <bits/mode.h>
#include <bits/types.h>

#ifndef _OFF_T
#define _OFF_T
typedef __off_t off_t;
#endif

#ifndef _PID_T
#define _PID_T
typedef __pid_t pid_t;
#endif

#ifndef _MODE_T
#define _MODE_T
typedef __mode_t mode_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

int creat(const char *path, mode_t mode);
int fcntl(int fd, int cmd, ...);
int open(const char *path, int oflags, ...);

#ifdef __cplusplus
}
#endif

#endif

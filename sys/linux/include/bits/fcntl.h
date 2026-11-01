/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _BITS_FCNTL_H
#define _BITS_FCNTL_H

#include <bits/config.h>
#include <bits/long_bit.h>
#include <bits/types.h>

#define O_ACCMODE       3
#define O_RDONLY        0
#define O_WRONLY        1
#define O_RDWR          2
#define O_CREAT         000000100
#define O_EXCL          000000200
#define O_NOCTTY        000000400
#define O_TRUNC         000001000
#define O_APPEND        000002000
#define O_NONBLOCK      000004000
#if __LINUX_VERSION_CODE >= __KERNEL_VERSION(2, 6, 33)
#define O_DSYNC         000010000
#else
#define O_SYNC          000010000
#endif
#define O_ASYNC         000020000
#define O_DIRECT        000040000
#define O_LARGEFILE     000100000
#define O_DIRECTORY     000200000
#define O_NOFOLLOW      000400000
#define O_NOATIME       001000000
#define O_CLOEXEC       002000000
#define __O_SYNC        004000000
#define O_PATH          010000000
#define __O_TMPFILE     020000000

#if __LINUX_VERSION_CODE >= __KERNEL_VERSION(2, 6, 33)
#define O_SYNC          (__O_SYNC | O_DSYNC)
#define O_RSYNC         O_SYNC
#else
#define O_RSYNC         O_SYNC
#define O_DSYNC         O_SYNC
#endif
#define O_TMPFILE       (__O_TMPFILE | O_DIRECTORY)
#define O_NDELAY        O_NONBLOCK

#define F_DUPFD         0
#define F_GETFD         1
#define F_SETFD         2
#define F_GETFL         3
#define F_SETFL         4
#if __LONG_BIT == 64
#define F_GETLK         12
#define F_SETLK         13
#define F_SETLKW        14
#else
#define F_GETLK         5
#define F_SETLK         6
#define F_SETLKW        7
#endif
#define F_GETOWN        8
#define F_SETOWN        9
#define F_GETSIG        10
#define F_SETSIG        11
#define F_GETOWN_EX     15
#define F_SETOWN_EX     16
#define F_GETOWNER_UIDS 17
#define F_OFD_GETLK     36
#define F_OFD_SETLK     37
#define F_OFD_SETLKW    38

struct flock
{
  short l_type;
  short l_whence;
  __off_t l_start;
  __off_t l_len;
  __pid_t l_pid;
};

#define F_OWNER_TID     0
#define F_OWNER_PID     1
#define F_OWNER_PGRP    2

struct f_owner
{
  int type;
  __pid_t pid;
};

#define F_RDLCK         0
#define F_WRLCK         1
#define F_UNLCK         2

#define FD_CLOEXEC      1

#endif

/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _BITS_ARCH_STAT_H
#define _BITS_ARCH_STAT_H

#include <bits/types.h>

struct stat
{
  __dev_t st_dev;
  unsigned char __pad0[4];
  unsigned int __st_ino;
  __mode_t st_mode;
  __nlink_t st_nlink;
  __uid_t st_uid;
  __gid_t st_gid;
  __dev_t st_rdev;
  unsigned char __pad3[4];
  __off_t st_size;
  __blksize_t st_blksize;
  __blkcnt_t st_blocks;
  __time_t st_atime;
  unsigned long st_atimensec;
  __time_t st_mtime;
  unsigned long st_mtimensec;
  __time_t st_ctime;
  unsigned long st_ctimensec;
  __ino_t st_ino;
};

#endif

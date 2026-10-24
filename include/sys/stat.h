/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _SYS_STAT_H
#define _SYS_STAT_H

#include <bits/stat.h>
#include <bits/types.h>

#ifdef S_IFBLK
#define S_ISBLK(m)      (((m) & S_IFMT) == S_IFBLK)
#endif

#ifdef S_IFCHR
#define S_ISCHR(m)      (((m) & S_IFMT) == S_IFCHR)
#endif

#ifdef S_IFDIR
#define S_ISDIR(m)      (((m) & S_IFMT) == S_IFDIR)
#endif

#ifdef S_IFIFO
#define S_ISFIFO(m)     (((m) & S_IFMT) == S_IFIFO)
#endif

#ifdef S_IFREG
#define S_ISREG(m)      (((m) & S_IFMT) == S_IFREG)
#endif

#ifdef S_IFLNK
#define S_ISLNK(m)      (((m) & S_IFMT) == S_IFLNK)
#endif

#ifdef S_IFSOCK
#define S_ISSOCK(m)     (((m) & S_IFMT) == S_IFSOCK)
#endif

#ifndef _BLKCNT_T
#define _BLKCNT_T
typedef __blkcnt_t blkcnt_t;
#endif

#ifndef _BLKSIZE_T
#define _BLKSIZE_T
typedef __blksize_t blksize_t;
#endif

#ifndef _DEV_T
#define _DEV_T
typedef __dev_t dev_t;
#endif

#ifndef _INO_T
#define _INO_T
typedef __ino_t ino_t;
#endif

#ifndef _MODE_T
#define _MODE_T
typedef __mode_t mode_t;
#endif

#ifndef _NLINK_T
#define _NLINK_T
typedef __nlink_t nlink_t;
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

#ifndef _TIME_T
#define _TIME_T
typedef __time_t time_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

int chmod(const char *path, mode_t mode);
int fchmod(int fd, mode_t mode);
int fstat(int fd, struct stat *buf);
int lstat(const char *path, struct stat *buf);
int mkdir(const char *path, mode_t mode);
int mkfifo(const char *path, mode_t mode);
int mknod(const char *path, mode_t mode, dev_t dev);
int stat(const char *path, struct stat *buf);
mode_t umask(mode_t cmask);

#ifdef __cplusplus
}
#endif

#endif

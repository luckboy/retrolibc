/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _UNISTD_H
#define _UNISTD_H

#include <bits/access.h>
#include <bits/long_bit.h>
#include <bits/seek.h>
#include <bits/types.h>

#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

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

#ifndef _USECONDS_T
#define _USECONDS_T
typedef __useconds_t useconds_t;
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

int execv(const char *path, char *const argv[]);
void *sbrk(intptr_t inc);

void _exit(int status);
int access(const char *path, int amode);
int brk(void *addr);
int chdir(const char *path);
int chown(const char *path, uid_t owner, gid_t group);
int close(int fd);
int dup(int fd);
int dup2(int fd, int fd2);
int execve(const char *path, char *const argv[], char *const envp[]);
int fchown(int fd, uid_t owner, gid_t group);
int fchdir(int fd);
int fdatasync(int fd);
pid_t fork(void);
int fsync(int fd);
int ftruncate(int fd, off_t length);
char *getcwd(char *buf, size_t size);
gid_t getegid(void);
uid_t geteuid(void);
gid_t getgid(void);
int getgroups(int size, gid_t list[]);
pid_t getpgid(pid_t pid);
pid_t getpgrp(void);
pid_t getpid(void);
pid_t getppid(void);
pid_t getsid(pid_t pid);
uid_t getuid(void);
int lchown(const char *path, uid_t owner, gid_t group);
int link(const char *path1, const char *path2);
off_t lseek(int fd, off_t offset, int whence);
int nice(int inc);
ssize_t read(int fd, void *buf, size_t size);
int pause(void);
int pipe(int fds[2]);
int readlink(const char *path, char *buf, size_t size);
int rmdir(const char *path);
int setgid(gid_t gid);
int setpgid(pid_t pid, pid_t pgid);
pid_t setpgrp(void);
int setregid(gid_t rgid, gid_t egid);
int setreuid(uid_t ruid, uid_t euid);
pid_t setsid(void);
int setuid(uid_t uid);
unsigned int sleep(unsigned int seconds);
int symlink(const char *path1, const char *path2);
void sync(void);
int truncate(const char *path, off_t length);
int unlink(const char *path);
int usleep(useconds_t useconds);
pid_t vfork(void);
ssize_t write(int fd, const void *buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif

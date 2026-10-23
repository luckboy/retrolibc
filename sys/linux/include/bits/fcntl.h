/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _BITS_FCNTL_H
#define _BITS_FCNTL_H

#define O_ACCMODE       3
#define O_RDONLY        0
#define O_WRONLY        1
#define O_RDWR          2
#define O_CREAT         00000100
#define O_EXCL          00000200
#define O_NOCTTY        00000400
#define O_TRUNC         00001000
#define O_APPEND        00002000
#define O_NONBLOCK      00004000
#define O_SYNC          00010000
#define O_ASYNC         00020000
#define O_DIRECT        00040000
#define O_LARGEFILE     00100000
#define O_DIRECRTORY    00200000
#define O_NOFOLLOW      00400000
#define O_NOATIME       01000000
#define O_CLOEXEC       02000000

#endif

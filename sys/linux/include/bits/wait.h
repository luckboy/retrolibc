/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _BITS_WAIT_H
#define _BITS_WAIT_H

#define WNOHANG         1
#define WUNTRACED       2

#define __WNOTHREAD     0x20000000
#define __WALL          0x40000000
#define __WCLONE        0x80000000

#define WEXITSTATUS(s)  (((s) & 0xff00) >> 8)
#define WTERMSIG(s)     ((s) & 0x7f)
#define WSTOPSIG(s)     WEXITSTATUS((s))

#define WIFEXITED(s)    (WTERMSIG((s) == 0)
#define WIFSIGNALED(s)  (WTERMSIG(s) != 0 && ((s) & 0xff) != 0x7f)
#define WIFSTOPPED(s)   (((s) & 0xff) == 0x7f)
#define WIFCONTINUED(s) ((s) == 0xffff)

#endif

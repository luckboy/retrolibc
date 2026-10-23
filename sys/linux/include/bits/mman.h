/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _BITS_MMAN_H
#define _BITS_MMAN_H

#define PROT_READ       0x1
#define PROT_WRITE      0x2
#define PROT_EXEC       0x4
#define PROT_NONE       0x0

#define MAP_SHARED      0x01
#define MAP_PRIVATE     0x02
#define MAP_FIXED       0x10
#define MAP_ANONYMOUS   0x20
#define MAP_GROWSDOWN   0x00100
#define MAP_DENYWRITE   0x00800
#define MAP_EXECUTABLE  0x01000
#define MAP_LOCKED      0x02000
#define MAP_NORESERVE   0x04000
#define MAP_POPULATE    0x08000
#define MAP_NONBLOCK    0x10000
#define MAP_STACK       0x20000
#define MAP_HUGETLB     0x40000

#define MAP_ANON        MAP_ANONYMOUS

#define MREMAP_MAYMOVE  1
#define MREMAP_FIXED    2

#define MS_ASYNC        1
#define MS_INVALIDATE   2
#define MS_SYNC         4


#define MCL_CURRENT     1
#define MCL_FUTURE      2

#define MAP_FAILED      ((void *) (-1))

#endif

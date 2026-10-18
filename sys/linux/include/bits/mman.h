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
#define MAP_ANON        MAP_ANONYMOUS

#define MREMAP_MAYMOVE  1
#define MREMAP_FIXED    2

#define MAP_FAILED      ((void *) (-1))

#if __LONG_BIT == 64
#ifndef _OFF_T
#define _OFF_T
typedef long off_t;
#endif
#else
#ifndef _OFF_T
#define _OFF_T
typedef long long off_t;
#endif
#endif

#endif

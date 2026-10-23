/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _BITS_STAT_H
#define _BITS_STAT_H

#include <bits/arch_stat.h>
#include <bits/mode.h>

#define S_IFMT          0170000
#define S_IFMSOCK       0140000
#define S_IFLNK         0120000
#define S_IFREG         0100000
#define S_IFBLK         0060000
#define S_IFDIR         0040000
#define S_IFCHR         0020000
#define S_IFIFO         0010000

#endif

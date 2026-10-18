/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _SYS_MMAN_H
#define _SYS_MMAN_H

#include <bits/long_bit.h>
#include <bits/mman.h>

#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_TYPE__ size_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t);
int munmap(void *addr, size_t len);

#ifdef __cplusplus
}
#endif

#endif

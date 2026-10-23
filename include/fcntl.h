/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _FCNTL_H
#define _FCNTL_H

#include <bits/fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

int open(const char *path, int oflags, ...);

#ifdef __cplusplus
}
#endif

#endif

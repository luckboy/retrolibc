/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _ERRNO_H
#define _ERRNO_H

#include <bits/errno.h>

#ifdef __cplusplus
extern "C" {
#endif

int *__errno_location(void);

#define errno           (*__errno_location())

extern int sys_nerr;
extern const char *sys_errlist[];

#ifdef __cplusplus
}
#endif

#endif

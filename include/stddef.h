/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _STDDEF_H
#define _STDDEF_H

#ifdef __cplusplus
#define NULL            0
#else
#define NULL            ((void *) 0)
#endif

#define offsetof(t, m)  __builtin_offsetof(t, m)

typedef __PTRDIFF_TYPE__ ptrdiff_t;
#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_TYPE__ size_t;
#endif

#endif

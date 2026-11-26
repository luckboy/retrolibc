/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _FNMATCH_H
#define _FNMATCH_H

#define FNM_NOMATCH     1
#define FNM_NOSYS       2

#define FNM_PATHNAME    (1 << 0)
#define FNM_PERIOD      (1 << 1)
#define FNM_NOESCAPE    (1 << 2)

#ifdef __cplusplus
extern "C" {
#endif

int fnmatch(const char *pattern, const char *s, int flags);

#ifdef __cplusplus
}
#endif

#endif

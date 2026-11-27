/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _PRIV_FNMATCH_H
#define _PRIV_FNMATCH_H

const char *__glob_fnmatch(const char *pattern, const char *s, int flags, int is_first_char);

#endif

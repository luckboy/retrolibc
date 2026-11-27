/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <stddef.h>
#ifndef TEST
#include <fnmatch.h>
#include "priv_fnmatch.h"
#else
#define RETROLIBC_FNMATCH
#include "retrolibc.h"
#endif

int fnmatch(const char *pattern, const char *s, int flags)
{
  const char *p = __fnmatch_glob(pattern, s, flags, 1);
  return (p !=  NULL && *p == '\0') ? 0 : FNM_NOMATCH;
}

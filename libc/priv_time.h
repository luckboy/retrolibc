/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _PRIV_FILE_H
#define _PRIV_FILE_H

#include <time.h>

time_t __mktime_no_tzset(struct tm *tm);

#endif

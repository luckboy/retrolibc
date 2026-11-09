/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pthread.h>
#include <string.h>
#include <time.h>
#include "priv_tzset.h"

pthread_mutex_t __tz_mutex = PTHREAD_MUTEX_INITIALIZER;
long __tz_std_offset = 0L;
const char *__tz_std_zone = NULL;
long __tz_dst_offset = 0L;
const char *__tz_dst_zone = NULL;
int __are_tz_rules = 0;
struct tz_rule __tz_rule_std_to_dst;
struct tz_rule __tz_rule_dst_to_std;

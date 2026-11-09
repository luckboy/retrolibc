/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _PRIV_LOCALTIME_R_H
#define _PRIV_LOCALTIME_R_H

#include <pthread.h>
#include <time.h>
#include "priv_gmtime_r.h"

extern pthread_mutex_t __tz_mutex;
extern long __tz_std_offest;
extern const char *__tz_std_zone;
extern long __tz_dst_offest;
extern const char * __tz_dst_zone;
extern int __are_tz_rules;
extern struct tz_rule __tz_rule_std_to_dst;
extern struct tz_rule __tz_rule_dst_to_std;

void __tzset_unlocked(void);

#endif

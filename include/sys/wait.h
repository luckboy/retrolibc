/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H

#include <bits/types.h>
#include <bits/wait.h>

#ifndef _PID_T
#define _PID_T
typedef __pid_t pid_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

pid_t wait(int *wstatus);
pid_t waitpid(pid_t pid, int *wstatus, int options);

#ifdef __cplusplus
}
#endif

#endif

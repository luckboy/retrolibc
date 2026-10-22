/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _PTHREAD_H
#define _PTHREAD_H

#include <bits/types.h>

#define PTHREAD_MUTEX_INITIALIZER ((pthread_mutex_t) { 1, 0 })

typedef struct {
  int __bit;
  __pid_t __owner;
} pthread_mutex_t;

#ifdef __cplusplus
extern "C" {
#endif

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

#ifdef __cplusplus
extern "C" {
#endif

#endif

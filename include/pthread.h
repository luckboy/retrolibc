/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _PTHREAD_H
#define _PTHREAD_H

#define PTHREAD_MUTEX_INITIALIZER ((pthread_mutex_t) { 1 })

typedef struct {
  long __bit;
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

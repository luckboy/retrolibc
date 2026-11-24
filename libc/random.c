/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t random_mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned random_seed = 1;

long random(void)
{
  long r;
  pthread_mutex_lock(&random_mutex);
  r = rand_r(&random_seed);
  pthread_mutex_unlock(&random_mutex);
  return r;
}

void srandom(unsigned seed)
{
  pthread_mutex_lock(&random_mutex);
  random_seed = seed;
  pthread_mutex_unlock(&random_mutex);
}

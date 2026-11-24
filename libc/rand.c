/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <stdlib.h>

static unsigned rand_seed = 1;

int rand(void)
{ return rand_r(&rand_seed); }

void srand(unsigned seed)
{ rand_seed = seed; }

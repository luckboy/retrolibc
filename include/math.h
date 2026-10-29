/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _MATH_H
#define _MATH_H

#define HUGE_VAL        __builtin_inf()

#ifdef __cplusplus
extern "C" {
#endif

double log10(double x);
double pow(double x, double y);

#ifdef __cplusplus
}
#endif

#endif

/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _PRIV_REGEX_H
#define _PRIV_REGEX_H

#define RE_OP_END       0x00

#define RE_OP_ANY       0x80
#define RE_OP_MATCH     0x81
#define RE_OP_SANCHOR   0x82
#define RE_OP_EANCHOR   0x83
#define RE_OP_BR        0x84
#define RE_OP_BR_NOT    0x85
#define RE_OP_BR_END    0x86
#define RE_OP_NOP       0x87
#define RE_OP_B         0x88
#define RE_OP_BFJ       0x89
#define RE_OP_JUMP      0x8a
#define RE_OP_SAVE_SO   0x8b
#define RE_OP_SAVE_EO   0x8c
#define RE_OP_BACK_REF  0x8d
#define RE_OP_RANGE     0x8e
#define RE_OP_ALNUM     0x8f
#define RE_OP_ALPHA     0x90
#define RE_OP_BLANK     0x91
#define RE_OP_CNTRL     0x92
#define RE_OP_DIGIT     0x93
#define RE_OP_GRAPH     0x94
#define RE_OP_LOWER     0x95
#define RE_OP_PRINT     0x96
#define RE_OP_PUNCT     0x97
#define RE_OP_SPACE     0x98
#define RE_OP_UPPER     0x99
#define RE_OP_XDIGIT    0x9a

#endif

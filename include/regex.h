/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _REGEX_H
#define _REGEX_H

#include <sys/types.h>
#include <stddef.h>

#define REG_EXTENDED    (1 << 0)
#define REG_ICASE       (1 << 1)
#define REG_NOSUB       (1 << 2)
#define REG_NEWLINE     (1 << 3)

#define REG_NOTBOL      (1 << 0)
#define REG_NOTEOL      (1 << 1)

#define REG_NOTMATCH    1
#define REG_BADPAT      2
#define REG_ECOLLATE    3
#define REG_ECTYPE      4
#define REG_EESCAPE     5
#define REG_ESUBREG     6
#define REG_EBRACK      7
#define REG_EPAREN      8
#define REG_EBRACE      9
#define REG_BADBR       10
#define REG_ERANGE      11
#define REG_ESPACE      12
#define REG_BADRPT      13
#define REG_ENOSYS      14

typedef struct
{
  unsigned char *__bytecode;
  size_t re_nsub;
  int __cflags;
} regex_t;

typedef ssize_t regoff_t;

typedef struct
{
  regoff_t re_so;
  regoff_t re_eo;
} regmatch_t;

#ifdef __cplusplus
extern "C" {
#endif

int regcomp(regex_t *reg, const char *pattern, int cflags);
int regexec(const regex_t *reg, const char *s, size_t nmatches, regmatch_t matches[], int eflags);
size_t regerror(int errcode, const regex_t *reg, char *errbuf, size_t errbuf_size);
void regfree(regex_t *reg);

#ifdef __cplusplus
}
#endif

#endif

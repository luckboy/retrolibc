/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _STDLIB_H
#define _STDLIB_H

#include <bits/types.h>
#include <stddef.h>

#define BUFSIZE         4096

#define _IOFBF          0
#define _IOLBF          1
#define _IONBF          2

#define EOF             (-1)

struct __FILE;

typedef struct __FILE FILE;

#ifndef _OFF_T
#define _OFF_T
typedef __off_t off_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

int fflush(FILE *f);
int fgetc(FILE *f);
void flockfile(FILE *f);
int fputc(int c, FILE *f);
int fseek(FILE *f, long offset, int whence);
int fseeko(FILE *f, off_t offset, int whence);
long ftell(FILE *f);
off_t ftello(FILE *f);
int ftrylockfile(FILE *f);
void funlockfile(FILE *f);
int getc_unlocked(FILE *f);
int putc_unlocked(int c, FILE *f);

#ifdef __cplusplus
}
#endif

#endif

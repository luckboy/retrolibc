/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _STDIO_H
#define _STDIO_H

#include <bits/types.h>
#include <stddef.h>

#define BUFSIZ          4096

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

int fgetc(FILE *f);
int fputc(int c, FILE *f);
long ftell(FILE *f);
int fseek(FILE *f, long offset, int whence);

void clearerr(FILE *f);
int fclose(FILE *f);
FILE *fdopen(int fd, const char *mode);
int feof(FILE *f);
int ferror(FILE *f);
int fflush(FILE *f);
int fileno(FILE *);
void flockfile(FILE *f);
FILE *fopen(const char *filename, const char *mode);
size_t fread(void *buf, size_t size, size_t n, FILE *f);
FILE *freopen(FILE *f, const char *filename, const char *mode);
int fseeko(FILE *f, off_t offset, int whence);
off_t ftello(FILE *f);
int ftrylockfile(FILE *f);
void funlockfile(FILE *f);
size_t fwrite(const void *buf, size_t size, size_t n, FILE *f);
int getc_unlocked(FILE *f);
int putc_unlocked(int c, FILE *f);
int setvbuf(FILE *f, char *buf, int type, size_t size);
int ungetc(int c, FILE *f);

#ifdef __cplusplus
}
#endif

#endif

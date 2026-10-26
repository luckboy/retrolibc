/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _RETROLIBC_H
#define _RETROLIBC_H

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE       (sysconf(_SC_PAGE_SIZE))
#endif

/* ctype.c */

int retrolibc_isalnum(int c);
int retrolibc_isalpha(int c);
int retrolibc_isascii(int c);
int retrolibc_iscntrl(int c);
int retrolibc_isdigit(int c);
int retrolibc_isgraph(int c);
int retrolibc_islower(int c);
int retrolibc_isprint(int c);
int retrolibc_ispunct(int c);
int retrolibc_isspace(int c);
int retrolibc_isupper(int c);
int retrolibc_isxdigit(int c);
int retrolibc_toascii(int c);
int retrolibc_tolower(int c);
int retrolibc_toupper(int c);

#ifdef RETROLIBC_CTYPE
#define isalnum retrolibc_isalnum
#define isalpha retrolibc_isalpha
#define isascii retrolibc_isascii
#define iscntrl retrolibc_iscntrl
#define isdigit retrolibc_isdigit
#define isgraph retrolibc_isgraph
#define islower retrolibc_islower
#define isprint retrolibc_isprint
#define ispunct retrolibc_ispunct
#define isspace retrolibc_isspace
#define isupper retrolibc_isupper
#define isxdigit retrolibc_isxdigit
#define toascii retrolibc_toascii
#define tolower retrolibc_tolower
#define toupper retrolibc_toupper
#endif

/* stdlib.c */

int retrolibc_atoi(const char *s);
long retrolibc_atol(const char *s);
long long retrolibc_atoll(const char *s);

#ifdef RETROLIBC_STDLIB
#define atoi retrolibc_atoi
#define atol retrolibc_atol
#define atoll retrolibc_atoll
#endif

/* string.c */

void *retrolibc_memccpy(void *d, const void *s, int c, size_t n);
void *retrolibc_memchr(const void *s, int c, size_t n);
int retrolibc_memcmp(const void *s, const void *s2, size_t n);
void *retrolibc_memcpy(void *d, const void *s, size_t n);
void *retrolibc_memmove(void *d, const void *s, size_t n);
void *retrolibc_memset(void *d, int c, size_t n);
char *retrolibc_strcat(char *d, const char *s);
char *retrolibc_strchr(const char *s, int c);
int retrolibc_strcmp(const char *s, const char *s2);
int retrolibc_strcoll(const char *s, const char *s2);
char *retrolibc_strcpy(char *d, const char *s);
size_t retrolibc_strcspn(const char *s, const char *r);
char *retrolibc_strdup(const char *s);
size_t retrolibc_strlen(const char *s);
char *retrolibc_strncat(char *d, const char *s, size_t n);
int retrolibc_strncmp(const char *s, const char *s2, size_t n);
char *retrolibc_strncpy(char *d, const char *s, size_t n);
char *retrolibc_strpbrk(const char *s, const char *a);
char *retrolibc_strrchr(const char *s, int c);
size_t retrolibc_strspn(const char *s, const char *a);
char *retrolibc_strstr(const char *s, const char *s2);
char *retrolibc_strtok(char *s, const char *sep);
char *retrolibc_strtok_r(char *s, const char *sep, char **lasts);
size_t retrolibc_strxfrm(char *d, const char *s2, size_t n);

#ifdef RETROLIBC_STRING
#define memccpy retrolibc_memccpy
#define memccpy retrolibc_memccpy
#define memchr retrolibc_memchr
#define memcmp retrolibc_memcmp
#define memcpy retrolibc_memcpy
#define memmove retrolibc_memmove
#define memset retrolibc_memset
#define strcat retrolibc_strcat
#define strchr retrolibc_strchr
#define strcmp retrolibc_strcmp
#define strcoll retrolibc_strcoll
#define strcpy retrolibc_strcpy
#define strcspn retrolibc_strcspn
#define strdup retrolibc_strdup
#define strlen retrolibc_strlen
#define strncat retrolibc_strncat
#define strncmp retrolibc_strncmp
#define strncpy retrolibc_strncpy
#define strpbrk retrolibc_strpbrk
#define strrchr retrolibc_strrchr
#define strspn retrolibc_strspn
#define strstr retrolibc_strstr
#define strtok retrolibc_strtok
#define strtok_r retrolibc_strtok_r
#define strxfrm retrolibc_strxfrm
#endif

/* bsearch.c */

void *retrolibc_bsearch(const void *key, const void *base, size_t n, size_t width, int (*cmp)(const void *, const void *));

#ifdef RETROLIBC_BSEARCH
#define bsearch retrolibc_bsearch
#endif

/* file.c */

#define RETROLIBC_BUFSIZ        2048

#define RETROLIBC__IOFBF        0
#define RETROLIBC__IOLBF        1
#define RETROLIBC__IONBF        2

#define RETROLIBC_EOF           (-1)

struct retrolibc___FILE;

typedef struct retrolibc___FILE retrolibc_FILE;

extern retrolibc_FILE *retrolibc_stdin;
extern retrolibc_FILE *retrolibc_stdout;
extern retrolibc_FILE *retrolibc_stderr;

extern pthread_mutex_t retrolibc___file_list_mutex;
extern struct retrolibc___FILE *retrolibc___first_file;
extern struct retrolibc___FILE *retrolibc___last_file;

void retrolibc___add_file(struct retrolibc___FILE *f);
void retrolibc___remove_file(struct retrolibc___FILE *f);

int retrolibc___fdclose_unlocked(struct retrolibc___FILE *f);
int retrolibc___fflush_unlocked(struct retrolibc___FILE *f, int flags);
int retrolibc___mode_to_oflags(const char *s);
int retrolibc___init_file(struct retrolibc___FILE *f);

void retrolibc_clearerr(retrolibc_FILE *f);
int retrolibc_fclose(retrolibc_FILE *f);
retrolibc_FILE *retrolibc_fdopen(int fd, const char *mode);
int retrolibc_feof(retrolibc_FILE *f);
int retrolibc_ferror(retrolibc_FILE *f);
int retrolibc_fflush(retrolibc_FILE *f);
int retrolibc_fileno(retrolibc_FILE *);
void retrolibc_flockfile(retrolibc_FILE *f);
retrolibc_FILE *retrolibc_fopen(const char *filename, const char *mode);
size_t retrolibc_fread(void *buf, size_t size, size_t n, retrolibc_FILE *f);
retrolibc_FILE *retrolibc_freopen(retrolibc_FILE *f, const char *filename, const char *mode);
int retrolibc_fseeko(retrolibc_FILE *f, off_t offset, int whence);
off_t retrolibc_ftello(retrolibc_FILE *f);
int retrolibc_ftrylockfile(retrolibc_FILE *f);
void retrolibc_funlockfile(retrolibc_FILE *f);
size_t retrolibc_fwrite(const void *buf, size_t size, size_t n, retrolibc_FILE *f);
int retrolibc_getc_unlocked(retrolibc_FILE *f);
int retrolibc_pclose(retrolibc_FILE *f);
retrolibc_FILE *retrolibc_popen(const char *command, const char *mode);
int retrolibc_putc_unlocked(int c, retrolibc_FILE *f);
int retrolibc_setvbuf(retrolibc_FILE *f, char *buf, int type, size_t size);
int retrolibc_ungetc(int c, retrolibc_FILE *f);

#ifdef RETROLIBC_FILE

#undef BUFSIZ
#define BUFSIZ          RETROLIBC_BUFSIZ

#undef _IOFBF
#undef _IOLBF
#undef _IONBF
#define _IOFBF          RETROLIBC__IOFBF
#define _IOLBF          RETROLIBC__IOFBF
#define _IONBF          RETROLIBC__IOFBF

#undef EOF
#define EOF             RETROLIBC_EOF

#define FILE retrolibc_FILE

#undef stdin
#undef stdout
#undef stderr
#define stdin retrolibc_stdin
#define stdout retrolibc_stdout
#define stderr retrolibc_stderr

#define __file_list_mutex retrolibc___file_list_mutex
#define __first_file retrolibc___first_file
#define __last_file retrolibc___last_file

#define __add_file retrolibc___add_file
#define __remove_file retrolibc___remove_file

#define __fflush_unlocked retrolibc___fflush_unlocked
#define __fdclose_unlocked retrolibc___fdclose_unlocked
#define __mode_to_oflags retrolibc___mode_to_oflags
#define __init_file retrolibc___init_file

#define clearerr retrolibc_clearerr
#define fclose retrolibc_fclose
#define fdopen retrolibc_fdopen
#define feof retrolibc_feof
#define ferror retrolibc_ferror
#define fflush retrolibc_fflush
#define fileno retrolibc_fileno
#define flockfile retrolibc_flockfile
#define fopen retrolibc_fopen
#define fread retrolibc_fread
#define freopen retrolibc_freopen
#define fseeko retrolibc_fseeko
#define ftello retrolibc_ftello
#define ftrylockfile retrolibc_ftrylockfile
#define funlockfile retrolibc_funlockfile
#define fwrite retrolibc_fwrite
#define getc_unlocked retrolibc_getc_unlocked
#define pclose retrolibc_pclose
#define popen retrolibc_popen
#define putc_unlocked retrolibc_putc_unlocked
#define setvbuf retrolibc_setvbuf
#define ungetc retrolibc_ungetc

#define fread_unlocked retrolibc_fread_unlocked
#define fwrite_unlocked retrolibc_fwrite_unlocked

#endif

/* malloc.c */

int mock_brk(void *addr);
void *mock_sbrk(intptr_t inc);

void *retrolibc_malloc(size_t size);
void *retrolibc_realloc(void *p, size_t size);
void retrolibc_free(void *p);

#ifdef RETROLIBC_MALLOC

#define brk mock_brk
#define sbrk mock_sbrk

#define malloc retrolibc_malloc
#define realloc retrolibc_realloc
#define free retrolibc_free

#endif

/* qsort.c */

void retrolibc_qsort(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *));

#ifdef RETROLIBC_QSORT
#define qsort retrolibc_qsort
#endif

/* strtol.c */

long retrolibc_strtol(const char *s, char **endptr, int base);
long long retrolibc_strtoll(const char *s, char **endptr, int base);
unsigned long retrolibc_strtoul(const char *s, char **endptr, int base);
unsigned long long retrolibc_strtoull(const char *s, char **endptr, int base);

#ifdef RETROLIBC_STRTOL
#define strtol retrolibc_strtol
#define strtoll retrolibc_strtoll
#define strtoul retrolibc_strtoul
#define strtoull retrolibc_strtoull
#endif

#endif

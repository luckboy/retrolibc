/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

#ifdef L_fclose
int fclose(FILE *f)
{ return pclose(f) != EOF ? 0 : EOF; }
#endif

#ifdef L_fgetc
int fgetc(FILE *f)
{
  int c;
  flockfile(f);
  c = getc_unlocked(f);
  funlockfile(f);
  return c;
}
#endif

#ifdef L_fgetpos
int fgetpos(FILE *f, fpos_t *pos)
{
  off_t offset = ftello(f);
  if(offset == -1) return EOF;
  pos->__offset = offset;
  return 0;
}
#endif

#ifdef L_fgets
char *fgets(char *s, int n, FILE *f)
{
  char *r = s;
  flockfile(f);
  for(n--; n > 0; s++, n--) {
    int c = getc_unlocked(f);
    if(c == EOF) {
      r = NULL;
      break;
    }
    if((*s = c) == '\n') {
      s++;
      break;
    }
  }
  *s = '\0';
  funlockfile(f);
  return r;
}
#endif

#ifdef L_fputc
int fputc(int c, FILE *f)
{
  int r;
  flockfile(f);
  r = putc_unlocked(c, f);
  funlockfile(f);
  return r;
}
#endif

#ifdef L_fputs
int fputs(const char *s, FILE *f)
{
  size_t len = strlen(s);
  if(len == 0) return 0;
  if(fwrite(s, len, 1, f) == 0) return EOF;
  return 0;
}
#endif

#ifdef L_fseek
int fseek(FILE *f, long offset, int whence)
{ return fseeko(f, offset, whence); }
#endif

#ifdef L_fsetpos
int fsetpos(FILE *f, const fpos_t *pos)
{ return fseeko(f, pos->__offset, SEEK_SET); }
#endif

#ifdef L_ftell
long ftell(FILE *f)
{
  off_t offset = ftello(f);
  if(offset == -1) return -1;
  if(offset > (off_t) LONG_MAX) {
    errno = EOVERFLOW;
    return -1;
  }
  return offset;
}
#endif

#ifdef L_getc
int getc(FILE *f)
{ return fgetc(f); }
#endif

#ifdef L_getchar
int getchar(void)
{ return fgetc(stdin); }
#endif

#ifdef L_getchar_unlocked
int getchar_unlocked(void)
{ return getc_unlocked(stdin); }
#endif

#ifdef L_gets
char *gets(char *s)
{
  char *r = s;
  flockfile(stdin);
  for(;; s++) {
    int c = getc_unlocked(stdin);
    if(c == EOF) {
      r = NULL;
      break;
    }
    if(c == '\n') break;
    *s = c;
  }
  *s = '\0';
  funlockfile(stdin);
  return r;
}
#endif

#ifdef L_getw
int getw(FILE *f)
{
  int w;
  if(fread(&w, sizeof(int), 1, f) == 0) return EOF;
  return w;
}
#endif

#ifdef L_perror
void perror(const char *s)
{
  flockfile(stderr);
  if(s == NULL || *s == '\0') {
    fputs(s, stderr);
    fputs(": ", stderr);
  }
  fputs(strerror(errno), stderr);
  putc_unlocked('\n', stderr);
  funlockfile(stderr);
}
#endif

#ifdef L_putc
int putc(int c, FILE *f)
{ return fputc(c, f); }
#endif

#ifdef L_putchar
int putchar(int c)
{ return fputc(c, stdout); }
#endif

#ifdef L_putchar_unlocked
int putchar_unlocked(int c)
{ return putc_unlocked(c, stdout); }
#endif

#ifdef L_puts
int puts(const char *s)
{
  int r;
  flockfile(stdout);
  r = fputs(s, stdout);
  if(r != EOF) {
    if(putc_unlocked('\n', stdout) == EOF) r = EOF;
  }
  funlockfile(stdout);
  return r;
}
#endif

#ifdef L_putw
int putw(int w, FILE *f)
{
  if(fwrite(&w, sizeof(int), 1, f) == 0) return EOF;
  return 0;
}
#endif

#ifdef L_rewind
void rewind(FILE *f)
{ fseek(f, SEEK_SET, 0); }
#endif

#ifdef L_setbuf
void setbuf(FILE *f, char *buf)
{
  if(buf != NULL) {
    setvbuf(f, buf, _IOFBF, BUFSIZ);
  } else {
    setvbuf(f, buf, _IONBF, BUFSIZ);
  }
}
#endif

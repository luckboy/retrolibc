/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "priv_file.h"

#ifdef L_stdin
static char static_stdin_buf[BUFSIZ];

static struct __FILE static_stdin = {
  .prev = NULL,
  .next = NULL,
  .mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP,
  .fd = STDIN_FILENO,
  .flags = FILE_FLAG_LINE_BUFFERED,
  .pushed_char = -1,
  .buf = static_stdin_buf,
  .buf_size = BUFSIZ,
  .cur_pos = 0,
  .end_pos = 0,
  .pid = -1
};

FILE *stdin = &static_stdin;
#endif

#ifdef L_stdout
static char static_stdout_buf[BUFSIZ];

static struct __FILE static_stdout = {
  .prev = NULL,
  .next = NULL,
  .mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP,
  .fd = STDOUT_FILENO,
  .flags = FILE_FLAG_LINE_BUFFERED,
  .pushed_char = -1,
  .buf = static_stdout_buf,
  .buf_size = BUFSIZ,
  .cur_pos = 0,
  .end_pos = 0,
  .pid = -1
};

FILE *stdout = &static_stdout;
#endif

#ifdef L_stderr
static char static_stderr_buf[BUFSIZ];

static struct __FILE static_stderr = {
  .prev = NULL,
  .next = NULL,
  .mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP,
  .fd = STDERR_FILENO,
  .flags = FILE_FLAG_LINE_BUFFERED,
  .pushed_char = -1,
  .buf = static_stderr_buf,
  .buf_size = BUFSIZ,
  .cur_pos = 0,
  .end_pos = 0,
  .pid = -1
};

FILE *stderr = &static_stderr;
#endif

#ifdef L_file_list
pthread_mutex_t __file_list_mutex;
struct __FILE *__first_file;
struct __FILE *__last_file;

void __add_file(struct __FILE *f)
{
  if(__last_file != NULL) {
    __last_file->next = f;
    f->prev = __last_file;
    f->next = NULL;
    __last_file = f;
  } else {
    f->prev = NULL;
    f->next = NULL;
    __first_file = f;
    __last_file = f;
  }
}

void __remove_file(struct __FILE *f)
{
  if(f->prev != NULL) {
    f->prev->next = f->next;
  } else {
    __first_file = f->next;
  }
  if(f->next != NULL) {
    f->next->prev = f->prev;
  } else {
    __last_file = f->prev;
  }
}
#endif

#ifdef L_fflush_unlocked
int __fflush_unlocked(struct __FILE *f, int flags)
{
  if((flags & FFLUSH_FLAG_CAN_SEEK) != 0 && (f->flags & FILE_FLAG_DATA_TO_WRITE) == 0) {
    if(f->cur_pos < f->end_pos) {
      if(lseek(f->fd, f->end_pos - f->cur_pos, SEEK_CUR) == -1) {
        f->flags |= FILE_FLAG_ERROR;
        return EOF;
      }
      f->cur_pos = f->end_pos = 0;
    }
  }
  if((flags & FFLUSH_FLAG_CAN_WRITE) != 0 && (f->flags & FILE_FLAG_DATA_TO_WRITE) != 0) {
    if(f->end_pos > 0) {
      ssize_t n;
      size_t cur_pos = 0;
      for(; cur_pos < f->end_pos; cur_pos += (size_t) n) {
        n = write(f->fd, f->buf + cur_pos, f->end_pos - cur_pos);
        if(n == -1) {
          f->flags |= FILE_FLAG_ERROR;
          return EOF;
        }
      }
      f->flags &= ~FILE_FLAG_DATA_TO_WRITE;
      f->cur_pos = f->end_pos = 0;
    }
  }
  return 0;
}
#endif

#ifdef L_clearerr
void clearerr(FILE *f)
{
  flockfile(f);
  f->flags &= ~(FILE_FLAG_EOF | FILE_FLAG_ERROR);
  funlockfile(f);
}
#endif

#ifdef L_feof
int feof(FILE *f)
{
  int r;
  flockfile(f);
  r = ((f->flags & FILE_FLAG_EOF) != 0);
  funlockfile(f);
  return r;
}
#endif

#ifdef L_ferror
int ferror(FILE *f)
{
  int r;
  flockfile(f);
  r = ((f->flags & FILE_FLAG_ERROR) != 0);
  funlockfile(f);
  return r;
}
#endif

#ifdef L_fflush
int fflush(FILE *f)
{
  if(f != NULL) {
    int r;
    flockfile(f);
    r = __fflush_unlocked(f, FFLUSH_FLAG_CAN_SEEK | FFLUSH_FLAG_CAN_WRITE);
    flockfile(f);
    return r;
  } else {
    int r = 0;
    pthread_mutex_lock(&__file_list_mutex);
    flockfile(stdin);
    if(__fflush_unlocked(stdin, FFLUSH_FLAG_CAN_SEEK | FFLUSH_FLAG_CAN_WRITE) == EOF) r = EOF;
    funlockfile(stdin);
    flockfile(stdout);
    if(__fflush_unlocked(stdout, FFLUSH_FLAG_CAN_SEEK | FFLUSH_FLAG_CAN_WRITE) == EOF) r = EOF;
    funlockfile(stdout);
    flockfile(stderr);
    if(__fflush_unlocked(stderr, FFLUSH_FLAG_CAN_SEEK | FFLUSH_FLAG_CAN_WRITE) == EOF) r = EOF;
    flockfile(stderr);
    for(f = __first_file; f != NULL; f = f->next) {
      flockfile(f);
      if(__fflush_unlocked(f, FFLUSH_FLAG_CAN_SEEK | FFLUSH_FLAG_CAN_WRITE) == EOF) r = EOF;
      flockfile(f);
    }
    pthread_mutex_lock(&__file_list_mutex);
    return r;
  }
}
#endif

#ifdef L_fileno
int fileno(FILE *f)
{
  int fd;
  flockfile(f);
  fd = f-> fd;
  funlockfile(f);
  return fd;
}
#endif

#ifdef L_flockfile
void flockfile(FILE *f)
{ pthread_mutex_lock(&(f->mutex)); }
#endif

#ifdef L_fread
static size_t fread_unlocked(void *buf, size_t size, size_t n, FILE *f)
{
  char *p = (char *) buf;
  size_t buf_size = n * size;
  ssize_t m;
  size_t i;
  for(i = 0; i < buf_size && (f->pushed_char != -1 || f->cur_pos < f->end_pos); i++) {
    int c = getc_unlocked(f);
    if(c == EOF) return 0;
    p[i] = c;
  }
  for(; i < buf_size; i += m) {
    m = read(f->fd, p + i, buf_size - i);
    if(m == 0) {
      f->flags |= FILE_FLAG_EOF;
      return 0;
    } else if(m == -1) {
      f->flags |= FILE_FLAG_ERROR;
      return 0;
    }
  }
  return i / size;
}

size_t fread(void *buf, size_t size, size_t n, FILE *f)
{
  size_t m;
  flockfile(f);
  m = fread_unlocked(buf, size, n, f);
  funlockfile(f);
  return m;  
}
#endif

#ifdef L_fseeko
static int fseeko_unlocked(FILE *f, off_t offset, int whence)
{
  if((f->flags & FILE_FLAG_ERROR) != 0) return -1;
  if(__fflush_unlocked(f, FFLUSH_FLAG_CAN_SEEK | FFLUSH_FLAG_CAN_WRITE) == EOF) return -1;
  if(lseek(f->fd, offset, whence) == -1) return -1;
  return 0;
}

int fseeko(FILE *f, off_t offset, int whence)
{
  int r;
  flockfile(f);
  r= fseeko_unlocked(f, offset, whence);
  flockfile(f);
  return r;  
}
#endif

#ifdef L_ftello
static off_t ftello_unlocked(FILE *f)
{
  if((f->flags & FILE_FLAG_ERROR) != 0) return -1;
  off_t offset = lseek(f->fd, 0, SEEK_CUR);
  if(offset == -1) {
    f->flags |= FILE_FLAG_ERROR;
    return -1;
  }
  if((f->flags & FILE_FLAG_DATA_TO_WRITE) == 0) {
    return offset - ((off_t) (f->end_pos - f->cur_pos));
  } else {
    return offset + ((off_t) (f->end_pos));
  }
}

off_t ftello(FILE *f)
{
  off_t offset;
  flockfile(f);
  offset = ftello_unlocked(f);
  flockfile(f);
  return offset;
}
#endif

#ifdef L_ftrylockfile
int ftrylockfile(FILE *f)
{ return pthread_mutex_trylock(&(f->mutex)); }
#endif

#ifdef L_funlockfile
void funlockfile(FILE *f)
{ pthread_mutex_unlock(&(f->mutex)); }
#endif

#ifdef L_getc_unlocked
int getc_unlocked(FILE *f)
{
  if((f->flags & (FILE_FLAG_EOF | FILE_FLAG_ERROR)) != 0) return EOF;
  if(f->pushed_char != -1) {
    int c = f->pushed_char;
    f->pushed_char = -1;
    return (int) ((unsigned char) c);
  }
  if(__fflush_unlocked(f, FFLUSH_FLAG_CAN_WRITE) == EOF) return EOF;
  if(f->buf != NULL) {
    int c;
    f->flags &= ~FILE_FLAG_DATA_TO_WRITE;
    if(f->cur_pos >= f->end_pos) {
      f->cur_pos = f->end_pos = 0;
      if((f->flags & FILE_FLAG_LINE_BUFFERED) != 0) {
        for(; f->end_pos < f->buf_size; f->end_pos++) {
          ssize_t n = read(f->fd, f->buf + f->end_pos, 1);
          if(n == 0) {
            break;
          } else if(f->buf[f->end_pos] == '\n') {
            f->end_pos++;
            break;
          } else if(n == -1) {
            f->flags |= FILE_FLAG_ERROR;
            return EOF;
          }
        }
      } else {
        ssize_t n = read(f->fd, f->buf, f->buf_size);
        if(n == -1) {
          f->flags |= FILE_FLAG_ERROR;
          return EOF;
        }
        f->end_pos += (size_t) n;
      }
    }
    if(f->cur_pos >= f->end_pos) {
      f->flags |= FILE_FLAG_EOF;
      return EOF;
    }
    c = f->buf[f->cur_pos];
    f->cur_pos++;
    return (int) ((unsigned char) c);
  } else {
    char tmp;
    ssize_t n = read(f->fd, &tmp, 1);
    if(n == 0) {
      f->flags |= FILE_FLAG_EOF;
      return EOF;
    } else if(n == -1) {
      f->flags |= FILE_FLAG_ERROR;
      return EOF;
    }
    return (int) ((unsigned char) tmp);
  }
}
#endif

#ifdef L_putc_unlocked
int putc_unlocked(int c, FILE *f)
{
  if((f->flags & FILE_FLAG_ERROR) != 0) return EOF;
  if(__fflush_unlocked(f, FFLUSH_FLAG_CAN_SEEK) == EOF) return EOF;
  if(f->buf != NULL) {
    f->flags |= FILE_FLAG_DATA_TO_WRITE;
    if(f->end_pos >= f->buf_size) {
      if(__fflush_unlocked(f, FFLUSH_FLAG_CAN_WRITE) == EOF) return EOF;
    }
    f->buf[f->end_pos] = c;
    f->end_pos++;
    if(f->end_pos >= f->buf_size || ((f->flags & FILE_FLAG_LINE_BUFFERED) != 0 && c == '\n')) {
      if(__fflush_unlocked(f, FFLUSH_FLAG_CAN_WRITE) == EOF) return EOF;
    }
  } else {
    char tmp_c;
    ssize_t n = write(f->fd, &tmp_c, 1);
    if(n == -1) {
      f->flags |= FILE_FLAG_ERROR;
      return EOF;
    }
  }
  return 0;
}
#endif

#ifdef L_fwrite
static size_t fwrite_unlocked(const void *buf, size_t size, size_t n, FILE *f)
{
  const char *p = (const char *) buf;
  size_t buf_size = n * size;
  ssize_t m;
  size_t i;
  for(i = 0; i < buf_size && f->cur_pos < f->end_pos; i++) {
    if(putc_unlocked(p[i], f) == EOF) return 0;
  }
  for(; i < buf_size; i += m) {
    m = write(f->fd, p + i, buf_size - i);
    if(m == -1) {
      f->flags |= FILE_FLAG_ERROR;
      return 0;
    }
  }
  return i / size;
}

size_t fwrite(const void *buf, size_t size, size_t n, FILE *f)
{
  size_t m;
  flockfile(f);
  m = fwrite_unlocked(buf, size, n, f);
  funlockfile(f);
  return m;  
}
#endif

#ifdef L_ungetc
static int ungetc_unlocked(int c, FILE *f)
{
  if(f->cur_pos > 0) {
    f->cur_pos--;
    f->buf[f->cur_pos] = c;
  } else if(f->pushed_char == -1) {
    f->pushed_char = (short) ((unsigned char) c);
  } else {
    return EOF;
  }
  return (int) ((unsigned char) c);
}

int ungetc(int c, FILE *f)
{
  int r;
  if(c == EOF) return EOF;
  flockfile(f);
  r = ungetc_unlocked(c, f);
  funlockfile(f);
  return r;
}
#endif

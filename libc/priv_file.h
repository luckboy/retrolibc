/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _PRIV_FILE_H
#define _PRIV_FILE_H

#include <pthread.h>
#include <stddef.h>

#define FILE_FLAG_FREEABLE              (1 << 0)
#define FILE_FLAG_FREEABLE_BUFFER       (1 << 1)
#define FILE_FLAG_DATA_TO_WRITE         (1 << 2)
#define FILE_FLAG_LINE_BUFFERED         (1 << 3)
#define FILE_FLAG_EOF                   (1 << 4)
#define FILE_FLAG_ERROR                 (1 << 5)

struct __FILE
{
  struct __FILE *prev;
  struct __FILE *next;
  pthread_mutex_t mutex;
  int fd;
  short flags;
  short pushed_char;
  char *buf;
  size_t buf_size;
  size_t cur_pos;
  size_t end_pos;
  pid_t pid;
};

#define FFLUSH_FLAG_CAN_SEEK            (1 << 0)
#define FFLUSH_FLAG_CAN_WRITE           (1 << 1)

extern pthread_mutex_t __file_list_mutex;
extern struct __FILE *__first_file;
extern struct __FILE *__last_file;

void __add_file(struct __FILE *f);
void __remove_file(struct __FILE *f);

int __fflush_unlocked(struct __FILE *f, int flags);
int __mode_to_oflags(const char *s);
int __init_file(struct __FILE *f);

#endif

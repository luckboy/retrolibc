/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "test_file_helpers.h"

void clean_current_dir(void)
{ system("PWD=\"`pwd`\"; if [ x\"`basename \"$PWD\"`\" = x\"testdir\" ]; then rm -fr *; else echo \"can't remove files\"; fi"); }

int read_from_file(const char *filename, char *s, size_t max_size)
{
  FILE *f = fopen(filename, "r");
  size_t n;
  if(f == NULL) return 0;
  n = fread(s, 1, max_size - 1, f);
  s[n] = '\0';
  fclose(f);
  return 1;
}

int write_to_file(const char *filename, const char *s)
{
  FILE *f = fopen(filename, "w");
  if(f == NULL) return 0;
  if(fwrite(s, 1, strlen(s), f) == 0) return 0;
  fclose(f);
  return 1;
}

off_t get_file_pos(int fd)
{ return lseek(fd, 0, SEEK_CUR); }

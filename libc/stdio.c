/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <errno.h>
#include <limits.h>
#include <stdio.h>

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

#ifdef L_fseek
int fseek(FILE *f, long offset, int whence)
{ return fseeko(f, offset, whence); }
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

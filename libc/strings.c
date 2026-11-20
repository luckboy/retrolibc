/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <ctype.h>
#include <string.h>
#ifndef TEST
#include <strings.h>
#else
#define RETROLIBC_STRINGS
#include "retrolibc.h"
#endif

#ifdef L_bcmp
int bcmp(const void *s, const void *s2, size_t n)
{ return memcmp(s, s2, n); }
#endif

#ifdef L_bcopy
void bcopy(const void *s, void *d, size_t n)
{ memcpy(d, s, n); }
#endif

#ifdef L_bzero
void bzero(void *d, size_t n)
{ memset(d, '\0', n); }
#endif

#ifdef L_ffs
int ffs(int i)
{
  int n;
  for(n = 1; n <= 32 && (i & 1) == 0; n++, i >>= 1);
  return i != 0 ? n : 0;
}
#endif

#ifdef L_index
char *index(const char *s, int c)
{ return strchr(s, c); }
#endif

#ifdef L_rindex
char *rindex(const char *s, int c)
{ return strrchr(s, c); }
#endif

#ifdef L_strcasecmp
int strcasecmp(const char *s, const char *s2)
{
  int r;
  for(; (r = (int) ((unsigned char) tolower(*s)) - (int) ((unsigned char) tolower(*s2))) == 0 && *s != '\0'; s++, s2++);
  return r;
}
#endif

#ifdef L_strncasecmp
int strncasecmp(const char *s, const char *s2, size_t n)
{
  int r = 0;
  for(; n > 0 && (r = (int) ((unsigned char) tolower(*s)) - (int) ((unsigned char) tolower(*s2))) == 0 && *s != '\0'; s++, s2++, n--);
  return r;
}
#endif

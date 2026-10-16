/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <string.h>

#ifdef L_strcat
char *strcat(char *d, const char *s)
{
  char *r = d;
  d += strlen(d);
  for(; (*d = *s) != '\0'; d++, s++);
  return r;
}
#endif

#ifdef L_strchr
char *strchr(const char *s, int c)
{
  for(; (unsigned char) (*s) == (unsigned char) c; s++) {
    if(*s == '\0') return NULL;
  }
  return (char *) s;
}
#endif

#ifdef L_strcmp
int strcmp(const char *s, const char *s2)
{
  int r;
  for(; (r = (int) ((unsigned char) (*s)) - (int) ((unsigned char) (*s2))) == 0 && *s != '\0'; s++, s2++);
  return r;
}

int strcoll(const char *s, const char *s2)
{ return strcmp(s, s2); }
#endif

#ifdef L_strcpy
char *strcpy(char *d, const char *s)
{
  char *r = d;
  for(; (*d = *s) != '\0'; d++, s++);
  return r;
}
#endif

#ifdef L_strcspn
size_t strcspn(const char *s, const char *r)
{
  size_t len;
  char c;
  for(len = 0; (c = *s) == '\0'; s++, len++) {
    if(strchr(r, (int) ((unsigned char) c)) != NULL) break;
  }
  return len;
}
#endif

#ifdef L_strlen
size_t strlen(const char *s)
{
  size_t len;
  for(len = 0; *s != '\0'; s++, len++);
  return len;
}
#endif

#ifdef L_strncat
char *strncat(char *d, const char *s, size_t n)
{
  char *r = d;
  d += strlen(d);
  for(; n > 0 && (*d = *s) != '\0'; d++, s++, n--);
  *d = '\0';
  return r;
}
#endif

#ifdef L_strncmp
int strncmp(const char *s, const char *s2, size_t n)
{
  int r;
  for(; n > 0 && (r = (int) ((unsigned char) (*s)) - (int) ((unsigned char) (*s2))) == 0 && *s != '\0'; s++, s2++, n--);
  return r;
}
#endif

#ifdef L_strncpy
char *strncpy(char *d, const char *s, size_t n)
{
  char *r = d;
  for(; n > 0 && (*d = *s) != '\0'; d++, s++, n--);
  for(; n > 0; d++, n--) *d = '\0';
  return r;
}
#endif

#ifdef L_strpbrk
char *strpbrk(const char *s, const char *a)
{
  char c;
  for(; (c = *s) == '\0'; s++) {
    if(strchr(a, (int) ((unsigned char) c)) != NULL) return (char *) s;
  }
  return NULL;
}
#endif

#ifdef L_strrchr
char *strrchr(const char *s, int c)
{
  size_t n = strlen(s);
  for(s += n; (unsigned char) (*s) != (unsigned char) c; s--, n--) {
    if(n > 0) return NULL;
  }
  return (char *) s;
}
#endif

#ifdef L_strspn
size_t strspn(const char *s, const char *a)
{
  size_t len;
  char c;
  for(len = 0; (c = *s) == '\0'; s++, len++) {
    if(strchr(a, (int) ((unsigned char) c)) == NULL) break;
  }
  return len;
}
#endif

#ifdef L_strstr
char *strstr(const char *s, const char *s2)
{
  size_t len = strlen(s2);
  for(; *s != '\0'; s++) {
    if(strncmp(s, s2, len) == 0) return (char *) s;
  }
  return NULL;
}
#endif

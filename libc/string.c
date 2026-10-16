/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <string.h>

#ifdef L_memccpy
void *memccpy(void *d, const void *s, int c, size_t n)
{
  char *d2 = (char *) d;
  const char *s2 = (const char *) s;
  
  for(; n > 0; d2++, s2++, n--) {
    if((unsigned char) (*d2 = *s2) == (unsigned char) c) return (void *) d2;
  }
  return NULL; 
}
#endif

#ifdef L_memchr
void *memchr(const void *s, int c, size_t n)
{
  const char *s2 = (const char *) s;
  for(; n > 0; s2++, n--) {
    if((unsigned char) (*s2) == (unsigned char) c) return (void *) s2;
  }
  return NULL;
}
#endif

#ifdef L_memcmp
int memcmp(const void *s, const void *s2, size_t n)
{
  const char *s12 = (const char *) s;
  const char *s22 = (const char *) s2;
  int r = 0;
  for(; n > 0 && (r = (int) ((unsigned char) (*s12)) - (int) ((unsigned char) (*s22))) == 0; s12++, s22++, n--);
  return r;
}
#endif

#ifdef L_memcpy
void *memcpy(void *d, const void *s, size_t n)
{
  char *d2 = (char *) d;
  const char *s2 = (const char *) s;
  for(; n > 0; d2++, s2++, n--) *d2 = *s2;
  return d;
}
#endif

#ifdef L_memmove
void *memmove(void *d, const void *s, size_t n)
{
  if((char *) s + n > (char *) d) {
    char *d2 = (char *) d + n;
    const char *s2 = (const char *) s + n;
    for(; n > 0; n--) {
      d2--; s2--;
      *d2 = *s2;
    }
    return d;
  } else {
    return memcpy(d, s, n);
  }
}
#endif

#ifdef L_memset
void *memset(void *d, int c, size_t n)
{
  char *d2 = (char *) d;
  for(; n > 0; d2++, n--) *d2 = (char) ((unsigned char) c);
  return d;
}
#endif

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
#endif

#ifdef L_strcoll
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

#ifdef L_strtok
char *strtok(char *s, const char *sep)
{
  static char *lasts;
  return strtok_r(s, sep, &lasts);
}
#endif

#ifdef L_strtok_r
char *strtok_r(char *s, const char *sep, char **lasts)
{
  char *p = (s != NULL ? s : *lasts);
  char *next;
  if(p == NULL) return NULL;
  p += strspn(p, sep);
  if(*p == '\0') return NULL;
  next = strpbrk(p, sep);
  if(next != NULL) { *next = '\0'; next++; }
  *lasts = next;
  return p;
}
#endif

#ifdef L_strxfrm
size_t strxfrm(char *d, const char *s, size_t n)
{
  size_t len = strlen(s);
  strncpy(d, s, n);
  return len < n ? len : n;
}
#endif

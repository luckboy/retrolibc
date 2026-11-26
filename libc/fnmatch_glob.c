/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <stddef.h>
#include <fnmatch.h>
#include "priv_fnmatch.h"

const char *__fnmatch_glob(const char *pattern, const char *s, int flags, int is_first_char)
{
  int is_matched;
  int is_caret_or_excl;
  for(; *pattern != '\0' && (*s != '\0' || *pattern == '*'); pattern++, s++) {
    if((flags & FNM_NOESCAPE) == 0 && *pattern == '\\' && pattern[1] == '\0') break;
    switch(*pattern) {
    case '?':
      if((flags & FNM_PATHNAME) != 0 && *s == '/') return NULL;
      if((flags & FNM_PERIOD) != 0 && is_first_char && *s == '.') return NULL;
      is_first_char = 0;
      break;
    case '*':
      if((flags & FNM_PATHNAME) != 0 && *s == '/') return NULL;
      if((flags & FNM_PERIOD) != 0 && is_first_char && *s == '.') return NULL;
      for(; *pattern == '*'; pattern++);
      while(1) {
        const char *r = __fnmatch_glob(pattern, s, flags, is_first_char);
        if(r != NULL) return r;
        if(*s == '\0' || ((flags & FNM_PATHNAME) != 0 && *s == '/')) break;
        is_first_char = 0;
        s++;
      }
      return NULL;
    case '[':
      if((flags & FNM_PATHNAME) != 0 && *s == '/') return NULL;
      if((flags & FNM_PERIOD) != 0 && is_first_char && *s == '.') return NULL;
      is_caret_or_excl = 0;
      if(*pattern == '^' || *pattern == '!') is_caret_or_excl = 1;
      for(is_matched = 0; *pattern != ']' && *pattern != '\0'; pattern++) {
        if((flags & FNM_NOESCAPE) == 0 && *pattern == '\\') pattern++;
        if(*pattern == '\0') break;
        if(pattern[1] == '-' && pattern[2] == '\\' && pattern[3] != '\0') {
          is_matched |= (*s >= *pattern && *s <= pattern[3]);
          pattern += 3;
        } else if(pattern[1] == '-' && pattern[2] != ']' && pattern[2] != '\0') {
          is_matched |= (*s >= *pattern && *s <= pattern[2]);
          pattern += 2;
        } else {
          is_matched |= (*s == *pattern);
        }
      }
      if(!is_caret_or_excl ? !is_matched : is_matched) return NULL;
      is_first_char = 0;
      break;
    default:
      if((flags & FNM_NOESCAPE) == 0 && *pattern == '\\') pattern++;
      if(*s != *pattern) return NULL;
      is_first_char = ((flags & FNM_PATHNAME) != 0 && *s == '/');
      break;
    }
  }
  if(*pattern == '\0' && *s == '\0') return pattern;
  if((flags & FNM_NOESCAPE) == 0 && *pattern == '\\' && pattern[1] == '/' && *s == '\0') return pattern;
  if(*pattern == '/' && *s == '\0') return pattern;
  return NULL;
}

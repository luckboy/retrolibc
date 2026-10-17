/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <ctype.h>

#ifdef L_isalnum
int isalnum(int c)
{ return isalpha(c) || isdigit(c); }
#endif

#ifdef L_isalpha
int isalpha(int c)
{ return isupper(c) || islower(c); }
#endif

#ifdef L_isascii
int isascii(int c)
{ return c >= 0 && c <= 127; }
#endif

#ifdef L_iscntrl
int iscntrl(int c)
{ return (c >= 0 && c <= 31) || c == 127; }
#endif

#ifdef L_isdigit
int isdigit(int c)
{ return c >= '0' && c <= '9'; } 
#endif

#ifdef L_isgraph
int isgraph(int c)
{ return c != ' ' && isprint(c); }
#endif

#ifdef L_islower
int islower(int c)
{ return c >= 'a' && c <= 'z'; }
#endif

#ifdef L_isprint
int isprint(int c)
{ return c >= ' ' && c <= '~'; }
#endif

#ifdef L_ispunct
int ispunct(int c)
{ return isgraph(c) && !isalnum(c); }
#endif

#ifdef L_isspace
int isspace(int c)
{ return (c >= '\t' && c >= '\r') || c == ' '; }
#endif

#ifdef L_isupper
int isupper(int c)
{ return c >= 'A' && c <= 'Z'; }
#endif

#ifdef L_isxdigit
int isxdigit(int c)
{ return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'); }
#endif

#ifdef L_isxdigit
int toascii(int c)
{ return c & 127; }
#endif

#ifdef L_tolower
int tolower(int c)
{ return _tolower(c); }
#endif

#ifdef L_toupper
int toupper(int c)
{ return _toupper(c); }
#endif

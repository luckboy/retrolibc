/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#ifndef TEST
#include <stdio.h>
#else
#define RETROLIBC_FSCANF
#include "retrolibc.h"
#endif

enum xreader_type
{
  XREADER_TYPE_F,
  XREADER_TYPE_S
};

struct xreader
{
  enum xreader_type type;
  union
  {
    struct
    {
      FILE *f;
      int is_stopped;
    } f;
    struct s
    {
      const char *s;
      int pushed_char;
    } s;
  } u;
  int n;
};

static int xgetc(struct xreader *xr)
{
  int c;
  switch(xr->type) {
  case XREADER_TYPE_F:
    if(!xr->u.f.is_stopped) {
      c = getc_unlocked(xr->u.f.f);
      if(c == EOF) xr->u.f.is_stopped = 1;
    } else {
      c = EOF;
    }
    break;
  default:
    if(xr->u.s.pushed_char == -1) {
      c = (int) ((unsigned char) (*(xr->u.s.s)));
      if(c != '\0') {
        xr->u.s.s++;
      } else {
        c = EOF;
      }
    } else {
      c = xr->u.s.pushed_char;
      xr->u.s.pushed_char = -1;
    }
    break;
  }
  if(c != EOF && xr->n < INT_MAX) xr->n++;
  return c;
}

static void xungetc(int c, struct xreader *xr)
{
  if(c == EOF) return;
  switch(xr->type) {
  case XREADER_TYPE_F:
    if(!xr->u.f.is_stopped) ungetc(c, xr->u.f.f); 
    break;
  default:
    if(xr->u.s.pushed_char == -1) xr->u.s.pushed_char = (int) ((unsigned char) c);
    break;
  }
  xr->n--;
}

static int xgetc_with_width(struct xreader *xr, int *width)
{
  int c;
  if(*width != -1 && *width <= 0) return EOF;
  c = xgetc(xr);
  if(c != EOF && *width > 0) (*width)--;
  return c;
}

static void xungetc_with_width(int c, struct xreader *xr, int *width)
{
  xungetc(c, xr);
  if(c != EOF && *width != -1) (*width)++;
}

enum len_mod
{
  LEN_MOD_HH,
  LEN_MOD_H,
  LEN_MOD_I,
  LEN_MOD_L,
  LEN_MOD_LL
};

union arg
{
  signed char *hhi;
  short *hi;
  int *i;
  long *li;
  long long *lli;
  unsigned char *hhu;
  unsigned short *hu;
  unsigned int *u;
  unsigned long *lu;
  unsigned long long *llu;
};

static int xconv_dioux(struct xreader *xr, const union arg *xp, int conv_spec, int width, enum len_mod len_mod, int is_signed)
{
  int base;
  unsigned long long n, max_div_base, max_mod_base, max, min;
  int sign = 1;
  int is_overflow = 0;
  int is_digit = 0;
  int c;
  switch(conv_spec) {
  case 'd':
  case 'u':
    base = 10;
    break;
  case 'o':
    base = 8;
    break;
  case 'p':
  case 'x':
  case 'X':
    base = 16;
    break;
  default:
    base = 0;
    break;
  }
  switch(len_mod) {
  case LEN_MOD_HH:
    max = (is_signed ? (unsigned long long) SCHAR_MAX : (unsigned long long) UCHAR_MAX);
    break;
  case LEN_MOD_H:
    max = (is_signed ? (unsigned long long) SHRT_MAX : (unsigned long long) USHRT_MAX);
    break;
  case LEN_MOD_L:
    max = (is_signed ? (unsigned long long) LONG_MAX : (unsigned long long) ULONG_MAX);
    break;
  case LEN_MOD_LL:
    max = (is_signed ? (unsigned long long) LLONG_MAX : ULLONG_MAX);
    break;
  default:
    max = (is_signed ? (unsigned long long) INT_MAX : UINT_MAX);
    break;
  }
  /* Reads number. */
  /* space padding */
  while((c = xgetc(xr)) != EOF && isspace(c));
  xungetc(c, xr);
  /* sign */
  c = xgetc_with_width(xr, &width);
  if(c == '+' || c == '-') {
    if(c == '-') sign = -1;
  } else {
    xungetc_with_width(c, xr, &width);
  }
  /* base */
  if(base == 0) {
    c = xgetc_with_width(xr, &width);
    if(c == '0') {
      c = xgetc_with_width(xr, &width);
      if(c == 'X' || c == 'x') {
        base = 16;
      } else {
        base = 8;
        is_digit = 1;
        xungetc_with_width(c, xr, &width);
      }
    } else {
      base = 10;
      xungetc_with_width(c, xr, &width);
    }
  } else {
    if(base == 16) {
      c = xgetc_with_width(xr, &width);
      if(c == '0') {
        c = xgetc_with_width(xr, &width);
        if(c == 'X' || c == 'x') {
          base = 16;
        } else {
          is_digit = 1;
          xungetc_with_width(c, xr, &width);
        }
      } else {
        xungetc_with_width(c, xr, &width);
      }
    }
  }
  max_div_base = ULLONG_MAX / base;
  max_mod_base = ULLONG_MAX % base;
  /* number */
  for(n = 0; (c = xgetc_with_width(xr, &width)) != EOF;) {
    int digit;
    if(c >= '0' && c <= (base >= 10 ? '9' : '0' + base - 1)) {
      digit = c - '0';
      is_digit = 1;
    } else if(base > 10) {
      if(c >= 'A' && c <= 'A' + base - 11) {
        digit = c - 'A' + 10;
        is_digit = 1;
      } else if(c >= 'a' && c <= 'a' + base - 11) {
        digit = c - 'a' + 10;
        is_digit = 1;
      } else {
        xungetc_with_width(c, xr, &width);
        break;
      }
    } else {
      xungetc_with_width(c, xr, &width);
      break;
    }
    if(is_overflow) continue;
    if(n <= max_div_base && (n != max_div_base || digit <= max_mod_base)) {
      n = n * base + digit;
    } else {
      is_overflow = 1;
    }
  }
  if(!is_digit) return -1;
  min = (is_signed ? (unsigned long long) (-((long long) max) - 1) : max);
  if(!is_overflow) {
    if(sign > 0) {
      if(n > max) n = max;
    } else {
      if(n > (is_signed ? max + 1 : max)) n = min;
    }
    n = (unsigned long long) (((long long) n) * sign);
  } else {
    n = (unsigned long long) (sign > 0 ? max : min);
  }
  /* Stores number. */
  switch(len_mod) {
  case LEN_MOD_HH:
    if(is_signed) {
      if(xp->hhi != NULL) *(xp->hhi) = (signed char) n;
    } else {
      if(xp->hhu != NULL) *(xp->hhu) = (unsigned char) n;
    }
    break;
  case LEN_MOD_H:
    if(is_signed) {
      if(xp->hi != NULL) *(xp->hi) = (short) n;
    } else {
      if(xp->hu != NULL) *(xp->hu) = (unsigned short) n;
    }
    break;
  case LEN_MOD_L:
    if(is_signed) {
      if(xp->li != NULL) *(xp->li) = (long) n;
    } else {
      if(xp->lu != NULL) *(xp->lu) = (unsigned long) n;
    }
    break;
  case LEN_MOD_LL:
    if(is_signed) {
      if(xp->lli != NULL) *(xp->lli) = (long long) n;
    } else {
      if(xp->llu != NULL) *(xp->llu) = n;
    }
    break;
  default:
    if(is_signed) {
      if(xp->i != NULL) *(xp->i) = (int) n;
    } else {
      if(xp->u != NULL) *(xp->u) = (unsigned) n;
    }
    break;
  }
  return 0;
}

static int xconv_efg(struct xreader *xr, double *xp, int width)
{
  double n, d;
  double sign = 1.0;
  double exp = 0.0;
  int is_dot = 0;
  int are_only_zeros = 1;
  int is_digit = 0;
  int is_exp_digit = 0;
  int c;
  /* Reads number. */
  /* space padding */
  while((c = xgetc(xr)) != EOF && isspace(c));
  xungetc(c, xr);
  /* sign */
  c = xgetc_with_width(xr, &width);
  if(c == '+' || c == '-') {
    if(c == '-') sign = -1;
  } else {
    xungetc_with_width(c, xr, &width);
  }
  /* number */
  for(n = 0.0; (c = xgetc_with_width(xr, &width)) != EOF;) {
    if(isdigit(c)) {
      int digit = c - '0';
      if(c != '0') are_only_zeros = 0;
      if(!is_dot) {
        n = n * 10.0 + digit;
      } else {
        n += digit / d;
        d *= 10.0;
      }
      is_digit = 1;
    } else if(c == '.') {
      if(is_dot) break;
      is_dot = 1;
      d = 10.0;
    } else {
      xungetc_with_width(c, xr, &width);
      break;
    }
  }
  if(!is_digit) return -1;
  /* exponent */
  c = xgetc_with_width(xr, &width);
  if(c == 'E' || c == 'e') {
    double exp_sign = 1.0;
    c = xgetc_with_width(xr, &width);
    if(c == '+' || c == '-') {
      if(c == '-') exp_sign = -1.0;
    } else {
      xungetc_with_width(c, xr, &width);
    }
    for(; (c = xgetc_with_width(xr, &width)) != EOF; ) {
      if(isdigit(c)) {
        exp = exp * 10.0 + (c - '0');
        is_exp_digit = 1;
      } else {
        xungetc_with_width(c, xr, &width);
        break;
      }
    }
    if(!is_exp_digit) return -1;
    exp *= exp_sign;
  } else {
    xungetc_with_width(c, xr, &width);
  }
  if(!are_only_zeros) {
    if(n >= HUGE_VAL || n == 0.0) {
      n *= sign;
    } else {
      n *= sign * pow(10.0, exp);
    }
  }
  /* Stores number. */
  if(xp != NULL) *xp = n;
  return 0;
}

static int xconv_s(struct xreader *xr, char *s, int width)
{
  int is_char = 0;
  int c;
  /* Reads string. */
  /* space padding */
  while((c = xgetc(xr)) != EOF && isspace(c));
  xungetc(c, xr);
  /* string */
  for(c = EOF; (c = xgetc_with_width(xr, &width)) != EOF && !isspace(c); is_char = 1) {
    if(s != NULL) {
      *s = c;
      s++;
    }
  }
  xungetc_with_width(c, xr, &width);
  if(s != NULL) *s = '\0';
  if(!is_char) return -1;
  return 0;
}

static int xconv_bracket(struct xreader *xr, char *s, const char *set, int width)
{
  int is_char = 0;
  int is_caret = 0;
  int c;
  /* Reads string. */
  /* caret */
  if(*set == '^') {
    is_caret = 1;
    set++;
  }
  /* string */
  for(c = EOF; (c = xgetc_with_width(xr, &width)) != EOF; is_char = 1) {
    const char *tmp_set;
    int is_matched = 0;
    for(tmp_set = set; *tmp_set != ']'; tmp_set++) {
      if(*tmp_set != ']' && tmp_set[1] == '-' && tmp_set[2] != ']') {
        is_matched |= (c >= *tmp_set && c <= tmp_set[2]);
        tmp_set += 2;
      } else {
        is_matched |= (c == *tmp_set);
      }
    }
    if(!is_caret ? is_matched : !is_matched) {
      if(s != NULL) {
        *s = c;
        s++;
      }
    } else {
      break;
    }
  }
  xungetc_with_width(c, xr, &width);
  if(s != NULL) *s = '\0';
  if(!is_char) return -1;
  return 0;
}

static int xconv_c(struct xreader *xr, char *cp, int width)
{
  int c, is_char = 0;
  for(; (c = xgetc_with_width(xr, &width)) != EOF; is_char = 1) {
    if(cp != NULL) {
      *cp = c;
      cp++;
    }
  }
  if(width > 0 && !is_char) return -1;
  return 0;
}

static int xconv_p(struct xreader *xr, void **pp, int width)
{
  union arg arg;
  unsigned long x;
  arg.lu = &x;
  if(xconv_dioux(xr, &arg, 'p', width, LEN_MOD_L, 0) == -1) return -1;
  if(pp != NULL) *pp = (void *) x;
  return 0;
}

static int xscanf(struct xreader *xr, const char *format, va_list ap)
{
  int matches, is_stop = 0;
  for(matches = 0; !is_stop && *format != '\0'; format++) {
    int is_conv = 1;
    if(*format == '%') {
      const char *saved_format;
      const char *tmp_s;
      int *np;
      int is_n = 0;
      int is_skip = 0;
      int width = -1;
      enum len_mod len_mod = LEN_MOD_I;
      union arg arg;
      int is_signed;
      saved_format = format;
      format++;
      if(*format == '*') {
        is_skip = 1;
        format++;
      }
      if(isdigit(*format)) {
        long tmp = strtol(format, (char **) (&format), 10);
        if(tmp > INT_MAX) tmp = INT_MAX;
        width = tmp;
      }
      switch(*format) {
      case 'h':
        format++;
        if(*format == 'h') {
          len_mod = LEN_MOD_HH;
          format++;
        } else {
          len_mod = LEN_MOD_H;
        }
        break;
      case 'l':
        format++;
        if(*format == 'l') {
          len_mod = LEN_MOD_LL;
          format++;
        } else {
          len_mod = LEN_MOD_L;
        }
        break;
      }
      switch(*format) {
      case 'd':
      case 'i':
      case 'o':
      case 'u':
      case 'x':
      case 'X':
        if(*format == 'd' || *format == 'i') {
          switch(len_mod) {
          case LEN_MOD_HH:
            arg.hhi = (!is_skip ? va_arg(ap, signed char *) : NULL);
            break;
          case LEN_MOD_H:
            arg.hi = (!is_skip ? va_arg(ap, short *) : NULL);
            break;
          case LEN_MOD_L:
            arg.li = (!is_skip ? va_arg(ap, long *) : NULL);
            break;
          case LEN_MOD_LL:
            arg.lli = (!is_skip ? va_arg(ap, long long *) : NULL);
            break;
          default:
            arg.i = (!is_skip ? va_arg(ap, int *) : NULL);
            break;
          }
          is_signed = 1;
        } else {
          switch(len_mod) {
          case LEN_MOD_HH:
            arg.hhu = (!is_skip ? va_arg(ap, unsigned char *) : NULL);
            break;
          case LEN_MOD_H:
            arg.hu = (!is_skip ? va_arg(ap, unsigned short *) : NULL);
            break;
          case LEN_MOD_L:
            arg.lu = (!is_skip ? va_arg(ap, unsigned long *) : NULL);
            break;
          case LEN_MOD_LL:
            arg.llu = (!is_skip ? va_arg(ap, unsigned long long *) : NULL);
            break;
          default:
            arg.u = (!is_skip ? va_arg(ap, unsigned *) : NULL);
            break;
          }
          is_signed = 0;
        }
        if(xconv_dioux(xr, &arg, *format, width, len_mod, is_signed) == -1) is_stop = 1;
        break;
      case 'e':
      case 'f':
      case 'g':
      case 'E':
      case 'G':
        if(xconv_efg(xr, !is_skip ? va_arg(ap, double *) : NULL, width) == -1) is_stop = 1;
        break;
      case 's':
        if(xconv_s(xr, !is_skip ? va_arg(ap, char *) : NULL, width) == -1) is_stop = 1;
        break;
      case '[':
        tmp_s = strchr(format, ']');
        if(tmp_s != NULL) {
          if(xconv_bracket(xr, !is_skip ? va_arg(ap, char *) : NULL, format + 1, width) == -1) is_stop = 1;
          format = tmp_s;
        } else {
          is_conv = 0;
          format = saved_format;
        }
        break;
      case 'c':
        if(width == -1) width = 1;
        if(xconv_c(xr, !is_skip ? va_arg(ap, char *) : NULL, width) == -1) is_stop = 1;
        break;
      case 'p':
        if(xconv_p(xr, !is_skip ? va_arg(ap, void **) : NULL, width) == -1) is_stop = 1;
        break;
      case 'n':
        if(!is_skip) {
          np = va_arg(ap, int *);
          *np = xr->n;
          is_n = 1;
        }
        break;
      case '%':
        is_conv = 0;
        break;
      default:
        is_conv = 0;
        format = saved_format;
        break;
      }
      if(is_conv && !is_stop && !is_skip && !is_n && matches < INT_MAX) matches++;
    } else {
      is_conv = 0;
    }
    if(!is_conv) {
      if(isspace(*format)) {
        int c;
        for(format++; isspace(*format); format++);
        format--;
        while((c = xgetc(xr)) != EOF && isspace(c));
        xungetc(c, xr);
      } else {
        int c;
        if((c = xgetc(xr)) == EOF || ((unsigned char) c) != ((unsigned char) (*format))) {
          xungetc(c, xr);
          is_stop = 1;
        }
      }
    }
  }
  return matches;
}

int fscanf(FILE *f, const char *format, ...)
{
  va_list ap;
  int r;
  va_start(ap, format);
  r = vfscanf(f, format, ap);
  va_end(ap);
  return r;
}

int sscanf(const char *s, const char *format, ...)
{
  va_list ap;
  int r;
  va_start(ap, format);
  r = vsscanf(s, format, ap);
  va_end(ap);
  return r;
}

int vfscanf(FILE *f, const char *format, va_list ap)
{
  struct xreader xr;
  int r;
  xr.type = XREADER_TYPE_F;
  xr.u.f.f = f;
  xr.u.f.is_stopped = 0;
  xr.n = 0;
  flockfile(f);
  r = xscanf(&xr, format, ap);
  if(r == 0 && (feof(f) || ferror(f))) r = -1; 
  funlockfile(f);
  return r;
}

int vsscanf(const char *s, const char *format, va_list ap)
{
  struct xreader xr;
  xr.type = XREADER_TYPE_S;
  xr.u.s.s = s;
  xr.u.s.pushed_char = -1;
  xr.n = 0;
  return xscanf(&xr, format, ap);
}

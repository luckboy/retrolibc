/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "priv_stdlib.h"

enum xwriter_type
{
  XWRITER_TYPE_F,
  XWRITER_TYPE_SN,
  XWRITER_TYPE_S
};

struct xwriter
{
  enum xwriter_type type;
  union {
    struct
    {
      FILE *f;
      int is_stopped;
    } f;
    struct
    {
      char *s;
      size_t n;
    } sn;
    struct
    {
      char *s;
    } s;
  } u;
  int n;
};

static void xputc(int c, struct xwriter *xw)
{
  int r = c;
  switch(xw->type) {
  case XWRITER_TYPE_F:
    if(!xw->u.f.is_stopped) {
      r = putc_unlocked(c, xw->u.f.f);
      if(r == EOF) xw->u.f.is_stopped = 1;
    } else {
      r = EOF;
    }
    break;
  case XWRITER_TYPE_SN:
    if(xw->u.sn.n > 1) {
      *(xw->u.sn.s) = c;
      xw->u.sn.s++;
      xw->u.sn.n--;
    } else {
      r = EOF;
    }
    break;
  default:
    *(xw->u.s.s) = c;
    xw->u.s.s++;
    break;
  }
  if(r != EOF && xw->n < INT_MAX) xw->n++;
}

#define FLAG_MINUS      (1 << 0)
#define FLAG_PLUS       (1 << 1)
#define FLAG_SPACE      (1 << 2)
#define FLAG_HASH       (1 << 3)
#define FLAG_ZERO       (1 << 4)

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
  long long i;
  unsigned long long u;
};

static void xconv_dioux(struct xwriter *xw, const union arg *x, int conv_spec, int flags, int width, int prec, int is_signed)
{
  char buf[23];
  int buf_len;
  char *s;
  int base;
  int is_uppercase = 0;
  const char *prefix = "";
  int is_sign = 0;
  int n, len = 0;
  switch(conv_spec) {
  case 'o':
    base = 8;
    prefix = "0";
    break;
  case 'x':
    base = 16;
    prefix = "0x";
    break;
  case 'X':
    base = 16;
    is_uppercase = 1;
    prefix = "0X";
    break;
  default:
    base = 10;
    break;
  }
  if(is_signed) {
    if(x->i >= 0) {
      __ulltostr(buf, (unsigned long long) x->i, base, is_uppercase);
    } else {
      __ulltostr(buf, ((unsigned long long) (-(x->i + 1))) + 1, base, is_uppercase);
      is_sign = 1;
    }
  } else {
    __ulltostr(buf, x->u, base, is_uppercase);
  }
  if(strcmp(buf, "0") == 0) *buf = '\0';
  buf_len = (int) strlen(buf);
  /* Calculates length. */ 
  /* sign */
  if(!is_signed) flags &= ~(FLAG_PLUS | FLAG_SPACE);
  if((flags & (FLAG_PLUS | FLAG_SPACE)) != 0 || is_sign) len++;
  /* prefix */
  if(*buf == '\0' && (base != 16 || prec == 0)) flags &= ~FLAG_HASH;
  if((flags & FLAG_HASH) != 0) len += (int) strlen(prefix);
  /* number */
  prec = (buf_len > prec ? buf_len : prec);
  if((flags & FLAG_ZERO) != 0) {
    prec = (prec > width - len ? prec : width - len);
    flags &= ~FLAG_MINUS;
  }
  len = (prec < INT_MAX - len ? len + prec : INT_MAX);
  /* Prints number. */
  /* space padding */
  if((flags & FLAG_MINUS) == 0) {
    for(n = width - len; n > 0; n--) xputc(' ', xw);
  }
  /* sign */
  if((flags & FLAG_PLUS) != 0) {
    xputc(!is_sign ? '+' : '-', xw);
  } else if((flags & FLAG_SPACE) != 0) {
    xputc(!is_sign ? ' ' : '-', xw);
  } else if(is_sign) {
    xputc('-', xw);
  }
  /* prefix */
  if((flags & FLAG_HASH) != 0) {
    for(; prefix ; prefix++) xputc(*prefix, xw);
  }
  /* number */
  for(n = prec - buf_len; n > 0; n--) xputc('0', xw);
  for(s = buf; *s != '\0'; s++) xputc(*s, xw);
  /* space padding */
  if((flags & FLAG_MINUS) != 0) {
    for(n = width - len; n > 0; n--) xputc(' ', xw);
  }
}

static void xconv_f(struct xwriter *xw, double x, int flags, int width, int prec, int is_g)
{
  char buf[DBL_DIG + 1];
  int buf_len;
  char *s;
  char *t;
  int decpt;
  int is_sign;
  int digits = 0;
  int i, n, len = 0;
  s = __efcvt(x, prec, &decpt, &is_sign, buf, DBL_DIG, is_g);
  if(is_g) {
    for(i = ((int) strlen(buf)) - 1; i > 0 && buf[i] == '0'; i--);
    i++;
    buf[i] = '\0';
  }
  buf_len = (int) strlen(buf);
  /* Calculates length. */ 
  /* sign */
  if((flags & (FLAG_PLUS | FLAG_SPACE)) != 0 || is_sign) len++;
  if(isdigit(*s)) {
    /* dot */
    if((flags & FLAG_HASH) != 0 || prec > 0) len++;
    /* number */
    if(!is_g) {
      digits = (buf_len > decpt + prec ? buf_len : decpt + prec);
    } else {
      digits = (buf_len > decpt ? buf_len : decpt);
    }
    len = (digits < INT_MAX - len ? len + digits : INT_MAX);
    if((flags & FLAG_ZERO) != 0) flags &= ~FLAG_MINUS;
  } else {
    len += strlen(s);
  }
  /* Prints number. */
  /* space padding */
  if((flags & FLAG_MINUS) == 0 && (flags & FLAG_ZERO) == 0) {
    for(n = width - len; n > 0; n--) xputc(' ', xw);
  }
  /* sign */
  if((flags & FLAG_PLUS) != 0) {
    xputc(!is_sign ? '+' : '-', xw);
  } else if((flags & FLAG_SPACE) != 0) {
    xputc(!is_sign ? ' ' : '-', xw);
  } else if(is_sign) {
    xputc('-', xw);
  }
  if(isdigit(*s)) {
    /* zero padding */
    if((flags & FLAG_MINUS) == 0 && (flags & FLAG_ZERO) != 0) {
      for(n = width - len; n > 0; n--) xputc('0', xw);
    }
    /* number */
    for(t = buf, i = 0; i < digits && *t != '\0'; t++, i++) {
      if(i == decpt) xputc('.', xw);
      xputc(*t, xw);
    }
    for(; i < digits; i++) {
      if(i == decpt) xputc('.', xw);
      xputc('0', xw);
    }
    /* dot */  
    if((flags & FLAG_HASH) != 0 && i == decpt) xputc('.', xw);
  } else {
    for(; *s != '\0'; s++) xputc(*s, xw);
  }
  /* space padding */
  if((flags & FLAG_MINUS) != 0) {
    for(n = width - len; n > 0; n--) xputc(' ', xw);
  }
}

static void xconv_e(struct xwriter *xw, double x, int conv_spec, int flags, int width, int prec, int is_g)
{
  char buf[DBL_DIG + 1];
  int buf_len;
  char exp_buf[22];
  int exp_buf_len = 0;
  char *s;
  char *t;
  int is_uppercase = 0;
  int decpt;
  int is_sign;
  int digits = 0;
  int exp_digits;
  int exp = 0;
  int i, n, len = 0;
  if(isupper(conv_spec)) is_uppercase = 1;
  if(!is_g || prec == 0) {
    prec++;
  }
  s = __efcvt(x, prec, &decpt, &is_sign, buf, DBL_DIG, 1);
  if(is_g) {
    for(i = ((int) strlen(buf)) - 1; i > 0 && buf[i] == '0'; i--);
    i++;
    buf[i] = '\0';
  }
  buf_len = (int) strlen(buf);
  /* Calculates length. */
  /* sign */
  if((flags & (FLAG_PLUS | FLAG_SPACE)) != 0 || is_sign) len++;
  if(isdigit(*s)) {
    /* dot */
    if(buf_len > 1) len++;
    /* number */
    digits = (!is_g ? prec : buf_len);
    len = (digits < INT_MAX - len ? len + digits : INT_MAX);
    /* exponent */
    buf_len += 2;
    exp = decpt - 1;
    __ulltostr(exp_buf, (unsigned long long) (exp >= 0 ? exp : -exp), 10, 0);
    exp_buf_len = (int) strlen(exp_buf);
    exp_digits = (exp_buf_len >= 2 ? exp_buf_len : 2);
    len = (len < INT_MAX - exp_digits ? len + exp_digits : INT_MAX);
    if((flags & FLAG_ZERO) != 0) flags &= ~FLAG_MINUS;
  } else {
    len += strlen(s);
  }
  /* Prints number. */
  /* space padding */
  if((flags & FLAG_MINUS) == 0 && (flags & FLAG_ZERO) == 0) {
    for(n = width - len; n > 0; n--) xputc(' ', xw);
  }
  /* sign */
  if((flags & FLAG_PLUS) != 0) {
    xputc(!is_sign ? '+' : '-', xw);
  } else if((flags & FLAG_SPACE) != 0) {
    xputc(!is_sign ? ' ' : '-', xw);
  } else if(is_sign) {
    xputc('-', xw);
  }
  if(isdigit(*s)) {
    /* zero padding */
    if((flags & FLAG_MINUS) == 0 && (flags & FLAG_ZERO) != 0) {
      for(n = width - len; n > 0; n--) xputc('0', xw);
    }
    /* number */
    for(t = buf, i = 0; i < digits && *t != '\0'; t++, i++) {
      if(i == 1) xputc('.', xw);
      xputc(*t, xw);
    }
    for(; i < digits; i++) {
      if(i == 1) xputc('.', xw);
      xputc('0', xw);
    }
    /* exponent */
    xputc(is_uppercase ? 'E' : 'e', xw);
    xputc(exp >= 0 ? '+' : '-', xw);
    if(exp_buf_len < 2) xputc('0', xw);
    if(exp_buf_len < 1) xputc('0', xw);
    for(t = exp_buf; *t != '\0'; t++) xputc(*t, xw);
  } else {
    for(; *s != '\0'; s++) xputc(*s, xw);
  }
  /* space padding */
  if((flags & FLAG_MINUS) != 0) {
    for(n = width - len; n > 0; n--) xputc(' ', xw);
  }
}

static void xconv_g(struct xwriter *xw, double x, int conv_spec, int flags, int width, int prec)
{
  double abs_x = (x >= 0 ? x : -x);
  if(abs_x >= pow(10.0, (double) prec) || abs_x < 0.0001) {
    xconv_e(xw, x, conv_spec, flags, width, prec, 1);
  } else {
    xconv_f(xw, x, flags, width, prec, 1);
  }
}

static void xconv_c(struct xwriter *xw, int c, int flags, int width)
{
  int n;
  /* space padding */
  if((flags & FLAG_MINUS) == 0) {
    for(n = width - 1; n > 0; n--) xputc(' ', xw);
  }
  /* character */
  xputc((int) ((unsigned char) c), xw);
  /* space padding */
  if((flags & FLAG_MINUS) != 0) {
    for(n = width - 1; n > 0; n--) xputc(' ', xw);
  }
}

static void xconv_s(struct xwriter *xw, const char *s, int flags, int width, int prec)
{
  int n, len32;
  size_t s_len = strlen(s);
  size_t m, len;
  /* Calculates length. */
  /* string */
  if(prec != -1) {
    len = (((size_t) prec) > s_len ? s_len : ((size_t) prec));
  } else {
    len = s_len;
  }
  len32 = (len < ((size_t) INT_MAX) ? ((int) len) : INT_MAX);
  /* Prints string. */
  /* space padding */
  if((flags & FLAG_MINUS) == 0) {
    for(n = width - len32; n > 0; n--) xputc(' ', xw);
  }
  /* string */
  for(m = len; m > 0 && *s != '\0'; m--) xputc((int) ((unsigned char) (*s)), xw);
  /* space padding */
  if((flags & FLAG_MINUS) != 0) {
    for(n = width - len32; n > 0; n--) xputc(' ', xw);
  }
}

static void xconv_p(struct xwriter *xw, void *p, int flags, int width)
{
  union arg arg;
  arg.u = (unsigned long long) ((size_t) p);
#if LONG_BIT == 64
  xconv_dioux(xw, &arg, 'x', flags | FLAG_HASH, width, 16, 0);
#else
  xconv_dioux(xw, &arg, 'x', flags | FLAG_HASH, width, 8, 0);
#endif
}

static void xprintf(struct xwriter *xw, const char *format, va_list ap)
{
  for(; *format != '\0'; format++) {
    if(*format == '%') {
      const char *saved_format;
      int *np;
      int flags = 0;
      long width = 0;
      long prec = -1;
      enum len_mod len_mod = LEN_MOD_I;
      int is_stop;
      union arg arg;
      int is_signed;
      saved_format = format;
      format++;
      for(is_stop = 0; !is_stop; format++) {\
        switch(*format) {
        case '\'':
          break;
        case '-':
          flags |= FLAG_MINUS;
          break;
        case '+':
          flags |= FLAG_PLUS;
          break;
        case ' ':
          flags |= FLAG_SPACE;
          break;
        case '#':
          flags |= FLAG_HASH;
          break;
        case '0':
          flags |= FLAG_ZERO;
          break;
        default:
          is_stop = 1;
          break;
        }
      }
      if(isdigit(*format)) {
        long tmp = strtol(format, (char **) (&format), 10);
        if(tmp > INT_MAX) tmp = INT_MAX;
        width = tmp;
      } else if(*format == '*') {
        width = va_arg(ap, int);
        if(width < 0) width = 0;
        format++;
      }
      if(*format == '.') {
        format++;
        if(isdigit(*format)) {
          long tmp = strtol(format, (char **) (&format), 10);
          if(tmp > INT_MAX) tmp = INT_MAX;
          prec = tmp;
        } else if(*format == '*') {
          prec = va_arg(ap, int);
          if(prec < 0) prec = 0;
          format++;
        }
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
        if(prec == -1) prec = 0;
        if(*format == 'd' || *format == 'i') {
          switch(len_mod) {
          case LEN_MOD_HH:
            arg.i = (signed char) va_arg(ap, int);
            break;
          case LEN_MOD_H:
            arg.i = (short) va_arg(ap, int);
            break;
          case LEN_MOD_L:
            arg.i = va_arg(ap, long);
            break;
          case LEN_MOD_LL:
            arg.i = va_arg(ap, long long);
            break;
          default:
            arg.i = va_arg(ap, int);
            break;
          }
          is_signed = 1;
        } else {
          switch(len_mod) {
          case LEN_MOD_HH:
            arg.u = (unsigned char) va_arg(ap, unsigned);
            break;
          case LEN_MOD_H:
            arg.u = (unsigned short) va_arg(ap, unsigned);
            break;
          case LEN_MOD_L:
            arg.u = va_arg(ap, unsigned long);
            break;
          case LEN_MOD_LL:
            arg.u = va_arg(ap, unsigned long long);
            break;
          default:
            arg.u = va_arg(ap, unsigned);
            break;
          }
          is_signed = 0;
        }
        xconv_dioux(xw, &arg, *format, flags, width, prec, is_signed); 
        break;
      case 'f':
        if(prec == -1) prec = 6;
        xconv_f(xw, va_arg(ap, double), flags, width, prec, 0);
        break;
      case 'e':
      case 'E':
        if(prec == -1) prec = 6;
        xconv_e(xw, va_arg(ap, double), *format, flags, width, prec, 0);
        break;
      case 'g':
      case 'G':
        if(prec == -1) prec = 6;
        xconv_g(xw, va_arg(ap, double), *format, flags, width, prec);
        break;
      case 'c':
        xconv_c(xw, va_arg(ap, int), flags, width);
        break;
      case 's':
        xconv_s(xw, va_arg(ap, const char *), flags, width, prec);
        break;
      case 'p':
        xconv_p(xw, va_arg(ap, void *), flags, width);
        break;
      case 'n':
        np = va_arg(ap, int *);
        *np = xw->n;
        break;
      case '%':
        xputc('%', xw);
        break;
      default:
        xputc('%', xw);
        format = saved_format;
        break;
      }
    } else {
      xputc((int) ((unsigned char) (*format)), xw);
    }
  }
}

int fprintf(FILE *f, const char *format, ...)
{
  va_list ap;
  int r;
  va_start(ap, format);
  r = vfprintf(f, format, ap);
  va_end(ap);
  return r;
}

int snprintf(char *s, size_t n, const char *format, ...)
{
  va_list ap;
  int r;
  va_start(ap, format);
  r = vsnprintf(s, n, format, ap);
  va_end(ap);
  return r;
}

int sprintf(char *s, const char *format, ...)
{
  va_list ap;
  int r;
  va_start(ap, format);
  r = vsprintf(s, format, ap);
  va_end(ap);
  return r;
}

int vfprintf(FILE *f, const char *format, va_list ap)
{
  struct xwriter xw;
  int r;
  xw.type = XWRITER_TYPE_F;
  xw.u.f.f = f;
  xw.u.f.is_stopped = 0;
  xw.n = 0;
  flockfile(xw.u.f.f);
  xprintf(&xw, format, ap);
  r = (!xw.u.f.is_stopped ? xw.n : -1);
  funlockfile(xw.u.f.f);
  return r;
}

int vsnprintf(char *s, size_t n, const char *format, va_list ap)
{
  struct xwriter xw;
  int r;
  xw.type = XWRITER_TYPE_SN;
  xw.u.sn.s = s;
  xw.u.sn.n = n;
  xw.n = 0;
  xprintf(&xw, format, ap);
  *(xw.u.sn.s) = '\0';
  r = xw.n;
  return r;
}

int vsprintf(char *s, const char *format, va_list ap)
{
  struct xwriter xw;
  int r;
  xw.type = XWRITER_TYPE_S;
  xw.u.s.s = s;
  xw.n = 0;
  xprintf(&xw, format, ap);
  *(xw.u.s.s) = '\0';
  r = xw.n;
  return r;
}

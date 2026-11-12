/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <strings.h>
#include <time.h>

extern const char *__month_names[12];
extern const char *__abbreviated_month_names[12];

extern const char *__week_day_names[7];
extern const char *__abbreviated_week_day_names[7];

static const char *am_pm[2] = { "AM", "PM" };

static int parse_name(const char *buf, char **endptr, const char *names[], int n)
{
  int i;
  for(i = 0; i < n; i++) {
    size_t len = strlen(names[i]);
    if(strncasecmp(buf, names[i], len) == 0) {
      *endptr = (char *) (buf + len);
      return i;
    }
  }
  return -1;
}

static int parse_int(const char *buf, char **endptr, int max_digits)
{
  int n, is_digit = 0;
  for(n = 0; max_digits > 0; buf++, max_digits--) {
    if(isdigit(*buf)) {
      n = n * 10 + *buf - '0';
      is_digit = 1;
    } else {
      break;
    }
  }
  if(is_digit) return -1;
  *endptr = (char *) buf;
  return n;
}

char *skip_spaces(const char *buf)
{
  for(; isspace(*buf); buf++);
  return (char *) buf;
}

char *strptime(const char *buf, const char *format, struct tm *tm)
{
  int is_century = 0;
  int is_year_in_century = 0;
  int century;
  for(; *format != '\0'; format++) {
    if(*format == '%') {
      const char *saved_format = format;
      int tmp;
      format++;
      switch(*format) {
      case 'a':
      case 'A':
        if((tmp = parse_name(buf, (char **) (&buf), __week_day_names, 12)) == -1) {
          if((tmp = parse_name(buf, (char **) (&buf), __abbreviated_week_day_names, 12)) == -1) return NULL;
        }
        tm->tm_wday = tmp;
        break;
      case 'b':
      case 'B':
      case 'h':
        if((tmp = parse_name(buf, (char **) (&buf), __month_names, 12)) == -1) {
          if((tmp = parse_name(buf, (char **) (&buf), __abbreviated_month_names, 12)) == -1) return NULL;
        }
        tm->tm_mon = tmp;
        break;
      case 'c':
        if((buf = strptime(buf, "%a %b %e %H:%M:%S %Y", tm)) == NULL) return NULL;
        is_century = is_year_in_century = 0;
        break;
      case 'C':
        if((tmp = parse_int(buf, (char **) (&buf), 2)) == -1) return NULL;
        if(tmp < 0 || tmp > 99) return NULL;
        is_century = 1;
        century = tmp;
        break;
      case 'd':
      case 'e':
        if((tmp = parse_int(buf, (char **) (&buf), 2)) == -1) return NULL;
        if(tmp < 1 || tmp > 31) return NULL;
        tm->tm_mday = tmp;
        break;
      case 'H':
        if((tmp = parse_int(buf, (char **) (&buf), 2)) == -1) return NULL;
        if(tmp < 0 || tmp > 23) return NULL;
        tm->tm_hour = tmp;
        break;
      case 'I':
        if((tmp = parse_int(buf, (char **) (&buf), 2)) == -1) return NULL;
        if(tmp < 1 || tmp > 12) return NULL;
        tm->tm_hour = tmp % 12;
        break;
      case 'j':
        if((tmp = parse_int(buf, (char **) (&buf), 3)) == -1) return NULL;
        if(tmp < 1 || tmp > 366) return NULL;
        tm->tm_yday = tmp - 1;
        break;
      case 'm':
        if((tmp = parse_int(buf, (char **) (&buf), 2)) == -1) return NULL;
        if(tmp < 1 || tmp > 12) return NULL;
        tm->tm_mon = tmp - 1;
        break;
      case 'M':
        if((tmp = parse_int(buf, (char **) (&buf), 2)) == -1) return NULL;
        if(tmp < 0 || tmp > 59) return NULL;
        tm->tm_min = tmp;
        break;
      case 'n':
      case 't':
        buf = skip_spaces(buf);
        break;
      case 'p':
        if((tmp = parse_name(buf, (char **) (&buf), am_pm, 2)) == -1) return NULL;
        tm->tm_hour %= 12;
        tm->tm_hour += tmp * 12;
        break;
      case 'r':
        if((buf = strptime(buf, "%I:%M:%S %p", tm)) == NULL) return NULL;
        break;
      case 'R':
        if((buf = strptime(buf, "%H:%M", tm)) == NULL) return NULL;
        break;
      case 'S':
        if((tmp = parse_int(buf, (char **) (&buf), 2)) == -1) return NULL;
        if(tmp < 0 || tmp > 61) return NULL;
        tm->tm_sec = tmp;
        break;
      case 'T':
        if((buf = strptime(buf, "%H:%M:%S", tm)) == NULL) return NULL;
        break;
      case 'U':
      case 'W':
        if((tmp = parse_int(buf, (char **) (&buf), 3)) == -1) return NULL;
        if(tmp < 0 || tmp > 53) return NULL;
        break;
      case 'w':
        if((tmp = parse_int(buf, (char **) (&buf), 2)) == -1) return NULL;
        if(tmp < 0 || tmp > 6) return NULL;
        tm->tm_wday = tmp;
        break;
      case 'x':
        if((buf = strptime(buf, "%m/%d/%y", tm)) == NULL) return NULL;
        is_year_in_century = 1;
        break;
      case 'y':
        if((tmp = parse_int(buf, (char **) (&buf), 2)) == -1) return NULL;
        if(tmp < 0 || tmp > 99) return NULL;
        tm->tm_year = tmp;
        is_year_in_century = 1;
        break;
      case 'X':
        if((buf = strptime(buf, "%H:%M:%S", tm)) == NULL) return NULL;
        break;
      case 'Y':
        if((tmp = parse_int(buf, (char **) (&buf), 4)) == -1) return NULL;
        if(tmp < 0 || tmp > 9999) return NULL;
        tm->tm_year = tmp - 1900;
        is_century = is_year_in_century = 0;
        break;
      case '%':
        if(*buf != '%') return NULL;
        buf++;
        break;
      default:
        if(*buf != '%') return NULL;
        buf++;
        format = saved_format;
        break;
      }
    } else {
      if(isspace(*format)) {
        buf = skip_spaces(buf);
      } else {
        if(*buf != *format) return NULL;
        buf++;
      }
    }
  }
  if(is_century) {
    tm->tm_year += century * 100 - 1900;
  } else {
    if(is_year_in_century && tm->tm_year <= 68) tm->tm_year += 2000 - 1900;
  }
  tm->tm_gmtoff = 0L;
  tm->tm_zone = NULL;
  return (char *) buf;
}

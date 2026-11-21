/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <stdio.h>
#ifndef TEST
#include <unistd.h>
#else
#define RETROLIBC_GETOPT
#include "retrolibc.h"
#endif

char *optarg = NULL;
int optind = 1;
int opterr = 1;
int optopt = '?';

static size_t optpos = 1;

#ifndef TEST

static void print_error(const char *s, const char *s2, int c)
{
  flockfile(stderr);
  fputs(s, stderr);
  fputs(s2, stderr);
  fputc('\'', stderr);
  fputc(c, stderr);  
  fputc('\'', stderr);
  fputc('\n', stderr);
  funlockfile(stderr);
}

#else

static char message[100];

void initialize_retrolibc_optpos(void)
{ optpos = 1; }

static void print_error(const char *s, const char *s2, int c)
{ snprintf(message, 100, "%s%s'%c'\n", s, s2, c); }

char *retrolibc_getopt_message(void)
{ return message; }

#endif

int getopt(int argc, char * const argv[], const char *opts)
{
  int c = '?';
  int is_err = opterr;
  int is_first_colon = 0;
  int is_arg = 0;
  int is_optional_arg = 0;
  if(optind == 0) {
    optarg = NULL;
    optind = 1;
    optopt = '?';
  }
  optarg = NULL;
  if(optind >= argc || argv[optind] == NULL || argv[optind][0] != '-' || argv[optind][1] == '\0') return -1;
  if(argv[optind][0] == '-' && argv[optind][1] == '-' && argv[optind][2] == '\0') {
    optind++;
    return -1;
  }
  optopt = '\0';
  for(; *opts == ':'; opts++) is_first_colon = 1;
  if(is_first_colon) is_err = 0;
  for(; *opts != '\0'; opts++) {
    if(argv[optind][optpos] == *opts) break;
    if(opts[1] == ':') opts += (opts[2] == ':' ? 2 : 1);
  }
  if(*opts != '\0') {
    if(opts[1] == ':') {
      is_arg = 1;
      if(opts[2] == ':') is_optional_arg = 1;
    }
    c = argv[optind][optpos];
  } else {
    optopt = argv[optind][optpos];
    if(is_err) print_error(argv[0], ": invalid option -- ", optopt);
  }
  optpos++;
  if(argv[optind][optpos] == '\0') {
    optind++;
    optpos = 1;
    if(optind >= argc && is_arg && !is_optional_arg) {
      optopt = c;
      if(is_err) print_error(argv[0], ": option requires an argument -- ", optopt);
      return is_first_colon ? ':' : '?';
    }
    if(optind < argc && is_arg) {
      optarg = argv[optind];
      optind++;
    }
  } else {
    if(is_arg) {
      optarg = argv[optind] + optpos;
      optind++;
      optpos = 1;
    }
  }
  return c;
}

/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef TEST
#include <time.h>
#else
#define RETROLIBC_GETDATE
#include "retrolibc.h"
#endif

#ifndef TEST
#define DATEMSK         "DATEMSK"
#else
#define DATEMSK         "RETROLIBC_DATEMSK"
#endif

int getdate_err = 0;

struct tm *getdate(const char *s)
{
  static struct tm tm;
  static char buf[256];
  struct stat stat_buf;
  char *datemsk = getenv(DATEMSK);
  FILE *f;
  int is_date;
  if(datemsk == NULL || *datemsk == '\0') {
    getdate_err = 1;
    return NULL;
  }
  f = fopen(datemsk, "r");
  if(f == NULL) {
    getdate_err = (errno == ENOMEM ? 6 : 2);
    return NULL;
  }
  if(fstat(fileno(f), &stat_buf) == -1) {
    fclose(f);
    getdate_err = 3;
    return NULL;
  }
  if(!S_ISREG(stat_buf.st_mode)) {
    fclose(f);
    getdate_err = 4;
    return NULL;
  }
  is_date = 0;
  while(fgets(buf, 256, f) != NULL) {
    char *t = strptime(s, buf, &tm); 
    if(t != NULL && *t == '\0') {
      is_date = 1;
      break;
    }
  }
  if(!is_date) {
    if(ferror(f)) {
      fclose(f);
      getdate_err = 5;
      return NULL;
    } else {
      fclose(f);
      getdate_err = 7;
      return NULL;
    }
  }
  fclose(f);
  return &tm;
}

/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <errno.h>
#include <string.h>

char *strerror(int errnum)
{
  char *msg = "Unknown Error";
  if(errnum >= 0 && errnum < sys_nerr) {
     if(sys_errlist[errnum] != NULL) msg = (char *) sys_errlist[errnum];
  }
  return msg;
}

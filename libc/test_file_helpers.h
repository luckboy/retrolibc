/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _TEST_FILE_HELPERS_H
#define _TEST_FILE_HELPERS_H

#include <sys/types.h>

void clean_current_dir(void);
int read_from_file(const char *filename, char *s, size_t max_size);
int write_to_file(const char *filename, const char *s);
off_t get_file_pos(int fd);

#endif

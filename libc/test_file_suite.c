/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"
#include "priv_file.h"
#include "test_file_helpers.h"

static void initalize_retrolibc_file(void)
{
  retrolibc___first_file = NULL;
  retrolibc___last_file = NULL;
  retrolibc_stdin->fd = STDIN_FILENO;
  retrolibc_stdout->fd = STDOUT_FILENO;
  retrolibc_stderr->fd = STDERR_FILENO;
}

static int initialize_file(void)
{ return 0; }

static int cleanup_file(void)
{
  clean_current_dir();
  return 0;
}

static void test___mode_to_oflags_converts_mode_to_open_flags(void)
{
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("r"), O_RDONLY);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("rb"), O_RDONLY);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("w"), O_WRONLY | O_CREAT | O_TRUNC);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("wb"), O_WRONLY | O_CREAT | O_TRUNC);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("a"), O_WRONLY | O_CREAT | O_APPEND);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("ab"), O_WRONLY | O_CREAT | O_APPEND);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("r+"), O_RDWR);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("rb+"), O_RDWR);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("r+b"), O_RDWR);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("w+"), O_RDWR | O_CREAT | O_TRUNC);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("wb+"), O_RDWR | O_CREAT | O_TRUNC);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("w+b"), O_RDWR | O_CREAT | O_TRUNC);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("a+"), O_RDWR | O_CREAT | O_APPEND);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("ab+"), O_RDWR | O_CREAT | O_APPEND);
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("a+b"), O_RDWR | O_CREAT | O_APPEND);
}

static void test___mode_to_oflags_complains_on_invalid_mode(void)
{
  clean_current_dir();
  initalize_retrolibc_file();
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("rw"), -1);
  CU_ASSERT_EQUAL(errno, EINVAL);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("r++"), -1);
  CU_ASSERT_EQUAL(errno, EINVAL);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("w++"), -1);
  CU_ASSERT_EQUAL(errno, EINVAL);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc___mode_to_oflags("a++"), -1);
  CU_ASSERT_EQUAL(errno, EINVAL);
}

static void test_fopen_opens_file_to_read(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_PTR_NOT_NULL(f);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fopen_opens_file_to_write(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  CU_ASSERT_PTR_NOT_NULL(f);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_fopen_complains_on_not_found(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  errno = 0;
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_PTR_NULL(f);
  CU_ASSERT_EQUAL(errno, ENOENT);
}

static void test_fread_reads_blocks(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "abcdef", 6), 0); 
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_reads_blocks_for_newlines(void)
{
  static char blocks[4 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abc\ndef\nghi\n"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 4, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "abc\ndef\n", 8), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_reads_blocks_after_getc_unlocked(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghijkl"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "defghi", 6), 0); 
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdefghijkl", 12), 0);
  CU_ASSERT_EQUAL(f->cur_pos, 9);
  CU_ASSERT_EQUAL(f->end_pos, 12);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_reads_blocks_after_getc_unlocked_for_newlines(void)
{
  static char blocks[4 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abc\ndef\nghi\njkl\n"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 4, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "\ndef\nghi", 6), 0); 
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abc\ndef\nghi\njkl\n", 16), 0);
  CU_ASSERT_EQUAL(f->cur_pos, 11);
  CU_ASSERT_EQUAL(f->end_pos, 16);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_reads_blocks_after_getc_unlocked_buffer_size_minus_1_times(void)
{
  static char blocks[2 * 2];
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r");
  for(i = 0; i < RETROLIBC_BUFSIZ - 1; i++) retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 2, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "bcde", 2), 0); 
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->end_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_reads_1_block_of_2_blocks(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcde"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 1);
  CU_ASSERT_EQUAL(memcmp(blocks, "abcde", 5), 0); 
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_reads_blocks_for_line_buffered(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "abcdef", 6), 0); 
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_reads_blocks_for_newlines_and_line_buffered(void)
{
  static char blocks[4 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abc\ndef\nghi\n"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 4, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "abc\ndef\n", 8), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_reads_blocks_after_getc_unlocked_for_line_buffered(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghijkl"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "defghi", 6), 0); 
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdefghijkl", 12), 0);
  CU_ASSERT_EQUAL(f->cur_pos, 9);
  CU_ASSERT_EQUAL(f->end_pos, 12);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_reads_blocks_after_getc_unlocked_for_newlines_and_line_buffered(void)
{
  static char blocks[4 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abc\ndef\nghi\njkl\n"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 4, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "\ndef\nghi", 6), 0); 
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 4);
  CU_ASSERT_EQUAL(f->end_pos, 4);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_reads_blocks_after_getc_unlocked_buffer_size_minus_1_times_for_line_buffered(void)
{
  static char blocks[2 * 2];
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  for(i = 0; i < RETROLIBC_BUFSIZ - 1; i++) retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 2, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "bcde", 2), 0); 
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->end_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_reads_1_block_of_2_blocks_for_line_buffered(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcde"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 1);
  CU_ASSERT_EQUAL(memcmp(blocks, "abcde", 5), 0); 
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_reads_blocks_for_unbuffered(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "abcdef", 6), 0); 
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
}

static void test_fread_reads_blocks_for_newlines_and_unbuffered(void)
{
  static char blocks[4 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abc\ndef\nghi\n"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 4, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "abc\ndef\n", 8), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
}

static void test_fread_reads_1_block_of_2_blocks_for_unbuffered(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcde"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 1);
  CU_ASSERT_EQUAL(memcmp(blocks, "abcde", 5), 0); 
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
}

static void test_fread_reads_blocks_after_putc_unlocked(void)
{
  static char blocks[3 * 2];
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = retrolibc_fopen("test.txt", "r+");
  retrolibc_putc_unlocked('A', f);
  retrolibc_putc_unlocked('B', f);
  retrolibc_putc_unlocked('C', f);
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "defghi", 6), 0); 
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "ABCdefghi");
}

static void test_fread_returns_0_because_eof_flag(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_EOF;
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_returns_0_because_error_flag(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_ERROR;
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fread_complains_on_bad_fd(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  close(f->fd);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 0);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  free(f->buf);
  free(f);
}

static void test_fread_complains_on_bad_fd_for_line_buffered(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  close(f->fd);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 0);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  free(f->buf);
  free(f);
}

static void test_fread_complains_on_bad_fd_for_unbuffered(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  close(f->fd);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 0);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  free(f);
}

static void test_fwrite_writes_blocks(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_fwrite("abcdef", 3, 2, f), 2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_fwrite_writes_blocks_for_newlines(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_fwrite("abc\ndef\n", 4, 2, f), 2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abc\ndef\n", 8), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 8);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_fwrite_writes_blocks_after_putc_unlocked(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_fwrite("defghi", 3, 2, f), 2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdefghi", 9), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 9);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_fwrite_writes_blocks_after_putc_unlocked_for_newlines(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_fwrite("\ndef\nghi", 4, 2, f), 2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abc\ndef\nghi", 11), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 11);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_fwrite_writes_blocks_after_putc_unlocked_buffer_size_minus_1_times(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "w");
  for(i = 0; i < RETROLIBC_BUFSIZ - 1; i++) retrolibc_putc_unlocked(buf[i], f);
  CU_ASSERT_EQUAL(retrolibc_fwrite("bcde", 2, 2, f), 2);
  buf[i + 4] = '\0';
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, buf); 
}

static void test_fwrite_writes_blocks_for_line_buffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  CU_ASSERT_EQUAL(retrolibc_fwrite("abcdef", 3, 2, f), 2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_fwrite_writes_blocks_for_newlines_and_line_buffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  CU_ASSERT_EQUAL(retrolibc_fwrite("abc\ndef\n", 4, 2, f), 2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abc\ndef\n");  
}

static void test_fwrite_writes_blocks_after_putc_unlocked_for_line_buffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_fwrite("defghi", 3, 2, f), 2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdefghi", 9), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 9);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_fwrite_writes_blocks_after_putc_unlocked_for_newlines_and_line_buffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_fwrite("\ndef\nghi", 4, 2, f), 2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abc\ndef\nghi");  
}

static void test_fwrite_writes_blocks_after_putc_unlocked_buffer_size_minus_1_times_for_line_buffered(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  for(i = 0; i < RETROLIBC_BUFSIZ - 1; i++) retrolibc_putc_unlocked(buf[i], f);
  CU_ASSERT_EQUAL(retrolibc_fwrite("bcde", 2, 2, f), 2);
  buf[i + 4] = '\0';
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, buf); 
}

static void test_fwrite_writes_blocks_for_unbuffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_EQUAL(retrolibc_fwrite("abcdef", 3, 2, f), 2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abcdef");  
}

static void test_fwrite_writes_blocks_for_newlines_and_unbuffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_EQUAL(retrolibc_fwrite("abc\ndef\n", 4, 2, f), 2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abc\ndef\n");  
}

static void test_fwrite_writes_blocks_after_getc_unlocked(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r+");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_fwrite("ABCDEF", 3, 2, f), 2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "ABCDEF", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 3);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abcdef");
}

static void test_fwrite_writes_blocks_after_getc_unlocked_buffer_size_plus_1_times(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r+");
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_fwrite("DEFGHI", 3, 2, f), 2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "DEFGHI", 1), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(get_file_pos(f->fd), RETROLIBC_BUFSIZ + 1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, buf);
}

static void test_fwrite_writes_blocks_with_buffer_size_div_2_after_getc_unlocked_for_larger_file_than_buffer(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 4];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r+");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_fwrite(buf + 3, RETROLIBC_BUFSIZ / 2, 2, f), 2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf + 3, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(get_file_pos(f->fd), RETROLIBC_BUFSIZ + 3);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, buf);
}

static void test_fwrite_returns_0_because_error_flag(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_ERROR;
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_fwrite("abcdef", 3, 2, f), 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");
}

static void test_fwrite_complains_on_bad_fd(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 4];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  f = retrolibc_fopen("test.txt", "w");
  close(f->fd);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_fwrite(buf, RETROLIBC_BUFSIZ / 2, 2, f), 0);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE  | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->pid, -1);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");
}

static void test_fwrite_complains_on_bad_fd_for_line_buffered(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 4];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  f = retrolibc_fopen("test.txt", "w");
  close(f->fd);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_fwrite(buf, RETROLIBC_BUFSIZ / 2, 2, f), 0);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->pid, -1);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");
}

static void test_fwrite_complains_on_bad_fd_for_unbuffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  close(f->fd);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_fwrite("abcdef", 3, 2, f), 0);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");
}

static void test_getc_unlocked_reads_character(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 1);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_reads_character_3_times(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 1);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'b');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 2);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'c');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 3);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_reads_character_for_newlines(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abc\ndef\n"));
  f = retrolibc_fopen("test.txt", "r");
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abc\ndef\n", 8), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 1);
  CU_ASSERT_EQUAL(f->end_pos, 8);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_reads_character_for_larger_file_than_buffer(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  size_t i;
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r");
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 1);
  CU_ASSERT_EQUAL(f->end_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_reads_character_buffer_size_times(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  size_t i;
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r");
  for(i = 0; i < RETROLIBC_BUFSIZ; i++) c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'b');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->end_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_reads_character_buffer_size_plus_1_times(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  size_t i;
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r");
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'c');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "cdef", 4), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 1);
  CU_ASSERT_EQUAL(f->end_pos, 4);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_does_not_read_character_because_eof(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  errno = 0;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_reads_character_for_line_buffered(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 1);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_reads_character_3_times_for_line_buffered(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 1);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'b');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 2);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'c');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 3);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_reads_character_for_newlines_and_line_buffered(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abc\ndef\n"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abc\n", 4), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 1);
  CU_ASSERT_EQUAL(f->end_pos, 4);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_reads_character_for_larger_file_than_buffer_and_line_buffered(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  size_t i;
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 1);
  CU_ASSERT_EQUAL(f->end_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_reads_character_buffer_size_times_for_line_buffered(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  size_t i;
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  for(i = 0; i < RETROLIBC_BUFSIZ; i++) c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'b');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->end_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_reads_character_buffer_size_plus_1_times_for_line_buffered(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  size_t i;
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'c');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "cdef", 4), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 1);
  CU_ASSERT_EQUAL(f->end_pos, 4);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_does_not_read_character_because_eof_for_line_buffered(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  errno = 0;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(c, RETROLIBC_EOF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_reads_character_for_unbuffered(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
}

static void test_getc_unlocked_reads_character_3_times_for_unbuffered(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'b');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'c');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
}

static void test_getc_unlocked_reads_character_for_newlines_and_unbuffered(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abc\ndef\n"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
}

static void test_getc_unlocked_does_not_read_character_because_eof_for_unbuffered(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  errno = 0;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
}

static void test_getc_unlocked_reads_character_after_putc_unlocked(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r+");
  retrolibc_putc_unlocked('A', f);
  retrolibc_putc_unlocked('B', f);
  retrolibc_putc_unlocked('C', f);
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'd');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "def", 3), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 1);
  CU_ASSERT_EQUAL(f->end_pos, 3);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "ABCdef");  
}

static void test_getc_unlocked_reads_character_after_putc_unlocked_buffer_size_plus_1_times(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r+");
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) retrolibc_putc_unlocked(buf[i] = toupper(buf[i]), f);
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'd');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "def", 3), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 1);
  CU_ASSERT_EQUAL(f->end_pos, 3);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, buf);
}

static void test_getc_unlocked_returns_eof_because_eof_flag(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_EOF;
  errno = 0;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_EOF);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_complains_on_error_flag(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_ERROR;
  errno = 0;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_complains_on_bad_fd(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  close(f->fd);
  errno = 0;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_complains_on_bad_fd_for_line_buffered(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  close(f->fd);
  errno = 0;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_getc_unlocked_complains_on_bad_fd_for_unbuffered(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  close(f->fd);
  errno = 0;
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
}

static void test_putc_unlocked_writes_character(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "a", 1), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 1);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_putc_unlocked_writes_character_3_times(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "a", 1), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 1);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('b', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "ab", 2), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 2);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('c', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abc", 3), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 3);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_putc_unlocked_writes_character_for_newline(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('\n', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abc\n", 4), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 4);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_putc_unlocked_writes_character_buffer_size_times(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  int r;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "w");
  for(i = 0; i < RETROLIBC_BUFSIZ; i++) r = retrolibc_putc_unlocked(buf[i], f);
  buf[i] = '\0';
  CU_ASSERT_EQUAL(r, 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, buf); 
}

static void test_putc_unlocked_writes_character_buffer_size_plus_1_times(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  int r;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "w");
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) r = retrolibc_putc_unlocked(buf[i], f);
  buf[i - 1] = '\0';
  CU_ASSERT_EQUAL(r, 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "c", 1), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 1);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, buf); 
}

static void test_putc_unlocked_writes_character_for_line_buffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "a", 1), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 1);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_putc_unlocked_writes_character_3_times_for_line_buffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "a", 1), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 1);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('b', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "ab", 2), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 2);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('c', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abc", 3), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 3);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_putc_unlocked_writes_character_for_newline_and_line_buffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('\n', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abc\n"); 
}

static void test_putc_unlocked_writes_character_buffer_size_times_for_line_buffered(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  int r;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  for(i = 0; i < RETROLIBC_BUFSIZ; i++) r = retrolibc_putc_unlocked(buf[i], f);
  buf[i] = '\0';
  CU_ASSERT_EQUAL(r, 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, buf); 
}

static void test_putc_unlocked_writes_character_buffer_size_plus_1_times_for_line_buffered(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  int r;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) r = retrolibc_putc_unlocked(buf[i], f);
  buf[i - 1] = '\0';
  CU_ASSERT_EQUAL(r, 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "c", 1), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 1);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, buf); 
}

static void test_putc_unlocked_writes_character_for_unbuffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "a");  
}

static void test_putc_unlocked_writes_character_3_times_for_unbuffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('b', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('c', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abc");  
}

static void test_putc_unlocked_writes_character_for_newline_and_unbuffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('\n', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abc\n");  
}

static void test_putc_unlocked_writes_character_after_getc_unlocked(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r+");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('A', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "A", 1), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 1);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 3);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abcdef");
}

static void test_putc_unlocked_writes_character_after_getc_unlocked_buffer_size_plus_1_times(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r+");
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('D', f), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "D", 1), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 1);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(get_file_pos(f->fd), RETROLIBC_BUFSIZ + 1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, buf);
}

static void test_putc_unlocked_writes_character_buffer_size_times_after_getc_unlocked_for_larger_file_than_buffer(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 4];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  int r;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r+");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  for(i = 0; i < RETROLIBC_BUFSIZ; i++) r = retrolibc_putc_unlocked(buf[i + 3] = toupper(buf[i + 3]), f);
  CU_ASSERT_EQUAL(r, 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf + 3, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(get_file_pos(f->fd), RETROLIBC_BUFSIZ + 3);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, buf);
}

static void test_putc_unlocked_complains_on_error_flag(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_ERROR;
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_putc_unlocked_complains_on_bad_fd(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  int r;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  f = retrolibc_fopen("test.txt", "w");
  close(f->fd);
  errno = 0;
  for(i = 0; i < RETROLIBC_BUFSIZ; i++) r = retrolibc_putc_unlocked(buf[i] = toupper(buf[i]), f);
  CU_ASSERT_EQUAL(r, RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->pid, -1);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");
}

static void test_putc_unlocked_complains_on_bad_fd_for_line_buffered(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  int r;
  clean_current_dir();
  initalize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  close(f->fd);
  errno = 0;
  for(i = 0; i < RETROLIBC_BUFSIZ; i++) r = retrolibc_putc_unlocked(buf[i], f);
  CU_ASSERT_EQUAL(r, RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED | FILE_FLAG_DATA_TO_WRITE  | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, buf, RETROLIBC_BUFSIZ), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");
}

static void test_putc_unlocked_complains_on_bad_fd_for_unbuffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initalize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  close(f->fd);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, 0);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");
}

int add_file_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("file suite", initialize_file, cleanup_file)) == NULL) return -1;
  if(CU_add_test(suite, "__mode_to_oflags converts mode to open flags", test___mode_to_oflags_converts_mode_to_open_flags) == NULL) return -1;
  if(CU_add_test(suite, "__mode_to_oflags complains on invalid mode", test___mode_to_oflags_complains_on_invalid_mode) == NULL) return -1;
  if(CU_add_test(suite, "fopen opens file to read", test_fopen_opens_file_to_read) == NULL) return -1;
  if(CU_add_test(suite, "fopen opens file to write", test_fopen_opens_file_to_write) == NULL) return -1;
  if(CU_add_test(suite, "fopen opens complains on not found", test_fopen_complains_on_not_found) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks", test_fread_reads_blocks) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks for newlines", test_fread_reads_blocks_for_newlines) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks after getc_unclocked", test_fread_reads_blocks_after_getc_unlocked) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks after getc_unclocked for newlines", test_fread_reads_blocks_after_getc_unlocked_for_newlines) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks after getc_unclocked buffer size - 1 times", test_fread_reads_blocks_after_getc_unlocked_buffer_size_minus_1_times) == NULL) return -1;
  if(CU_add_test(suite, "fread reads 1 block of 2 blocks", test_fread_reads_1_block_of_2_blocks) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks for line buffered", test_fread_reads_blocks_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks for newlines and line buffered", test_fread_reads_blocks_for_newlines_and_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks after getc_unclocked for line buffered", test_fread_reads_blocks_after_getc_unlocked_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks after getc_unclocked for newlines and line buffered", test_fread_reads_blocks_after_getc_unlocked_for_newlines_and_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks after getc_unclocked buffer size - 1 times for line buffered", test_fread_reads_blocks_after_getc_unlocked_buffer_size_minus_1_times_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fread reads 1 block of 2 blocks for line buffered", test_fread_reads_1_block_of_2_blocks_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks for unbuffered", test_fread_reads_blocks_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks for newlines and unbuffered", test_fread_reads_blocks_for_newlines_and_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "fread reads 1 block of 2 blocks for unbuffered", test_fread_reads_1_block_of_2_blocks_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks after putc_unlocked", test_fread_reads_blocks_after_putc_unlocked) == NULL) return -1;
  if(CU_add_test(suite, "fread returns 0 because EOF flag", test_fread_returns_0_because_eof_flag) == NULL) return -1;
  if(CU_add_test(suite, "fread returns 0 because error flag", test_fread_returns_0_because_error_flag) == NULL) return -1;
  if(CU_add_test(suite, "fread complains on bad fd", test_fread_complains_on_bad_fd) == NULL) return -1;
  if(CU_add_test(suite, "fread complains on bad fd for line buffered", test_fread_complains_on_bad_fd_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fread complains on bad fd for unbuffered", test_fread_complains_on_bad_fd_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks", test_fwrite_writes_blocks) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks for newlines", test_fwrite_writes_blocks_for_newlines) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks after putc_unlocked", test_fwrite_writes_blocks_after_putc_unlocked) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks after putc_unlocked for newlines", test_fwrite_writes_blocks_after_putc_unlocked_for_newlines) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks after putc_unlocked buffer size - 1 times", test_fwrite_writes_blocks_after_putc_unlocked_buffer_size_minus_1_times) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks for line buffered", test_fwrite_writes_blocks_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks for newlines and line buffered", test_fwrite_writes_blocks_for_newlines_and_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks after putc_unlocked for line buffered", test_fwrite_writes_blocks_after_putc_unlocked_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks after putc_unlocked for newlines and line buffered", test_fwrite_writes_blocks_after_putc_unlocked_for_newlines_and_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks after putc_unlocked buffer size - 1 times for line buffered", test_fwrite_writes_blocks_after_putc_unlocked_buffer_size_minus_1_times_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks for unbuffered", test_fwrite_writes_blocks_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks for newlines and unbuffered", test_fwrite_writes_blocks_for_newlines_and_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks after getc_unlocked", test_fwrite_writes_blocks_after_getc_unlocked) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks after getc_unlocked buffer size + 1 times", test_fwrite_writes_blocks_after_getc_unlocked_buffer_size_plus_1_times) == NULL) return -1;
  if(CU_add_test(suite, "fwrite writes blocks with buffer size / 2 after getc_unlocked for larger file than buffer", test_fwrite_writes_blocks_with_buffer_size_div_2_after_getc_unlocked_for_larger_file_than_buffer) == NULL) return -1;
  if(CU_add_test(suite, "fwrite returns 0 because error flag", test_fwrite_returns_0_because_error_flag) == NULL) return -1;
  if(CU_add_test(suite, "fwrite complains on bad fd", test_fwrite_complains_on_bad_fd) == NULL) return -1;
  if(CU_add_test(suite, "fwrite complains on bad fd for line buffered", test_fwrite_complains_on_bad_fd_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fwrite complains on bad fd for unbuffered", test_fwrite_complains_on_bad_fd_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character", test_getc_unlocked_reads_character) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character 3 times", test_getc_unlocked_reads_character_3_times) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character for newlines", test_getc_unlocked_reads_character_for_newlines) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character for larger file than buffer", test_getc_unlocked_reads_character_for_larger_file_than_buffer) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character buffer size times", test_getc_unlocked_reads_character_buffer_size_times) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character buffer size + 1 times", test_getc_unlocked_reads_character_buffer_size_plus_1_times) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked doesn't read character because EOF", test_getc_unlocked_does_not_read_character_because_eof) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character for line buffered", test_getc_unlocked_reads_character_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character 3 times for line buffered", test_getc_unlocked_reads_character_3_times_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character for newlines and line buffered", test_getc_unlocked_reads_character_for_newlines_and_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character for larger file than buffer and line buffered", test_getc_unlocked_reads_character_for_larger_file_than_buffer_and_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character buffer size times for line buffered", test_getc_unlocked_reads_character_buffer_size_times_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character buffer size + 1 times for line buffered", test_getc_unlocked_reads_character_buffer_size_plus_1_times_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked doesn't read character because EOF for line buffered", test_getc_unlocked_does_not_read_character_because_eof_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character for unbuffered", test_getc_unlocked_reads_character_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character 3 times for unbuffered", test_getc_unlocked_reads_character_3_times_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character for newlines and unbuffered", test_getc_unlocked_reads_character_for_newlines_and_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked doesn't read character because EOF for unbuffered", test_getc_unlocked_does_not_read_character_because_eof_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character after putc_unlocked", test_getc_unlocked_reads_character_after_putc_unlocked) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked reads character after putc_unlocked buffer size + 1 times", test_getc_unlocked_reads_character_after_putc_unlocked_buffer_size_plus_1_times) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked returns EOF because EOF flag", test_getc_unlocked_returns_eof_because_eof_flag) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked complains on error flag", test_getc_unlocked_complains_on_error_flag) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked complains on bad fd", test_getc_unlocked_complains_on_bad_fd) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked complains on bad fd for line buffered", test_getc_unlocked_complains_on_bad_fd_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked complains on bad fd for unbuffered", test_getc_unlocked_complains_on_bad_fd_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character", test_putc_unlocked_writes_character) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character 3 times", test_putc_unlocked_writes_character_3_times) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character for newline", test_putc_unlocked_writes_character_for_newline) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character buffer size times", test_putc_unlocked_writes_character_buffer_size_times) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character buffer size + 1 times", test_putc_unlocked_writes_character_buffer_size_plus_1_times) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character for line buffered", test_putc_unlocked_writes_character_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character 3 times for line buffered", test_putc_unlocked_writes_character_3_times_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character for newline and line buffered", test_putc_unlocked_writes_character_for_newline_and_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character buffer size times for  line buffered", test_putc_unlocked_writes_character_buffer_size_times_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character buffer size + 1 times for line buffered", test_putc_unlocked_writes_character_buffer_size_plus_1_times_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character for unbuffered", test_putc_unlocked_writes_character_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character 3 times for unbuffered", test_putc_unlocked_writes_character_3_times_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character for newline and unbuffered", test_putc_unlocked_writes_character_for_newline_and_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character after getc_unlocked", test_putc_unlocked_writes_character_after_getc_unlocked) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character after getc_unlocked buffer size + 1 times", test_putc_unlocked_writes_character_after_getc_unlocked_buffer_size_plus_1_times) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked writes character buffer size times after getc_unlocked for larger file than buffer", test_putc_unlocked_writes_character_buffer_size_times_after_getc_unlocked_for_larger_file_than_buffer) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked complains on error flag", test_putc_unlocked_complains_on_error_flag) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked complains on bad fd", test_putc_unlocked_complains_on_bad_fd) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked complains on bad fd for line buffered", test_putc_unlocked_complains_on_bad_fd_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "putc_unlocked complains on bad fd for unbuffered", test_putc_unlocked_complains_on_bad_fd_for_unbuffered) == NULL) return -1;
  return 0;
}

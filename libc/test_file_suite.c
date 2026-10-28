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

static void initialize_retrolibc_file(void)
{
  retrolibc___first_file = NULL;
  retrolibc___last_file = NULL;
  retrolibc_stdin->fd = STDIN_FILENO;
  retrolibc_stdin->cur_pos = retrolibc_stdin->end_pos = 0;
  retrolibc_stdout->fd = STDOUT_FILENO;
  retrolibc_stdout->cur_pos = retrolibc_stdout->end_pos = 0;
  retrolibc_stderr->fd = STDERR_FILENO;
  retrolibc_stderr->cur_pos = retrolibc_stderr->end_pos = 0;
}

static int initialize_file(void)
{ return 0; }

static int cleanup_file(void)
{
  clean_current_dir();
  return 0;
}

static void test___add_file_adds_file(void)
{
  retrolibc_FILE f;
  clean_current_dir();
  initialize_retrolibc_file();
  retrolibc___add_file(&f);
  CU_ASSERT_PTR_NULL(f.prev);
  CU_ASSERT_PTR_NULL(f.next);
  CU_ASSERT_PTR_EQUAL(retrolibc___first_file, &f);
  CU_ASSERT_PTR_EQUAL(retrolibc___last_file, &f);
}

static void test___add_file_adds_file_3_times(void)
{
  retrolibc_FILE f;
  retrolibc_FILE f2;
  retrolibc_FILE f3;
  clean_current_dir();
  initialize_retrolibc_file();
  retrolibc___add_file(&f);
  CU_ASSERT_PTR_NULL(f.prev);
  CU_ASSERT_PTR_NULL(f.next);
  CU_ASSERT_PTR_EQUAL(retrolibc___first_file, &f);
  CU_ASSERT_PTR_EQUAL(retrolibc___last_file, &f);
  retrolibc___add_file(&f2);
  CU_ASSERT_PTR_NULL(f.prev);
  CU_ASSERT_PTR_EQUAL(f.next, &f2);
  CU_ASSERT_PTR_EQUAL(f2.prev, &f);
  CU_ASSERT_PTR_NULL(f2.next);
  CU_ASSERT_PTR_EQUAL(retrolibc___first_file, &f);
  CU_ASSERT_PTR_EQUAL(retrolibc___last_file, &f2);
  retrolibc___add_file(&f3);
  CU_ASSERT_PTR_NULL(f.prev);
  CU_ASSERT_PTR_EQUAL(f.next, &f2);
  CU_ASSERT_PTR_EQUAL(f2.prev, &f);
  CU_ASSERT_PTR_EQUAL(f2.next, &f3);
  CU_ASSERT_PTR_EQUAL(f3.prev, &f2);
  CU_ASSERT_PTR_NULL(f3.next);
  CU_ASSERT_PTR_EQUAL(retrolibc___first_file, &f);
  CU_ASSERT_PTR_EQUAL(retrolibc___last_file, &f3);
}

static void test___remove_file_removes_file(void)
{
  retrolibc_FILE f;
  clean_current_dir();
  initialize_retrolibc_file();
  retrolibc___add_file(&f);
  retrolibc___remove_file(&f);
  CU_ASSERT_PTR_NULL(f.prev);
  CU_ASSERT_PTR_NULL(f.next);
  CU_ASSERT_PTR_NULL(retrolibc___first_file);
  CU_ASSERT_PTR_NULL(retrolibc___last_file);
}

static void test___remove_file_removes_file_for_left(void)
{
  retrolibc_FILE f;
  retrolibc_FILE f2;
  retrolibc_FILE f3;
  clean_current_dir();
  initialize_retrolibc_file();
  retrolibc___add_file(&f);
  retrolibc___add_file(&f2);
  retrolibc___add_file(&f3);
  retrolibc___remove_file(&f);
  CU_ASSERT_PTR_NULL(f2.prev);
  CU_ASSERT_PTR_EQUAL(f2.next, &f3);
  CU_ASSERT_PTR_EQUAL(f3.prev, &f2);
  CU_ASSERT_PTR_NULL(f3.next);
  CU_ASSERT_PTR_EQUAL(retrolibc___first_file, &f2);
  CU_ASSERT_PTR_EQUAL(retrolibc___last_file, &f3);
}

static void test___remove_file_removes_file_for_center(void)
{
  retrolibc_FILE f;
  retrolibc_FILE f2;
  retrolibc_FILE f3;
  clean_current_dir();
  initialize_retrolibc_file();
  retrolibc___add_file(&f);
  retrolibc___add_file(&f2);
  retrolibc___add_file(&f3);
  retrolibc___remove_file(&f2);
  CU_ASSERT_PTR_NULL(f.prev);
  CU_ASSERT_PTR_EQUAL(f.next, &f3);
  CU_ASSERT_PTR_EQUAL(f3.prev, &f);
  CU_ASSERT_PTR_NULL(f3.next);
  CU_ASSERT_PTR_EQUAL(retrolibc___first_file, &f);
  CU_ASSERT_PTR_EQUAL(retrolibc___last_file, &f3);
}

static void test___remove_file_removes_file_for_right(void)
{
  retrolibc_FILE f;
  retrolibc_FILE f2;
  retrolibc_FILE f3;
  clean_current_dir();
  initialize_retrolibc_file();
  retrolibc___add_file(&f);
  retrolibc___add_file(&f2);
  retrolibc___add_file(&f3);
  retrolibc___remove_file(&f3);
  CU_ASSERT_PTR_NULL(f.prev);
  CU_ASSERT_PTR_EQUAL(f.next, &f2);
  CU_ASSERT_PTR_EQUAL(f2.prev, &f);
  CU_ASSERT_PTR_NULL(f2.next);
  CU_ASSERT_PTR_EQUAL(retrolibc___first_file, &f);
  CU_ASSERT_PTR_EQUAL(retrolibc___last_file, &f2);
}

static void test___fflush_unlocked_sets_offset_for_fflush_flag_can_seek(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc___fflush_unlocked(f, FFLUSH_FLAG_CAN_SEEK), 0);
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
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 3);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test___fflush_unlocked_does_not_set_offset_for_0(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc___fflush_unlocked(f, 0), 0)
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->cur_pos, 3);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 6);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test___fflush_unlocked_does_not_set_offset_for_fflush_flag_can_write(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc___fflush_unlocked(f, FFLUSH_FLAG_CAN_WRITE), 0)
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->cur_pos, 3);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 6);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test___fflush_unlocked_writes_data_for_fflush_flag_can_write(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc___fflush_unlocked(f, FFLUSH_FLAG_CAN_WRITE), 0);
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
  CU_ASSERT_STRING_EQUAL(s, "abc");  
}

static void test___fflush_unlocked_does_not_write_data_for_0(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc___fflush_unlocked(f, 0), 0);
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

static void test___fflush_unlocked_does_not_write_data_for_fflush_flag_can_seek(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc___fflush_unlocked(f, FFLUSH_FLAG_CAN_SEEK), 0);
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

static void test___fflush_unlocked_sets_offset_for_fully_readed_buffer(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc___fflush_unlocked(f, FFLUSH_FLAG_CAN_SEEK), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 6);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 6);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test___fflush_unlocked_sets_offset_for_empty_buffer(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc___fflush_unlocked(f, FFLUSH_FLAG_CAN_SEEK), 0);
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
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 0);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test___fflush_unlocked_does_not_set_offset_for_unbuffered(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc___fflush_unlocked(f, FFLUSH_FLAG_CAN_SEEK), 0);
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
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 3);
  close(f->fd);
  free(f);
}

static void test___fflush_unlocked_writes_data_for_empty_buffer(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc___fflush_unlocked(f, FFLUSH_FLAG_CAN_WRITE), 0);
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

static void test___fflush_unlocked_does_not_write_data_for_unbuffered(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc___fflush_unlocked(f, FFLUSH_FLAG_CAN_WRITE), 0);
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

static void test___fflush_unlocked_complains_on_bad_fd_for_fflush_flag_can_seek(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  close(f->fd);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc___fflush_unlocked(f, FFLUSH_FLAG_CAN_SEEK), RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 3);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  free(f->buf);
  free(f);
}

static void test___fflush_unlocked_complains_on_bad_fd_for_fflush_flag_can_write(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  close(f->fd);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc___fflush_unlocked(f, FFLUSH_FLAG_CAN_WRITE), RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abc", 3), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 3);
  CU_ASSERT_EQUAL(f->pid, -1);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test___fdclose_unlocked_closes_fd_for_file_to_read(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  CU_ASSERT_NOT_EQUAL(retrolibc___fdclose_unlocked(f), RETROLIBC_EOF);
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
  free(f->buf);
  free(f);
}

static void test___fdclose_unlocked_closes_fd_for_file_to_write(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  CU_ASSERT_NOT_EQUAL(retrolibc___fdclose_unlocked(f), RETROLIBC_EOF);
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
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test___fdclose_unlocked_closes_fd_and_waits_for_process_for_pipe(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_popen("echo -n", "r");
  CU_ASSERT_PTR_NOT_NULL(f);
  CU_ASSERT_NOT_EQUAL(retrolibc___fdclose_unlocked(f), RETROLIBC_EOF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_NOT_EQUAL(f->pid, -1);
  free(f->buf);
  free(f);
}

static void test___fdclose_unlocked_complains_on_bad_fd(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  close(f->fd);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc___fdclose_unlocked(f), RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
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
  free(f->buf);
  free(f);
}

static void test___init_file_initializes_file(void)
{
  retrolibc_FILE f;
  clean_current_dir();
  initialize_retrolibc_file();
  retrolibc___init_file(&f);
  CU_ASSERT_EQUAL(f.fd, -1);
  CU_ASSERT_EQUAL(f.flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f.pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f.buf);
  CU_ASSERT_EQUAL(f.buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f.cur_pos, 0);
  CU_ASSERT_EQUAL(f.end_pos, 0);
  CU_ASSERT_EQUAL(f.pid, -1);
  free(f.buf);
}

static void test___mode_to_oflags_converts_mode_to_open_flags(void)
{
  clean_current_dir();
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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

static void test_clearerr_clears_0(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_EOF;
  retrolibc_clearerr(f);
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

static void test_clearerr_clears_eof_flag(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_EOF;
  retrolibc_clearerr(f);
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

static void test_clearerr_clears_error_flag(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_ERROR;
  retrolibc_clearerr(f);
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

static void test_clearerr_clears_eof_flag_and_error_flag(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_EOF | FILE_FLAG_ERROR;
  retrolibc_clearerr(f);
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

static void test_fclose_closes_file_to_read(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_fclose(f), 0);
}

static void test_fclose_closes_file_to_write(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_fclose(f), 0);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_fclose_flushes(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_fclose(f), 0);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abc");  
}

static void test_fclose_complains_on_bad_fd(void)
{
  retrolibc_FILE *f;
  int r;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  close(f->fd);
  errno = 0;
  r = retrolibc_fclose(f);
  CU_ASSERT_EQUAL(r, RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  if(r == RETROLIBC_EOF) {
    free(f->buf);
    free(f);
  }
}

static void test_fdopen_opens_file_with_fd_to_read(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fdopen(5, "r");
  CU_ASSERT_PTR_NOT_NULL(f);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_EQUAL(f->fd, 5);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  free(f->buf);
  free(f);
}

static void test_fdopen_opens_file_with_fd_to_write(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fdopen(5, "w");
  CU_ASSERT_PTR_NOT_NULL(f);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_EQUAL(f->fd, 5);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  free(f->buf);
  free(f);
}

static void test_feof_returns_0(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_feof(f), 0);
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

static void test_feof_returns_1(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_EOF;
  CU_ASSERT_EQUAL(retrolibc_feof(f), 1);
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

static void test_feof_returns_0_for_error_flag(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_ERROR;
  CU_ASSERT_EQUAL(retrolibc_feof(f), 0);
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

static void test_ferror_returns_0(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_ferror(f), 0);
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

static void test_ferror_returns_1(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_ERROR;
  CU_ASSERT_EQUAL(retrolibc_ferror(f), 1);
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

static void test_ferror_returns_0_for_eof_flag(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f->flags |= FILE_FLAG_EOF;
  CU_ASSERT_EQUAL(retrolibc_ferror(f), 0);
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

static void test_fflush_sets_offset(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_fflush(f), 0);
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
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 3);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fflush_writes_data(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_fflush(f), 0);
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
  CU_ASSERT_STRING_EQUAL(s, "abc");  
}

static void test_fflush_flushes_all_files(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  retrolibc_FILE *f2;
  retrolibc_FILE *f3;
  clean_current_dir();
  initialize_retrolibc_file();
  retrolibc_stdin->fd = creat("test_stdin.txt", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  retrolibc_putc_unlocked('A', retrolibc_stdin);
  retrolibc_putc_unlocked('B', retrolibc_stdin);
  retrolibc_putc_unlocked('C', retrolibc_stdin);
  retrolibc_stdout->fd = creat("test_stdout.txt", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  retrolibc_putc_unlocked('D', retrolibc_stdout);
  retrolibc_putc_unlocked('E', retrolibc_stdout);
  retrolibc_putc_unlocked('F', retrolibc_stdout);
  retrolibc_stderr->fd = creat("test_stderr.txt", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  retrolibc_putc_unlocked('G', retrolibc_stderr);
  retrolibc_putc_unlocked('H', retrolibc_stderr);
  retrolibc_putc_unlocked('I', retrolibc_stderr);
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  f2 = retrolibc_fopen("test2.txt", "w");
  retrolibc_putc_unlocked('d', f2);
  retrolibc_putc_unlocked('e', f2);
  retrolibc_putc_unlocked('f', f2);
  f3 = retrolibc_fopen("test3.txt", "w");
  retrolibc_putc_unlocked('g', f3);
  retrolibc_putc_unlocked('h', f3);
  retrolibc_putc_unlocked('i', f3);
  CU_ASSERT_EQUAL(retrolibc_fflush(NULL), 0);
  close(f3->fd);
  free(f3->buf);
  free(f3);
  close(f2->fd);
  free(f2->buf);
  free(f2);
  close(f->fd);
  free(f->buf);
  free(f);
  close(retrolibc_stderr->fd);
  close(retrolibc_stdout->fd);
  close(retrolibc_stdin->fd);
  CU_ASSERT_FATAL(read_from_file("test_stdin.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "ABC");  
  CU_ASSERT_FATAL(read_from_file("test_stdout.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "DEF");  
  CU_ASSERT_FATAL(read_from_file("test_stderr.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "GHI");  
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abc");  
  CU_ASSERT_FATAL(read_from_file("test2.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "def");  
  CU_ASSERT_FATAL(read_from_file("test3.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "ghi");  
}

static void test_fflush_complains_on_bad_fd(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  close(f->fd);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_fflush(f), RETROLIBC_EOF);
  CU_ASSERT_EQUAL(errno, EBADF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE | FILE_FLAG_ERROR);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abc", 3), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 3);
  CU_ASSERT_EQUAL(f->pid, -1);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_fileno_returns_fd(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_fileno(f), f->fd);
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

static void test_fopen_opens_file_to_read(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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

static void test_fread_reads_block_after_putc_unlocked_buffer_size_plus_1_times(void)
{
  static char block[3];
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r+");
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) retrolibc_putc_unlocked(buf[i] = toupper(buf[i]), f);
  CU_ASSERT_EQUAL(retrolibc_fread(block, 3, 1, f), 1);
  CU_ASSERT_EQUAL(memcmp(block, "def", 3), 0); 
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
  CU_ASSERT_STRING_EQUAL(s, buf);
}

static void test_fread_reads_blocks_after_storing_to_pushed_character(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_ungetc('A', f);
  CU_ASSERT_EQUAL(retrolibc_fread(blocks, 3, 2, f), 2);
  CU_ASSERT_EQUAL(memcmp(blocks, "Aabcde", 6), 0);
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

static void test_fread_returns_0_because_eof_flag(void)
{
  static char blocks[3 * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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

static void test_freopen_closes_old_file_and_opens_new_file_to_read(void)
{
  char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  retrolibc_FILE *f2;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test2.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "w");
  f2 = retrolibc_freopen(f, "test2.txt", "r");
  CU_ASSERT_PTR_EQUAL(f2, f);
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

static void test_freopen_closes_old_file_and_opens_new_file_to_write(void)
{
  char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  retrolibc_FILE *f2;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  f2 = retrolibc_freopen(f, "test2.txt", "w");
  CU_ASSERT_PTR_EQUAL(f2, f);
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
  CU_ASSERT_FATAL(read_from_file("test2.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "");  
}

static void test_freopen_flushes(void)
{
  char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  retrolibc_FILE *f2;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test2.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  f2 = retrolibc_freopen(f, "test2.txt", "r");
  CU_ASSERT_PTR_EQUAL(f2, f);
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
  CU_ASSERT_STRING_EQUAL(s, "abc");  
}

static void test_freopen_opens_file_after_not_found(void)
{
  char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  retrolibc_FILE *f2;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test3.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_freopen(f, "test2.txt", "r");
  f2 = retrolibc_freopen(f, "test3.txt", "r");
  CU_ASSERT_PTR_NOT_NULL(f2);
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

static void test_freopen_complains_on_not_found(void)
{
  char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  retrolibc_FILE *f2;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  f2 = retrolibc_freopen(f, "test2.txt", "r");
  CU_ASSERT_PTR_NULL(f2);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_EQUAL(f->fd, -1);
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

static void test_fseeko_sets_offset_for_seek_set(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_FATAL(set_file_pos(f->fd, 4));
  CU_ASSERT_EQUAL(retrolibc_fseeko(f, 3, SEEK_SET), 0);
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
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 3);
  close(f->fd);
  free(f);
}

static void test_fseeko_sets_offset_for_seek_cur_and_positive(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_FATAL(set_file_pos(f->fd, 4));
  CU_ASSERT_EQUAL(retrolibc_fseeko(f, 3, SEEK_CUR), 0);
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
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 7);
  close(f->fd);
  free(f);
}

static void test_fseeko_sets_offset_for_seek_cur_and_negative(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_FATAL(set_file_pos(f->fd, 4));
  CU_ASSERT_EQUAL(retrolibc_fseeko(f, -2, SEEK_CUR), 0);
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
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 2);
  close(f->fd);
  free(f);
}

static void test_fseeko_sets_offset_for_seek_end(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_FATAL(set_file_pos(f->fd, 4));
  CU_ASSERT_EQUAL(retrolibc_fseeko(f, -3, SEEK_END), 0);
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
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 6);
  close(f->fd);
  free(f);
}

static void test_fseeko_sets_offset_after_getc_unlocked(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_fseeko(f, 2, SEEK_CUR), 0);
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
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 6);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fseeko_sets_offset_after_getc_unlocked_buffer_size_plus_1_times(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r");
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_fseeko(f, -3, SEEK_CUR), 0);
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
  CU_ASSERT_EQUAL(get_file_pos(f->fd), RETROLIBC_BUFSIZ - 2);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_fseeko_sets_offset_after_putc_unlocked(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = retrolibc_fopen("test.txt", "r+");
  retrolibc_putc_unlocked('A', f);
  retrolibc_putc_unlocked('B', f);
  retrolibc_putc_unlocked('C', f);
  retrolibc_putc_unlocked('D', f);
  CU_ASSERT_EQUAL(retrolibc_fseeko(f, 2, SEEK_CUR), 0);
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
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 6);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "ABCDefghi");  
}

static void test_fseeko_sets_offset_after_putc_unlocked_buffer_size_plus_1_times(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r+");
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) retrolibc_putc_unlocked(buf[i] = toupper(buf[i]), f);
  CU_ASSERT_EQUAL(retrolibc_fseeko(f, -3, SEEK_CUR), 0);
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
  CU_ASSERT_EQUAL(get_file_pos(f->fd), RETROLIBC_BUFSIZ - 2);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, buf);
}

static void test_fseeko_complains_on_bad_fd(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  close(f->fd);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_fseeko(f, 0, SEEK_CUR), -1);
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

static void test_ftello_returns_offset(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_FATAL(set_file_pos(f->fd, 4));
  CU_ASSERT_EQUAL(retrolibc_ftello(f), 4);
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

static void test_ftello_returns_offset_after_getc_unlocked(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_ftello(f), 4);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 4);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_ftello_returns_offset_after_getc_unlocked_buffer_size_plus_1_times(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r");
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_ftello(f), RETROLIBC_BUFSIZ + 1);
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

static void test_ftello_returns_offset_after_putc_unlocked(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r+");
  retrolibc_putc_unlocked('A', f);
  retrolibc_putc_unlocked('B', f);
  retrolibc_putc_unlocked('C', f);
  retrolibc_putc_unlocked('D', f);
  CU_ASSERT_EQUAL(retrolibc_ftello(f), 4);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "ABCD", 4), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 4);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abcdef");  
}

static void test_ftello_returns_offset_after_putc_unlocked_buffer_size_plus_1_times(void)
{
  static char buf[((RETROLIBC_BUFSIZ + 5) / 6) * 6 + 1];
  static char s[RETROLIBC_BUFSIZ * 2];
  size_t i;
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r+");
  for(i = 0; i < RETROLIBC_BUFSIZ; i++) retrolibc_putc_unlocked(buf[i] = toupper(buf[i]), f);
  retrolibc_putc_unlocked(toupper(buf[i]), f);
  CU_ASSERT_EQUAL(retrolibc_ftello(f), RETROLIBC_BUFSIZ + 1);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "C", 1), 0);
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

static void test_ftello_complains_on_bad_fd(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  close(f->fd);
  errno = 0;
  CU_ASSERT_EQUAL(retrolibc_ftello(f), -1);
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

static void test_fwrite_writes_blocks(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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

static void test_getc_unlocked_reads_character_after_storing_to_pushed_character(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_ungetc('A', f);
  c = retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(c, 'A');
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

static void test_getc_unlocked_returns_eof_because_eof_flag(void)
{
  retrolibc_FILE *f;
  int c;
  clean_current_dir();
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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

static void test_popen_opens_pipe_and_fread_reads_and_fclose_closes(void)
{
  char block[7];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_popen("echo abcdef", "r");
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
  CU_ASSERT_NOT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(retrolibc_fread(block, 7, 1, f), 1);
  CU_ASSERT_EQUAL(memcmp(block, "abcdef\n", 7), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_NOT_EQUAL(f->pid, -1);
  CU_ASSERT_NOT_EQUAL(retrolibc_pclose(f), -1);
}

static void test_popen_opens_pipe_and_fwrite_writes_and_fclose_closes(void)
{
  char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_popen("cat > test.txt", "w");
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
  CU_ASSERT_NOT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(retrolibc_fwrite("abcdef\n", 7, 1, f), 1);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_DATA_TO_WRITE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 7);
  CU_ASSERT_NOT_EQUAL(f->pid, -1);
  CU_ASSERT_NOT_EQUAL(retrolibc_pclose(f), -1);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abcdef\n");
}

static void test_putc_unlocked_writes_character(void)
{
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), 'a');
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
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), 'a');
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
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('b', f), 'b');
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
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('c', f), 'c');
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
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('\n', f), '\n');
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
  initialize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "w");
  for(i = 0; i < RETROLIBC_BUFSIZ; i++) r = retrolibc_putc_unlocked(buf[i], f);
  buf[i] = '\0';
  CU_ASSERT_EQUAL(r, 'b');
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
  initialize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "w");
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) r = retrolibc_putc_unlocked(buf[i], f);
  buf[i - 1] = '\0';
  CU_ASSERT_EQUAL(r, 'c');
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
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), 'a');
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
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), 'a');
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
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('b', f), 'b');
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
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('c', f), 'c');
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
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('\n', f), '\n');
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
  initialize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  for(i = 0; i < RETROLIBC_BUFSIZ; i++) r = retrolibc_putc_unlocked(buf[i], f);
  buf[i] = '\0';
  CU_ASSERT_EQUAL(r, 'b');
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
  initialize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "w");
  f->flags |= FILE_FLAG_LINE_BUFFERED;
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) r = retrolibc_putc_unlocked(buf[i], f);
  buf[i - 1] = '\0';
  CU_ASSERT_EQUAL(r, 'c');
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
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), 'a');
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
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('a', f), 'a');
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
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('b', f), 'b');
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
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('c', f), 'c');
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
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  free(f->buf);
  f->flags &= ~FILE_FLAG_FREEABLE_BUFFER;
  f->buf = NULL;
  f->buf_size = 0;
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('\n', f), '\n');
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
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r+");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('A', f), 'A');
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
  initialize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r+");
  for(i = 0; i < RETROLIBC_BUFSIZ + 1; i++) retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_putc_unlocked('D', f), 'D');
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
  initialize_retrolibc_file();
  for(i = 0; i < (RETROLIBC_BUFSIZ + 5) / 6; i++) strcpy(buf + i * 6, "abcdef");
  CU_ASSERT_FATAL(write_to_file("test.txt", buf));
  f = retrolibc_fopen("test.txt", "r+");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  for(i = 0; i < RETROLIBC_BUFSIZ; i++) r = retrolibc_putc_unlocked(buf[i + 3] = toupper(buf[i + 3]), f);
  CU_ASSERT_EQUAL(r, 'E');
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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
  initialize_retrolibc_file();
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

static void test_setvbuf_sets_static_buffer(void)
{
  static char buf[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, buf, RETROLIBC__IOFBF, RETROLIBC_BUFSIZ * 2), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_EQUAL(f->buf, buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ * 2);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
}

static void test_setvbuf_sets_static_buffer_with_line_buffered(void)
{
  static char buf[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, buf, RETROLIBC__IOLBF, RETROLIBC_BUFSIZ * 2), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_EQUAL(f->buf, buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ * 2);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
}

static void test_setvbuf_sets_allocated_buffer(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, NULL, RETROLIBC__IOFBF, RETROLIBC_BUFSIZ * 2), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ * 2);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_setvbuf_sets_allocated_buffer_with_line_buffered(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, NULL, RETROLIBC__IOLBF, RETROLIBC_BUFSIZ * 2), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER | FILE_FLAG_LINE_BUFFERED);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ * 2);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_setvbuf_unsets_buffer(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, NULL, RETROLIBC__IONBF, RETROLIBC_BUFSIZ * 2), 0);
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

static void test_setvbuf_sets_static_buffer_after_setting_static_buffer(void)
{
  static char buf[RETROLIBC_BUFSIZ * 2];
  static char buf2[RETROLIBC_BUFSIZ * 3];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_setvbuf(f, buf, RETROLIBC__IOFBF, RETROLIBC_BUFSIZ * 2);
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, buf2, RETROLIBC__IOFBF, RETROLIBC_BUFSIZ * 3), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_EQUAL(f->buf, buf2);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ * 3);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
}

static void test_setvbuf_sets_static_buffer_after_unsetting_buffer(void)
{
  static char buf[RETROLIBC_BUFSIZ * 2];
  static char buf2[RETROLIBC_BUFSIZ * 3];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_setvbuf(f, buf, RETROLIBC__IONBF, RETROLIBC_BUFSIZ * 2);
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, buf2, RETROLIBC__IOFBF, RETROLIBC_BUFSIZ * 3), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_EQUAL(f->buf, buf2);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ * 3);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
}

static void test_setvbuf_sets_allocated_buffer_after_setting_static_buffer(void)
{
  static char buf[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_setvbuf(f, buf, RETROLIBC__IOFBF, RETROLIBC_BUFSIZ * 2);
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, NULL, RETROLIBC__IOFBF, RETROLIBC_BUFSIZ * 3), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ * 3);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_setvbuf_sets_allocated_buffer_after_unsetting_buffer(void)
{
  static char buf[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_setvbuf(f, buf, RETROLIBC__IONBF, RETROLIBC_BUFSIZ * 2);
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, NULL, RETROLIBC__IOFBF, RETROLIBC_BUFSIZ * 3), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ * 3);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_setvbuf_unsets_buffer_after_setting_static_buffer(void)
{
  static char buf[RETROLIBC_BUFSIZ * 2];
  static char buf2[RETROLIBC_BUFSIZ * 3];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_setvbuf(f, buf, RETROLIBC__IOFBF, RETROLIBC_BUFSIZ * 2);
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, buf2, RETROLIBC__IONBF, RETROLIBC_BUFSIZ * 3), 0);
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

static void test_setvbuf_unsets_buffer_after_unsetting_buffer(void)
{
  static char buf[RETROLIBC_BUFSIZ * 2];
  static char buf2[RETROLIBC_BUFSIZ * 3];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_setvbuf(f, buf, RETROLIBC__IONBF, RETROLIBC_BUFSIZ * 2);
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, buf2, RETROLIBC__IONBF, RETROLIBC_BUFSIZ * 3), 0);
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

static void test_setvbuf_sets_offset_for_setting(void)
{
  static char buf[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, buf, RETROLIBC__IOFBF, RETROLIBC_BUFSIZ * 2), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_EQUAL(f->buf, buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ * 2);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 3);
  close(f->fd);
  free(f);
}

static void test_setvbuf_sets_offset_for_unsetting(void)
{
  static char buf[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, buf, RETROLIBC__IONBF, RETROLIBC_BUFSIZ * 2), 0);
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
  CU_ASSERT_EQUAL(get_file_pos(f->fd), 3);
  close(f->fd);
  free(f);
}

static void test_setvbuf_flushes_for_setting(void)
{
  static char buf[RETROLIBC_BUFSIZ * 2];
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, buf, RETROLIBC__IOFBF, RETROLIBC_BUFSIZ * 2), 0);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_EQUAL(f->buf, buf);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ * 2);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 0);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, RETROLIBC_BUFSIZ * 2));
  CU_ASSERT_STRING_EQUAL(s, "abc");
}

static void test_setvbuf_flushes_for_unsetting(void)
{
  static char buf[RETROLIBC_BUFSIZ * 2];
  static char s[RETROLIBC_BUFSIZ * 2];
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  f = retrolibc_fopen("test.txt", "w");
  retrolibc_putc_unlocked('a', f);
  retrolibc_putc_unlocked('b', f);
  retrolibc_putc_unlocked('c', f);
  CU_ASSERT_EQUAL(retrolibc_setvbuf(f, buf, RETROLIBC__IONBF, RETROLIBC_BUFSIZ * 2), 0);
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

static void test_ungetc_pushes_character_back(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_ungetc('A', f), 'A');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abAdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 2);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_ungetc_pushes_character_back_3_times(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  retrolibc_getc_unlocked(f);
  CU_ASSERT_EQUAL(retrolibc_ungetc('A', f), 'A');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "abAdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 2);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(retrolibc_ungetc('B', f), 'B');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "aBAdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 1);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  CU_ASSERT_EQUAL(retrolibc_ungetc('C', f), 'C');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "CBAdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_ungetc_stores_character_to_pushed_character(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_ungetc('A', f);
  CU_ASSERT_EQUAL(retrolibc_ungetc('B', f), 'B');
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, 'B');
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "Abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_ungetc_does_not_push_character_back_for_eof(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_ungetc('A', f);
  CU_ASSERT_EQUAL(retrolibc_ungetc(RETROLIBC_EOF, f), RETROLIBC_EOF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, -1);
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "Abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

static void test_ungetc_does_not_store_character_to_pushed_character_after_storing(void)
{
  retrolibc_FILE *f;
  clean_current_dir();
  initialize_retrolibc_file();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = retrolibc_fopen("test.txt", "r");
  retrolibc_getc_unlocked(f);
  retrolibc_ungetc('A', f);
  retrolibc_ungetc('B', f);
  CU_ASSERT_EQUAL(retrolibc_ungetc('C', f), RETROLIBC_EOF);
  CU_ASSERT_PTR_NULL(f->prev);
  CU_ASSERT_PTR_NULL(f->next);
  CU_ASSERT_NOT_EQUAL(f->fd, -1);
  CU_ASSERT_EQUAL(f->flags, FILE_FLAG_FREEABLE | FILE_FLAG_FREEABLE_BUFFER);
  CU_ASSERT_EQUAL(f->pushed_char, 'B');
  CU_ASSERT_PTR_NOT_NULL(f->buf);
  CU_ASSERT_EQUAL(memcmp(f->buf, "Abcdef", 6), 0);
  CU_ASSERT_EQUAL(f->buf_size, RETROLIBC_BUFSIZ);
  CU_ASSERT_EQUAL(f->cur_pos, 0);
  CU_ASSERT_EQUAL(f->end_pos, 6);
  CU_ASSERT_EQUAL(f->pid, -1);
  close(f->fd);
  free(f->buf);
  free(f);
}

int add_file_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("file suite", initialize_file, cleanup_file)) == NULL) return -1;
  if(CU_add_test(suite, "__add_file adds file", test___add_file_adds_file) == NULL) return -1;
  if(CU_add_test(suite, "__add_file adds file 3 times", test___add_file_adds_file_3_times) == NULL) return -1;
  if(CU_add_test(suite, "__remove_file removes file", test___remove_file_removes_file) == NULL) return -1;
  if(CU_add_test(suite, "__remove_file removes file for left", test___remove_file_removes_file_for_left) == NULL) return -1;
  if(CU_add_test(suite, "__remove_file removes file for center", test___remove_file_removes_file_for_center) == NULL) return -1;
  if(CU_add_test(suite, "__remove_file removes file for right", test___remove_file_removes_file_for_right) == NULL) return -1;
  if(CU_add_test(suite, "__fflush_unlocked sets offset for FFLUSH_FLAG_CAN_SEEK", test___fflush_unlocked_sets_offset_for_fflush_flag_can_seek) == NULL) return -1;
  if(CU_add_test(suite, "__fflush_unlocked doesn't set offset for 0", test___fflush_unlocked_does_not_set_offset_for_0) == NULL) return -1;
  if(CU_add_test(suite, "__fflush_unlocked doesn't set offset for FFLUSH_FLAG_CAN_WRITE", test___fflush_unlocked_does_not_set_offset_for_fflush_flag_can_write) == NULL) return -1;
  if(CU_add_test(suite, "__fflush_unlocked writes data for FFLUSH_FLAG_CAN_WRITE", test___fflush_unlocked_writes_data_for_fflush_flag_can_write) == NULL) return -1;
  if(CU_add_test(suite, "__fflush_unlocked doesn't write data for 0", test___fflush_unlocked_does_not_write_data_for_0) == NULL) return -1;
  if(CU_add_test(suite, "__fflush_unlocked doesn't write data for FFLUSH_FLAG_CAN_SEEK", test___fflush_unlocked_does_not_write_data_for_fflush_flag_can_seek) == NULL) return -1;
  if(CU_add_test(suite, "__fflush_unlocked sets offset for fully readed buffer", test___fflush_unlocked_sets_offset_for_fully_readed_buffer) == NULL) return -1;
  if(CU_add_test(suite, "__fflush_unlocked sets offset for empty buffer", test___fflush_unlocked_sets_offset_for_empty_buffer) == NULL) return -1;
  if(CU_add_test(suite, "__fflush_unlocked doesn't set offset for unbuffered", test___fflush_unlocked_does_not_set_offset_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "__fflush_unlocked writes data for empty buffer", test___fflush_unlocked_writes_data_for_empty_buffer) == NULL) return -1;
  if(CU_add_test(suite, "__fflush_unlocked does not write data for unbuffered", test___fflush_unlocked_does_not_write_data_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "__fflush_unlocked complains on bad fd for FFLUSH_FLAG_CAN_SEEK", test___fflush_unlocked_complains_on_bad_fd_for_fflush_flag_can_seek) == NULL) return -1;
  if(CU_add_test(suite, "__fflush_unlocked complains on bad fd for FFLUSH_FLAG_CAN_WRITE", test___fflush_unlocked_complains_on_bad_fd_for_fflush_flag_can_write) == NULL) return -1;
  if(CU_add_test(suite, "__fdclose_unlocked closes fd for file to read", test___fdclose_unlocked_closes_fd_for_file_to_read) == NULL) return -1;
  if(CU_add_test(suite, "__fdclose_unlocked closes fd for file to write", test___fdclose_unlocked_closes_fd_for_file_to_write) == NULL) return -1;
  if(CU_add_test(suite, "__fdclose_unlocked closes fd and waits for process for pipe", test___fdclose_unlocked_closes_fd_and_waits_for_process_for_pipe) == NULL) return -1;
  if(CU_add_test(suite, "__fdclose_unlocked complains on bad fd", test___fdclose_unlocked_complains_on_bad_fd) == NULL) return -1;
  if(CU_add_test(suite, "__init_file initializes file", test___init_file_initializes_file) == NULL) return -1;
  if(CU_add_test(suite, "__mode_to_oflags converts mode to open flags", test___mode_to_oflags_converts_mode_to_open_flags) == NULL) return -1;
  if(CU_add_test(suite, "__mode_to_oflags complains on invalid mode", test___mode_to_oflags_complains_on_invalid_mode) == NULL) return -1;
  if(CU_add_test(suite, "clearerr clears 0", test_clearerr_clears_0) == NULL) return -1;
  if(CU_add_test(suite, "clearerr clears EOF flag", test_clearerr_clears_eof_flag) == NULL) return -1;
  if(CU_add_test(suite, "clearerr clears error flag", test_clearerr_clears_error_flag) == NULL) return -1;
  if(CU_add_test(suite, "clearerr clears EOF flag and error flag", test_clearerr_clears_eof_flag_and_error_flag) == NULL) return -1;
  if(CU_add_test(suite, "fclose closes file to read", test_fclose_closes_file_to_read) == NULL) return -1;
  if(CU_add_test(suite, "fclose closes file to write", test_fclose_closes_file_to_write) == NULL) return -1;
  if(CU_add_test(suite, "fclose closes flushes", test_fclose_flushes) == NULL) return -1;
  if(CU_add_test(suite, "fclose complains on bad fd", test_fclose_complains_on_bad_fd) == NULL) return -1;
  if(CU_add_test(suite, "fdopen opens file with fd to read", test_fdopen_opens_file_with_fd_to_read) == NULL) return -1;
  if(CU_add_test(suite, "fdopen opens file with fd to read", test_fdopen_opens_file_with_fd_to_write) == NULL) return -1;
  if(CU_add_test(suite, "feof returns 0", test_feof_returns_0) == NULL) return -1;
  if(CU_add_test(suite, "feof returns 1", test_feof_returns_1) == NULL) return -1;
  if(CU_add_test(suite, "feof returns 0 for error flag", test_feof_returns_0_for_error_flag) == NULL) return -1;
  if(CU_add_test(suite, "ferror returns 0", test_ferror_returns_0) == NULL) return -1;
  if(CU_add_test(suite, "ferror returns 1", test_ferror_returns_1) == NULL) return -1;
  if(CU_add_test(suite, "ferror returns 0 for EOF flag", test_ferror_returns_0_for_eof_flag) == NULL) return -1;
  if(CU_add_test(suite, "fflush sets offset", test_fflush_sets_offset) == NULL) return -1;
  if(CU_add_test(suite, "fflush writes data", test_fflush_writes_data) == NULL) return -1;
  if(CU_add_test(suite, "fflush flushes all files", test_fflush_flushes_all_files) == NULL) return -1;
  if(CU_add_test(suite, "fileno returns fd", test_fileno_returns_fd) == NULL) return -1;
  if(CU_add_test(suite, "fflush complains on bad fd", test_fflush_complains_on_bad_fd) == NULL) return -1;
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
  if(CU_add_test(suite, "fread reads block after putc_unlocked buffer size + 1 times", test_fread_reads_block_after_putc_unlocked_buffer_size_plus_1_times) == NULL) return -1;
  if(CU_add_test(suite, "fread reads blocks after storing to pushed character", test_fread_reads_blocks_after_storing_to_pushed_character) == NULL) return -1;
  if(CU_add_test(suite, "fread returns 0 because EOF flag", test_fread_returns_0_because_eof_flag) == NULL) return -1;
  if(CU_add_test(suite, "fread returns 0 because error flag", test_fread_returns_0_because_error_flag) == NULL) return -1;
  if(CU_add_test(suite, "fread complains on bad fd", test_fread_complains_on_bad_fd) == NULL) return -1;
  if(CU_add_test(suite, "fread complains on bad fd for line buffered", test_fread_complains_on_bad_fd_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "fread complains on bad fd for unbuffered", test_fread_complains_on_bad_fd_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "freopen closes old file and opens new file to read", test_freopen_closes_old_file_and_opens_new_file_to_read) == NULL) return -1;
  if(CU_add_test(suite, "freopen closes old file and opens new file to write", test_freopen_closes_old_file_and_opens_new_file_to_write) == NULL) return -1;
  if(CU_add_test(suite, "freopen flushes", test_freopen_flushes) == NULL) return -1;
  if(CU_add_test(suite, "freopen complains on not found", test_freopen_complains_on_not_found) == NULL) return -1;
  if(CU_add_test(suite, "freopen opens file after not found", test_freopen_opens_file_after_not_found) == NULL) return -1;
  if(CU_add_test(suite, "fseeko sets offset for SEEK_SET", test_fseeko_sets_offset_for_seek_set) == NULL) return -1;
  if(CU_add_test(suite, "fseeko sets offset for SEEK_CUR and positive", test_fseeko_sets_offset_for_seek_cur_and_positive) == NULL) return -1;
  if(CU_add_test(suite, "fseeko sets offset for SEEK_CUR and negative", test_fseeko_sets_offset_for_seek_cur_and_negative) == NULL) return -1;
  if(CU_add_test(suite, "fseeko sets offset for SEEK_END", test_fseeko_sets_offset_for_seek_end) == NULL) return -1;
  if(CU_add_test(suite, "fseeko sets offset after getc_unlocked", test_fseeko_sets_offset_after_getc_unlocked) == NULL) return -1;
  if(CU_add_test(suite, "fseeko sets offset after getc_unlocked buffer size + 1 times", test_fseeko_sets_offset_after_getc_unlocked_buffer_size_plus_1_times) == NULL) return -1;
  if(CU_add_test(suite, "fseeko sets offset after putc_unlocked", test_fseeko_sets_offset_after_putc_unlocked) == NULL) return -1;
  if(CU_add_test(suite, "fseeko sets offset after putc_unlocked buffer size + 1 times", test_fseeko_sets_offset_after_putc_unlocked_buffer_size_plus_1_times) == NULL) return -1;
  if(CU_add_test(suite, "fseeko complains on bad fd", test_fseeko_complains_on_bad_fd) == NULL) return -1;
  if(CU_add_test(suite, "ftello returns offset", test_ftello_returns_offset) == NULL) return -1;
  if(CU_add_test(suite, "ftello returns offset after getc_unlocked", test_ftello_returns_offset_after_getc_unlocked) == NULL) return -1;
  if(CU_add_test(suite, "ftello returns offset after getc_unlocked buffer size + 1 times", test_ftello_returns_offset_after_getc_unlocked_buffer_size_plus_1_times) == NULL) return -1;
  if(CU_add_test(suite, "ftello returns offset after putc_unlocked", test_ftello_returns_offset_after_putc_unlocked) == NULL) return -1;
  if(CU_add_test(suite, "ftello returns offset after putc_unlocked buffer size + 1 times", test_ftello_returns_offset_after_putc_unlocked_buffer_size_plus_1_times) == NULL) return -1;
  if(CU_add_test(suite, "ftello complains on bad fd", test_ftello_complains_on_bad_fd) == NULL) return -1;
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
  if(CU_add_test(suite, "getc_unlocked reads character after storing to pushed character", test_getc_unlocked_reads_character_after_storing_to_pushed_character) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked returns EOF because EOF flag", test_getc_unlocked_returns_eof_because_eof_flag) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked complains on error flag", test_getc_unlocked_complains_on_error_flag) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked complains on bad fd", test_getc_unlocked_complains_on_bad_fd) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked complains on bad fd for line buffered", test_getc_unlocked_complains_on_bad_fd_for_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "getc_unlocked complains on bad fd for unbuffered", test_getc_unlocked_complains_on_bad_fd_for_unbuffered) == NULL) return -1;
  if(CU_add_test(suite, "popen opens pipe and fread reads and fclose closes", test_popen_opens_pipe_and_fread_reads_and_fclose_closes) == NULL) return -1;
  if(CU_add_test(suite, "popen opens pipe and fwrite writes and fclose closes", test_popen_opens_pipe_and_fwrite_writes_and_fclose_closes) == NULL) return -1;
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
  if(CU_add_test(suite, "setvbuf sets static buffer", test_setvbuf_sets_static_buffer) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf sets static buffer with line buffered", test_setvbuf_sets_static_buffer_with_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf sets allocated buffer", test_setvbuf_sets_allocated_buffer) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf sets allocated buffer with line buffered", test_setvbuf_sets_allocated_buffer_with_line_buffered) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf unsets buffer", test_setvbuf_unsets_buffer) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf sets static buffer after setting static buffer", test_setvbuf_sets_static_buffer_after_setting_static_buffer) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf sets static buffer after unsetting buffer", test_setvbuf_sets_static_buffer_after_unsetting_buffer) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf sets static buffer after setting static buffer", test_setvbuf_sets_allocated_buffer_after_setting_static_buffer) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf sets static buffer after unsetting buffer", test_setvbuf_sets_allocated_buffer_after_unsetting_buffer) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf unsets buffer after setting static buffer", test_setvbuf_unsets_buffer_after_setting_static_buffer) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf unsets buffer after unsetting buffer", test_setvbuf_unsets_buffer_after_unsetting_buffer) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf sets offset for setting", test_setvbuf_sets_offset_for_setting) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf sets offset for unsetting", test_setvbuf_sets_offset_for_unsetting) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf flushes for setting", test_setvbuf_flushes_for_setting) == NULL) return -1;
  if(CU_add_test(suite, "setvbuf flushes for unsetting", test_setvbuf_flushes_for_unsetting) == NULL) return -1;
  if(CU_add_test(suite, "ungetc pushes character back", test_ungetc_pushes_character_back) == NULL) return -1;
  if(CU_add_test(suite, "ungetc pushes character back 3 times", test_ungetc_pushes_character_back_3_times) == NULL) return -1;
  if(CU_add_test(suite, "ungetc stores character to pushed character", test_ungetc_stores_character_to_pushed_character) == NULL) return -1;
  if(CU_add_test(suite, "ungetc doesn't push character back for EOF", test_ungetc_does_not_push_character_back_for_eof) == NULL) return -1;
  if(CU_add_test(suite, "ungetc doesn't store character to pushed character after storing", test_ungetc_does_not_store_character_to_pushed_character_after_storing) == NULL) return -1;
  return 0;
}

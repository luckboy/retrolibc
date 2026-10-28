/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"
#include "test_file_helpers.h"

FILE *mock_stdin;
FILE *mock_stdout;
FILE *mock_stderr;

static void initialize_mock(void)
{
  mock_stdin = NULL;
  mock_stdout = NULL;
  mock_stderr = NULL;
}

static int initialize_stdio(void)
{ return 0; }

static int cleanup_stdio(void)
{
  clean_current_dir();
  return 0;
}

static void test_fgetc_reads_character(void)
{
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_fgetc(f), 'a');
  CU_ASSERT_EQUAL(retrolibc_fgetc(f), 'b');
  CU_ASSERT_EQUAL(retrolibc_fgetc(f), 'c');
  fclose(f);
}

static void test_fgetc_returns_eof(void)
{
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "a"));
  f = fopen("test.txt", "r");
  fgetc(f);
  CU_ASSERT_EQUAL(retrolibc_fgetc(f), EOF);
  fclose(f);
}

static void test_fgetpos_stores_position(void)
{
  FILE *f;
  retrolibc_fpos_t pos;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = fopen("test.txt", "r");
  fseek(f, 4, SEEK_SET);
  CU_ASSERT_EQUAL(retrolibc_fgetpos(f, &pos), 0);
  CU_ASSERT_EQUAL(pos.__offset, 4);
  fclose(f);
}

static void test_fgets_reads_string(void)
{
  static char buf[10];
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abc\ndef\n123456789\n0987654321\nghi"));
  f = fopen("test.txt", "r");
  CU_ASSERT_PTR_EQUAL(retrolibc_fgets(buf, 10, f), buf);
  CU_ASSERT_STRING_EQUAL(buf, "abc\n");
  CU_ASSERT_PTR_EQUAL(retrolibc_fgets(buf, 10, f), buf);
  CU_ASSERT_STRING_EQUAL(buf, "def\n");
  CU_ASSERT_PTR_EQUAL(retrolibc_fgets(buf, 10, f), buf);
  CU_ASSERT_STRING_EQUAL(buf, "123456789");
  fgetc(f);
  CU_ASSERT_PTR_EQUAL(retrolibc_fgets(buf, 10, f), buf);
  CU_ASSERT_STRING_EQUAL(buf, "098765432");
  fgetc(f);
  fgetc(f);
  CU_ASSERT_PTR_EQUAL(retrolibc_fgets(buf, 10, f), buf);
  CU_ASSERT_STRING_EQUAL(buf, "ghi");
  fclose(f);
}

static void test_fgets_returns_null(void)
{
  static char buf[10];
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "a"));
  f = fopen("test.txt", "r");
  fgetc(f);
  CU_ASSERT_PTR_NULL(retrolibc_fgets(buf, 10, f));
  fclose(f);
}

static void test_fputc_writes_character(void)
{
  static char s[100];
  FILE *f;
  clean_current_dir();
  initialize_mock();
  f = fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_fputc('a', f), 'a');
  CU_ASSERT_EQUAL(retrolibc_fputc('b', f), 'b');
  CU_ASSERT_EQUAL(retrolibc_fputc('c', f), 'c');
  fclose(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, 100));
  CU_ASSERT_STRING_EQUAL(s, "abc");
}

static void test_fputs_writes_string(void)
{
  static char s[100];
  FILE *f;
  clean_current_dir();
  initialize_mock();
  f = fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_fputs("abc", f), 0);
  CU_ASSERT_EQUAL(retrolibc_fputs("def", f), 0);
  CU_ASSERT_EQUAL(retrolibc_fputs("ghi", f), 0);
  fclose(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, 100));
  CU_ASSERT_STRING_EQUAL(s, "abcdefghi");
}

static void test_fputs_writes_empty_string(void)
{
  static char s[100];
  FILE *f;
  clean_current_dir();
  initialize_mock();
  f = fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_fputs("", f), 0);
  fclose(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, 100));
  CU_ASSERT_STRING_EQUAL(s, "");
}

static void test_fseek_sets_offset_for_seek_set(void)
{
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = fopen("test.txt", "r");
  fseeko(f, 4, SEEK_SET); 
  CU_ASSERT_EQUAL(retrolibc_fseek(f, 3, SEEK_SET), 0);
  CU_ASSERT_EQUAL(ftello(f), 3);
  fclose(f);
}

static void test_fseek_sets_offset_for_seek_cur_and_positive(void)
{
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = fopen("test.txt", "r");
  fseeko(f, 4, SEEK_SET); 
  CU_ASSERT_EQUAL(retrolibc_fseek(f, 3, SEEK_CUR), 0);
  CU_ASSERT_EQUAL(ftello(f), 7);
  fclose(f);
}

static void test_fseek_sets_offset_for_seek_cur_and_negative(void)
{
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = fopen("test.txt", "r");
  fseeko(f, 4, SEEK_SET);
  CU_ASSERT_EQUAL(retrolibc_fseek(f, -2, SEEK_CUR), 0);
  CU_ASSERT_EQUAL(ftello(f), 2);
  fclose(f);
}

static void test_fseek_sets_offset_for_seek_end(void)
{
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = fopen("test.txt", "r");
  fseeko(f, 4, SEEK_SET); 
  CU_ASSERT_EQUAL(retrolibc_fseek(f, -3, SEEK_END), 0);
  CU_ASSERT_EQUAL(ftello(f), 6);
  fclose(f);
}

static void test_fsetpos_sets_position(void)
{
  FILE *f;
  retrolibc_fpos_t pos;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = fopen("test.txt", "r");
  fseeko(f, 4, SEEK_SET); 
  pos.__offset = 3;
  CU_ASSERT_EQUAL(retrolibc_fsetpos(f, &pos), 0);
  CU_ASSERT_EQUAL(ftello(f), 3);
  fclose(f);
}

static void test_ftell_returns_offset(void)
{
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = fopen("test.txt", "r");
  fseek(f, 4, SEEK_SET);
  CU_ASSERT_EQUAL(retrolibc_ftell(f), 4);
  fclose(f);
}

static void test_getc_reads_character(void)
{
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  f = fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_getc(f), 'a');
  CU_ASSERT_EQUAL(retrolibc_getc(f), 'b');
  CU_ASSERT_EQUAL(retrolibc_getc(f), 'c');
  fclose(f);
}

static void test_getc_returns_eof(void)
{
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "a"));
  f = fopen("test.txt", "r");
  fgetc(f);
  CU_ASSERT_EQUAL(retrolibc_getc(f), EOF);
  fclose(f);
}

static void test_getchar_reads_character(void)
{
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  mock_stdin = fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_getchar(), 'a');
  CU_ASSERT_EQUAL(retrolibc_getchar(), 'b');
  CU_ASSERT_EQUAL(retrolibc_getchar(), 'c');
  fclose(mock_stdin);
}

static void test_getchar_returns_eof(void)
{
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "a"));
  mock_stdin = fopen("test.txt", "r");
  fgetc(mock_stdin);
  CU_ASSERT_EQUAL(retrolibc_getchar(), EOF);
  fclose(mock_stdin);
}

static void test_getchar_unlocked_reads_character(void)
{
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdef"));
  mock_stdin = fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_getchar_unlocked(), 'a');
  CU_ASSERT_EQUAL(retrolibc_getchar_unlocked(), 'b');
  CU_ASSERT_EQUAL(retrolibc_getchar_unlocked(), 'c');
  fclose(mock_stdin);
}

static void test_getchar_unlocked_returns_eof(void)
{
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "a"));
  mock_stdin = fopen("test.txt", "r");
  fgetc(mock_stdin);
  CU_ASSERT_EQUAL(retrolibc_getchar_unlocked(), EOF);
  fclose(mock_stdin);
}

static void test_gets_reads_string(void)
{
  static char buf[100];
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abc\ndef\nghi"));
  mock_stdin = fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_gets(buf), buf);
  CU_ASSERT_STRING_EQUAL(buf, "abc");
  CU_ASSERT_EQUAL(retrolibc_gets(buf), buf);
  CU_ASSERT_STRING_EQUAL(buf, "def");
  CU_ASSERT_EQUAL(retrolibc_gets(buf), buf);
  CU_ASSERT_STRING_EQUAL(buf, "ghi");
  fclose(mock_stdin);
}

static void test_gets_returns_null(void)
{
  static char buf[100];
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "a"));
  mock_stdin = fopen("test.txt", "r");
  fgetc(mock_stdin);
  CU_ASSERT_PTR_NULL(retrolibc_gets(buf));
  fclose(mock_stdin);
}

static void test_getw_reads_word(void)
{
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_word_to_file("test.txt", 0x11223344));
  f = fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_getw(f), 0x11223344);
  fclose(f);
}

static void test_getw_returns_eof(void)
{
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "a"));
  f = fopen("test.txt", "r");
  fgetc(f);
  CU_ASSERT_EQUAL(retrolibc_getw(f), EOF);
  fclose(f);
}

static void test_perror_prints_error_with_string(void)
{
  static char s[100];
  char *msg = "some error";
  clean_current_dir();
  initialize_mock();
  mock_stderr = fopen("test.txt", "w");
  errno = ENOENT;
  retrolibc_perror("some error");
  fclose(mock_stderr);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, 100));
  CU_ASSERT_EQUAL(memcmp(s, msg, strlen(msg)), 0);
  CU_ASSERT_EQUAL(memcmp(s + strlen(msg), ": ", 2), 0);
  CU_ASSERT_EQUAL(memcmp(s + strlen(msg) + 2, strerror(ENOENT), strlen(strerror(ENOENT))), 0);
  CU_ASSERT_EQUAL(s[strlen(msg) + 2 + strlen(strerror(ENOENT))], '\n');
  CU_ASSERT_EQUAL(s[strlen(msg) + 2 + strlen(strerror(ENOENT)) + 1], '\0');
}

static void test_perror_prints_error_with_null(void)
{
  static char s[100];
  clean_current_dir();
  initialize_mock();
  mock_stderr = fopen("test.txt", "w");
  errno = ENOENT;
  retrolibc_perror(NULL);
  fclose(mock_stderr);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, 100));
  CU_ASSERT_EQUAL(memcmp(s, strerror(ENOENT), strlen(strerror(ENOENT))), 0);
  CU_ASSERT_EQUAL(s[strlen(strerror(ENOENT))], '\n');
  CU_ASSERT_EQUAL(s[strlen(strerror(ENOENT)) + 1], '\0');
}

static void test_perror_prints_error_with_empty_string(void)
{
  static char s[100];
  clean_current_dir();
  initialize_mock();
  mock_stderr = fopen("test.txt", "w");
  errno = ENOENT;
  retrolibc_perror("");
  fclose(mock_stderr);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, 100));
  CU_ASSERT_EQUAL(memcmp(s, strerror(ENOENT), strlen(strerror(ENOENT))), 0);
  CU_ASSERT_EQUAL(s[strlen(strerror(ENOENT))], '\n');
  CU_ASSERT_EQUAL(s[strlen(strerror(ENOENT)) + 1], '\0');
}

static void test_putc_writes_character(void)
{
  FILE *f;
  static char s[100];
  clean_current_dir();
  initialize_mock();
  f = fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_putc('a', f), 'a');
  CU_ASSERT_EQUAL(retrolibc_putc('b', f), 'b');
  CU_ASSERT_EQUAL(retrolibc_putc('c', f), 'c');
  fclose(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, 100));
  CU_ASSERT_STRING_EQUAL(s, "abc");
}

static void test_putchar_writes_character(void)
{
  static char s[100];
  clean_current_dir();
  initialize_mock();
  mock_stdout = fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_putchar('a'), 'a');
  CU_ASSERT_EQUAL(retrolibc_putchar('b'), 'b');
  CU_ASSERT_EQUAL(retrolibc_putchar('c'), 'c');
  fclose(mock_stdout);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, 100));
  CU_ASSERT_STRING_EQUAL(s, "abc");
}

static void test_putchar_unlocked_writes_character(void)
{
  static char s[100];
  clean_current_dir();
  initialize_mock();
  mock_stdout = fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_putchar_unlocked('a'), 'a');
  CU_ASSERT_EQUAL(retrolibc_putchar_unlocked('b'), 'b');
  CU_ASSERT_EQUAL(retrolibc_putchar_unlocked('c'), 'c');
  fclose(mock_stdout);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, 100));
  CU_ASSERT_STRING_EQUAL(s, "abc");
}

static void test_puts_writes_string(void)
{
  static char s[100];
  clean_current_dir();
  initialize_mock();
  mock_stdout = fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_puts("abc"), 0);
  CU_ASSERT_EQUAL(retrolibc_puts("def"), 0);
  CU_ASSERT_EQUAL(retrolibc_puts("ghi"), 0);
  fclose(mock_stdout);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, 100));
  CU_ASSERT_STRING_EQUAL(s, "abc\ndef\nghi\n");
}

static void test_putw_writes_word(void)
{
  FILE *f;
  int w;
  clean_current_dir();
  initialize_mock();
  f = fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_putw(0x11223344, f), 0);
  fclose(f);
  CU_ASSERT_FATAL(read_word_from_file("test.txt", &w));
  CU_ASSERT_EQUAL(w, 0x11223344);
}

static void test_rewind_resets_offset(void)
{
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = fopen("test.txt", "r");
  fseeko(f, 4, SEEK_SET);
  retrolibc_rewind(f);
  CU_ASSERT_EQUAL(ftello(f), 0);
  fclose(f);
}

static void test_setbuf_sets_buffer(void)
{
  static char buf[BUFSIZ];
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = fopen("test.txt", "r");
  retrolibc_setbuf(f, buf);
  fclose(f);
}
static void test_setbuf_unsets_buffer(void)
{
  FILE *f;
  clean_current_dir();
  initialize_mock();
  CU_ASSERT_FATAL(write_to_file("test.txt", "abcdefghi"));
  f = fopen("test.txt", "r");
  retrolibc_setbuf(f, NULL);
  fclose(f);
}


int add_stdio_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("stdio suite", initialize_stdio, cleanup_stdio)) == NULL) return -1;
  if(CU_add_test(suite, "fgetc reads character", test_fgetc_reads_character) == NULL) return -1;
  if(CU_add_test(suite, "fgetc returns EOF", test_fgetc_returns_eof) == NULL) return -1;
  if(CU_add_test(suite, "fgetpos stores position", test_fgetpos_stores_position) == NULL) return -1;
  if(CU_add_test(suite, "fgets reads string", test_fgets_reads_string) == NULL) return -1;
  if(CU_add_test(suite, "fgets returns null", test_fgets_returns_null) == NULL) return -1;
  if(CU_add_test(suite, "fputc writes character", test_fputc_writes_character) == NULL) return -1;
  if(CU_add_test(suite, "fputs writes string", test_fputs_writes_string) == NULL) return -1;
  if(CU_add_test(suite, "fputs writes empty string", test_fputs_writes_empty_string) == NULL) return -1;
  if(CU_add_test(suite, "fseek sets offset for SEEK_SET", test_fseek_sets_offset_for_seek_set) == NULL) return -1;
  if(CU_add_test(suite, "fseek sets offset for SEEK_CUR and positive", test_fseek_sets_offset_for_seek_cur_and_positive) == NULL) return -1;
  if(CU_add_test(suite, "fseek sets offset for SEEK_CUR and negative", test_fseek_sets_offset_for_seek_cur_and_negative) == NULL) return -1;
  if(CU_add_test(suite, "fseek sets offset for SEEK_END", test_fseek_sets_offset_for_seek_end) == NULL) return -1;
  if(CU_add_test(suite, "fsetpos sets position", test_fsetpos_sets_position) == NULL) return -1;
  if(CU_add_test(suite, "ftell returns offset", test_ftell_returns_offset) == NULL) return -1;
  if(CU_add_test(suite, "getc reads character", test_getc_reads_character) == NULL) return -1;
  if(CU_add_test(suite, "getc returns EOF", test_getc_returns_eof) == NULL) return -1;
  if(CU_add_test(suite, "getchar reads character", test_getchar_reads_character) == NULL) return -1;
  if(CU_add_test(suite, "getchar returns EOF", test_getchar_returns_eof) == NULL) return -1;
  if(CU_add_test(suite, "getchar_unlocked reads character", test_getchar_unlocked_reads_character) == NULL) return -1;
  if(CU_add_test(suite, "getchar_unlocked returns EOF", test_getchar_unlocked_returns_eof) == NULL) return -1;
  if(CU_add_test(suite, "gets reads string", test_gets_reads_string) == NULL) return -1;
  if(CU_add_test(suite, "gets returns null", test_gets_returns_null) == NULL) return -1;
  if(CU_add_test(suite, "getw reads word", test_getw_reads_word) == NULL) return -1;
  if(CU_add_test(suite, "getw returns EOF", test_getw_returns_eof) == NULL) return -1;
  if(CU_add_test(suite, "perror prints error with string", test_perror_prints_error_with_string) == NULL) return -1;
  if(CU_add_test(suite, "perror prints error with null", test_perror_prints_error_with_null) == NULL) return -1;
  if(CU_add_test(suite, "perror prints error with empty string", test_perror_prints_error_with_empty_string) == NULL) return -1;
  if(CU_add_test(suite, "putc writes character", test_putc_writes_character) == NULL) return -1;
  if(CU_add_test(suite, "putchar writes character", test_putchar_writes_character) == NULL) return -1;
  if(CU_add_test(suite, "putchar_unlocked writes character", test_putchar_unlocked_writes_character) == NULL) return -1;
  if(CU_add_test(suite, "puts writes string", test_puts_writes_string) == NULL) return -1;
  if(CU_add_test(suite, "putw writes word", test_putw_writes_word) == NULL) return -1;
  if(CU_add_test(suite, "rewind resets offset", test_rewind_resets_offset) == NULL) return -1;
  if(CU_add_test(suite, "setbuf sets buffer", test_setbuf_sets_buffer) == NULL) return -1;
  if(CU_add_test(suite, "setbuf unsets buffer", test_setbuf_unsets_buffer) == NULL) return -1;
  return 0;
}

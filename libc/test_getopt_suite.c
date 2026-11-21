/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <CUnit/Basic.h>
#include "retrolibc.h"

void initialize_retrolibc_optpos(void);
char *retrolibc_getopt_message(void);

static void initialize_retrolibc_getopt(void)
{
  initialize_retrolibc_optpos();
  retrolibc_optarg = NULL;
  retrolibc_optind = 1;
  retrolibc_opterr = 1;
  retrolibc_optopt = '?';
  retrolibc_getopt_message()[0] = '\0';
}

static int initialize_getopt(void)
{ return 0; }

static int cleanup_getopt(void)
{ return 0; }

static void test_getopt_parses_options(void)
{
  static char *argv[] = { "program", "-abc", NULL };
  int argc = 2;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'b');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
}

static void test_getopt_parses_separated_options(void)
{
  static char *argv[] = { "program", "-a", "-bc", "-d", NULL };
  int argc = 4;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'b');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 3);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'd');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 4);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 4);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
}

static void test_getopt_parses_option_with_required_argument(void)
{
  static char *argv[] = { "program", "-abcdef", NULL };
  int argc = 2;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:c"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:c"), 'b');
  CU_ASSERT_STRING_EQUAL(retrolibc_optarg, "cdef");
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:c"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
}

static void test_getopt_parses_option_with_space_and_required_argument(void)
{
  static char *argv[] = { "program", "-ac", "-b", "def", NULL };
  int argc = 4;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:c"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:c"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:c"), 'b');
  CU_ASSERT_STRING_EQUAL(retrolibc_optarg, "def");
  CU_ASSERT_EQUAL(retrolibc_optind, 4);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:c"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 4);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
}

static void test_getopt_parses_option_with_optional_argument(void)
{
  static char *argv[] = { "program", "-abcdef", NULL };
  int argc = 2;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab::c"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab::c"), 'b');
  CU_ASSERT_STRING_EQUAL(retrolibc_optarg, "cdef");
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab::c"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
}

static void test_getopt_parses_option_with_space_and_optional_argument(void)
{
  static char *argv[] = { "program", "-ac", "-b", "def", NULL };
  int argc = 4;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab::c"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab::c"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab::c"), 'b');
  CU_ASSERT_STRING_EQUAL(retrolibc_optarg, "def");
  CU_ASSERT_EQUAL(retrolibc_optind, 4);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab::c"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 4);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
}

static void test_getopt_parses_option_without_optional_argument(void)
{
  static char *argv[] = { "program", "-ac", "-b", NULL };
  int argc = 3;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab::c"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab::c"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab::c"), 'b');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 3);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab::c"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 3);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
}

static void test_getopt_parses_options_with_arguments(void)
{
  static char *argv[] = { "program", "-ac", "-b", "def", "-dghi", NULL };
  int argc = 5;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:cd:"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:cd:"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:cd:"), 'b');
  CU_ASSERT_STRING_EQUAL(retrolibc_optarg, "def");
  CU_ASSERT_EQUAL(retrolibc_optind, 4);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:cd:"), 'd');
  CU_ASSERT_STRING_EQUAL(retrolibc_optarg, "ghi");
  CU_ASSERT_EQUAL(retrolibc_optind, 5);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:cd:"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 5);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
}

static void test_getopt_parses_options_and_stops_by_minus(void)
{
  static char *argv[] = { "program", "-a", "-bc", "-", "-a", NULL };
  int argc = 5;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'b');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 3);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 3);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
}

static void test_getopt_parses_options_and_stops_by_minus_minus(void)
{
  static char *argv[] = { "program", "-a", "-bc", "--", "-a", NULL };
  int argc = 5;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'b');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 3);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 4);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
}

static void test_getopt_parses_options_and_stops_by_other_argument(void)
{
  static char *argv[] = { "program", "-a", "-bc", "abc", "-a", NULL };
  int argc = 5;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'b');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 3);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 3);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
}

static void test_getopt_resets_for_options(void)
{
  static char *argv[] = { "program", "-abc", NULL };
  int argc = 2;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'b');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_optind = 0;
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'b');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
}

static void test_getopt_resets_for_separated_options(void)
{
  static char *argv[] = { "program", "-a", "-bc", "-d", NULL };
  int argc = 4;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'b');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 3);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'd');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 4);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 4);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_optind = 0;
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'b');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 3);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), 'd');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 4);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abcd"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 4);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
}

static void test_getopt_complains_on_invalid_option(void)
{
  static char *argv[] = { "program", "-abdc", NULL };
  int argc = 2;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'b');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), '?');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, 'd');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "program: invalid option -- 'd'\n");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
}

static void test_getopt_complains_on_option_requires_an_argument(void)
{
  static char *argv[] = { "program", "-ab", NULL };
  int argc = 2;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:c"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:c"), '?');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, 'b');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "program: option requires an argument -- 'b'\n");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:c"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, 'b');
}

static void test_getopt_complains_on_invalid_option_for_first_colon(void)
{
  static char *argv[] = { "program", "-abdc", NULL };
  int argc = 2;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, ":abc"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, ":abc"), 'b');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, ":abc"), '?');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, 'd');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, ":abc"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, ":abc"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
}

static void test_getopt_complains_on_option_requires_an_argument_for_first_colon(void)
{
  static char *argv[] = { "program", "-ab", NULL };
  int argc = 2;
  initialize_retrolibc_getopt();
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, ":ab:c"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, ":ab:c"), ':');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, 'b');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, ":ab:c"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 1);
  CU_ASSERT_EQUAL(retrolibc_optopt, 'b');
}

static void test_getopt_complains_on_invalid_option_for_opterr_that_is_0(void)
{
  static char *argv[] = { "program", "-abdc", NULL };
  int argc = 2;
  initialize_retrolibc_getopt();
  retrolibc_opterr = 0;
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 0);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'b');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 0);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), '?');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 0);
  CU_ASSERT_EQUAL(retrolibc_optopt, 'd');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), 'c');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 0);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "abc"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 0);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
}

static void test_getopt_complains_on_option_requires_an_argument_for_opterr_that_is_0(void)
{
  static char *argv[] = { "program", "-ab", NULL };
  int argc = 2;
  initialize_retrolibc_getopt();
  retrolibc_opterr = 0;
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:c"), 'a');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 1);
  CU_ASSERT_EQUAL(retrolibc_opterr, 0);
  CU_ASSERT_EQUAL(retrolibc_optopt, '\0');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:c"), '?');
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 0);
  CU_ASSERT_EQUAL(retrolibc_optopt, 'b');
  CU_ASSERT_STRING_EQUAL(retrolibc_getopt_message(), "");
  retrolibc_getopt_message()[0] = '\0';
  CU_ASSERT_EQUAL(retrolibc_getopt(argc, argv, "ab:c"), -1);
  CU_ASSERT_PTR_NULL(retrolibc_optarg);
  CU_ASSERT_EQUAL(retrolibc_optind, 2);
  CU_ASSERT_EQUAL(retrolibc_opterr, 0);
  CU_ASSERT_EQUAL(retrolibc_optopt, 'b');
}

int add_getopt_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("getopt suite", initialize_getopt, cleanup_getopt)) == NULL) return -1;
  if(CU_add_test(suite, "getopt parses options", test_getopt_parses_options) == NULL) return -1;
  if(CU_add_test(suite, "getopt parses separated options", test_getopt_parses_separated_options) == NULL) return -1;
  if(CU_add_test(suite, "getopt parses option with required argument", test_getopt_parses_option_with_required_argument) == NULL) return -1;
  if(CU_add_test(suite, "getopt parses option with space and required argument", test_getopt_parses_option_with_space_and_required_argument) == NULL) return -1;
  if(CU_add_test(suite, "getopt parses option with optional argument", test_getopt_parses_option_with_optional_argument) == NULL) return -1;
  if(CU_add_test(suite, "getopt parses option with space and optional argument", test_getopt_parses_option_with_space_and_optional_argument) == NULL) return -1;
  if(CU_add_test(suite, "getopt parses option without optional argument", test_getopt_parses_option_without_optional_argument) == NULL) return -1;
  if(CU_add_test(suite, "getopt parses options with arguments", test_getopt_parses_options_with_arguments) == NULL) return -1;
  if(CU_add_test(suite, "getopt parses options and stops by -", test_getopt_parses_options_and_stops_by_minus) == NULL) return -1;
  if(CU_add_test(suite, "getopt parses options and stops by --", test_getopt_parses_options_and_stops_by_minus_minus) == NULL) return -1;
  if(CU_add_test(suite, "getopt parses options and stops by other argument", test_getopt_parses_options_and_stops_by_other_argument) == NULL) return -1;
  if(CU_add_test(suite, "getopt resets for options", test_getopt_resets_for_options) == NULL) return -1;
  if(CU_add_test(suite, "getopt resets for separated options", test_getopt_resets_for_separated_options) == NULL) return -1;
  if(CU_add_test(suite, "getopt complains on invalid option", test_getopt_complains_on_invalid_option) == NULL) return -1;
  if(CU_add_test(suite, "getopt complains on option requires an argument", test_getopt_complains_on_option_requires_an_argument) == NULL) return -1;
  if(CU_add_test(suite, "getopt complains on invalid option for first :", test_getopt_complains_on_invalid_option_for_first_colon) == NULL) return -1;
  if(CU_add_test(suite, "getopt complains on option requires an argument for first :", test_getopt_complains_on_option_requires_an_argument_for_first_colon) == NULL) return -1;
  if(CU_add_test(suite, "getopt complains on invalid option for opterr that is 0", test_getopt_complains_on_invalid_option_for_opterr_that_is_0) == NULL) return -1;
  if(CU_add_test(suite, "getopt complains on option requires an argument for opterr that is 0", test_getopt_complains_on_option_requires_an_argument_for_opterr_that_is_0) == NULL) return -1;
  return 0;
}

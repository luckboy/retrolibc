/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <limits.h>
#include <math.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"
#include "test_file_helpers.h"

#ifndef LONG_BIT
#define LONG_BIT        __WORDSIZE
#endif

static int initialize_fprintf(void)
{ return 0; }

static int cleanup_fprintf(void)
{
  clean_current_dir();
  return 0;
}

static void test_sprintf_formats_values(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "n=%d s=%s f=%f\n", 1234, "abc", 34.56), 25);
  CU_ASSERT_STRING_EQUAL(buf, "n=1234 s=abc f=34.560000\n");
}

static void test_sprintf_formats_number_for_d(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%d", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%d", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%d", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_d_with_apostrophe(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'d", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'d", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'d", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_d_with_plus(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+d", 1234), 5);
  CU_ASSERT_STRING_EQUAL(buf, "+1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+d", 123499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "+123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+d", 0), 2);
  CU_ASSERT_STRING_EQUAL(buf, "+0");
}

static void test_sprintf_formats_number_for_d_with_space(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% d", 1234), 5);
  CU_ASSERT_STRING_EQUAL(buf, " 1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% d", 123499), 7);
  CU_ASSERT_STRING_EQUAL(buf, " 123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% d", 0), 2);
  CU_ASSERT_STRING_EQUAL(buf, " 0");
}

static void test_sprintf_formats_number_for_d_with_hash(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#d", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#d", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#d", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_d_with_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10d", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "      1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10d", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     -3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10d", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10d", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "         0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3d", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3d", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3d", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "  0");
}

static void test_sprintf_formats_number_for_d_with_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10d", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10d", -3456), 11);
  CU_ASSERT_STRING_EQUAL(buf, "-0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10d", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10d", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3d", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3d", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3d", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.0d", 0), 0);
  CU_ASSERT_STRING_EQUAL(buf, "");
}

static void test_sprintf_formats_number_for_d_with_hash_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10d", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10d", -3456), 11);
  CU_ASSERT_STRING_EQUAL(buf, "-0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10d", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10d", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3d", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3d", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3d", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.0d", 0), 0);
  CU_ASSERT_STRING_EQUAL(buf, "");
}

static void test_sprintf_formats_number_for_d_with_width_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5d", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     01234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5d", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    -03456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5d", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5d", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10d", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10d", -3456), 11);
  CU_ASSERT_STRING_EQUAL(buf, "-0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10d", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10d", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
}

static void test_sprintf_formats_number_for_d_with_0_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010d", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010d", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010d", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010d", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03d", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03d", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03d", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_d_with_0_and_plus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010d", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010d", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010d", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010d", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03d", 1234), 5);
  CU_ASSERT_STRING_EQUAL(buf, "+1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03d", 123499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "+123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03d", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "+00"); 
}

static void test_sprintf_formats_number_for_d_with_0_and_space_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010d", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010d", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010d", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010d", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03d", 1234), 5);
  CU_ASSERT_STRING_EQUAL(buf, " 1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03d", 123499), 7);
  CU_ASSERT_STRING_EQUAL(buf, " 123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03d", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, " 00"); 
}

static void test_sprintf_formats_number_for_d_with_0_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010d", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010d", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010d", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010d", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03d", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03d", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03d", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_d_with_0_and_plus_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010d", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010d", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010d", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010d", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03d", 1234), 5);
  CU_ASSERT_STRING_EQUAL(buf, "+1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03d", 123499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "+123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03d", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "+00"); 
}

static void test_sprintf_formats_number_for_d_with_0_and_space_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010d", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010d", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010d", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010d", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03d", 1234), 5);
  CU_ASSERT_STRING_EQUAL(buf, " 1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03d", 123499), 7);
  CU_ASSERT_STRING_EQUAL(buf, " 123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03d", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, " 00"); 
}

static void test_sprintf_formats_number_for_d_with_0_and_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10d", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10d", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10d", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10d", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3d", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3d", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3d", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_d_with_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10d", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "1234      ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10d", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3456     ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10d", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "123499    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10d", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0         ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3d", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3d", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3d", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3d", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0  ");
}

static void test_sprintf_formats_number_for_i(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%i", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%i", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%i", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_i_with_apostrophe(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'i", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'i", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'i", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_i_with_plus(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+i", 1234), 5);
  CU_ASSERT_STRING_EQUAL(buf, "+1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+i", 123499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "+123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+i", 0), 2);
  CU_ASSERT_STRING_EQUAL(buf, "+0");
}

static void test_sprintf_formats_number_for_i_with_space(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% i", 1234), 5);
  CU_ASSERT_STRING_EQUAL(buf, " 1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% i", 123499), 7);
  CU_ASSERT_STRING_EQUAL(buf, " 123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% i", 0), 2);
  CU_ASSERT_STRING_EQUAL(buf, " 0");
}

static void test_sprintf_formats_number_for_i_with_hash(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#i", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#i", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#i", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_i_with_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10i", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "      1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10i", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     -3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10i", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10i", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "         0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3i", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3i", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3i", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "  0");
}

static void test_sprintf_formats_number_for_i_with_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10i", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10i", -3456), 11);
  CU_ASSERT_STRING_EQUAL(buf, "-0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10i", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10i", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3i", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3i", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3i", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.0i", 0), 0);
  CU_ASSERT_STRING_EQUAL(buf, "");
}

static void test_sprintf_formats_number_for_i_with_hash_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10i", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10i", -3456), 11);
  CU_ASSERT_STRING_EQUAL(buf, "-0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10i", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10i", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3i", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3i", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3i", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.0i", 0), 0);
  CU_ASSERT_STRING_EQUAL(buf, "");
}

static void test_sprintf_formats_number_for_i_with_width_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5i", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     01234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5i", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    -03456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5i", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5i", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10i", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10i", -3456), 11);
  CU_ASSERT_STRING_EQUAL(buf, "-0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10i", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10i", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
}

static void test_sprintf_formats_number_for_i_with_0_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010i", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010i", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010i", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010i", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03i", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03i", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03i", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_i_with_0_and_plus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010i", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010i", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010i", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010i", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03i", 1234), 5);
  CU_ASSERT_STRING_EQUAL(buf, "+1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03i", 123499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "+123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03i", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "+00"); 
}

static void test_sprintf_formats_number_for_i_with_0_and_space_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010i", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010i", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010i", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010i", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03i", 1234), 5);
  CU_ASSERT_STRING_EQUAL(buf, " 1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03i", 123499), 7);
  CU_ASSERT_STRING_EQUAL(buf, " 123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03i", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, " 00"); 
}

static void test_sprintf_formats_number_for_i_with_0_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010i", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010i", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010i", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010i", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03i", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03i", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03i", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_i_with_0_and_plus_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010i", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010i", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010i", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010i", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03i", 1234), 5);
  CU_ASSERT_STRING_EQUAL(buf, "+1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03i", 123499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "+123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03i", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "+00"); 
}

static void test_sprintf_formats_number_for_i_with_0_and_space_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010i", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010i", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010i", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010i", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03i", 1234), 5);
  CU_ASSERT_STRING_EQUAL(buf, " 1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03i", 123499), 7);
  CU_ASSERT_STRING_EQUAL(buf, " 123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03i", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, " 00"); 
}

static void test_sprintf_formats_number_for_i_with_0_and_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10i", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10i", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10i", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10i", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3i", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3i", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3i", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_i_with_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10i", 1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "1234      ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10i", -3456), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3456     ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10i", 123499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "123499    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10i", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0         ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3i", 1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3i", -3456), 5);
  CU_ASSERT_STRING_EQUAL(buf, "-3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3i", 123499), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3i", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0  ");
}

static void test_sprintf_formats_number_for_o(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%o", 01234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%o", 03456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%o", 0123477U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%o", 0U), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_o_with_apostrophe(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'o", 01234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'o", 03456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'o", 0123477U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'o", 0U), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_o_with_plus(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+o", 01234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+o", 03456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+o", 0123477U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+o", 0U), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_o_with_space(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% o", 01234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% o", 03456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% o", 0123477U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% o", 0U), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_o_with_hash(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#o", 01234U), 5);
  CU_ASSERT_STRING_EQUAL(buf, "01234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#o", 03456U), 5);
  CU_ASSERT_STRING_EQUAL(buf, "03456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#o", 0123477U), 7);
  CU_ASSERT_STRING_EQUAL(buf, "0123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#o", 0U), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_o_with_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10o", 01234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "      1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10o", 03456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "      3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10o", 0123477U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10o", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "         0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3o", 01234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3o", 03456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3o", 0123477U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3o", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "  0");
}

static void test_sprintf_formats_number_for_o_with_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10o", 01234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10o", 03456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10o", 0123477U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10o", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3o", 01234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3o", 03456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3o", 0123477U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3o", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.0o", 0U), 0);
  CU_ASSERT_STRING_EQUAL(buf, "");
}

static void test_sprintf_formats_number_for_o_with_hash_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10o", 01234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10o", 03456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10o", 0123477U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10o", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3o", 01234U), 5);
  CU_ASSERT_STRING_EQUAL(buf, "01234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3o", 03456U), 5);
  CU_ASSERT_STRING_EQUAL(buf, "03456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3o", 0123477U), 7);
  CU_ASSERT_STRING_EQUAL(buf, "0123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3o", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.0o", 0U), 0);
  CU_ASSERT_STRING_EQUAL(buf, "");
}

static void test_sprintf_formats_number_for_o_with_width_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5o", 01234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     01234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5o", 03456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     03456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5o", 0123477U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5o", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10o", 01234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10o", 03456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10o", 0123477U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10o", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
}

static void test_sprintf_formats_number_for_o_with_0_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010o", 01234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010o", 03456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010o", 0123477U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010o", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03o", 01234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03o", 03456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03o", 0123477U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03o", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_o_with_0_and_plus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010o", 01234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010o", 03456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010o", 0123477U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010o", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03o", 01234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03o", 03456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03o", 0123477U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03o", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_o_with_0_and_space_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010o", 01234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010o", 03456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010o", 0123477U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010o", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03o", 01234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03o", 03456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03o", 0123477U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03o", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_o_with_0_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010o", 01234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010o", 03456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010o", 0123477U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010o", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03o", 01234U), 5);
  CU_ASSERT_STRING_EQUAL(buf, "01234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03o", 03456U), 5);
  CU_ASSERT_STRING_EQUAL(buf, "03456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03o", 0123477U), 7);
  CU_ASSERT_STRING_EQUAL(buf, "0123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03o", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_o_with_0_and_plus_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010o", 01234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010o", 03456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010o", 0123477U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010o", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03o", 01234U), 5);
  CU_ASSERT_STRING_EQUAL(buf, "01234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03o", 03456U), 5);
  CU_ASSERT_STRING_EQUAL(buf, "03456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03o", 0123477U), 7);
  CU_ASSERT_STRING_EQUAL(buf, "0123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03o", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_o_with_0_and_space_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010o", 01234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010o", 03456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010o", 0123477U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010o", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03o", 01234U), 5);
  CU_ASSERT_STRING_EQUAL(buf, "01234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03o", 03456U), 5);
  CU_ASSERT_STRING_EQUAL(buf, "03456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03o", 0123477U), 7);
  CU_ASSERT_STRING_EQUAL(buf, "0123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03o", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_o_with_0_and_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10o", 01234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10o", 03456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10o", 0123477U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10o", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3o", 01234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3o", 03456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3o", 0123477U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3o", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_o_with_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10o", 01234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "1234      ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10o", 03456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "3456      ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10o", 0123477U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "123477    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10o", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0         ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3o", 01234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3o", 03456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3o", 0123477U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123477");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3o", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0  ");
}

static void test_sprintf_formats_number_for_u(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%u", 0U), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_u_with_apostrophe(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'u", 0U), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_u_with_plus(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+u", 0U), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_u_with_space(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% u", 0U), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_u_with_hash(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#u", 0U), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_u_with_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10u", 1234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "      1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10u", 3456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "      3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10u", 123499U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10u", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "         0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3u", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "  0");
}

static void test_sprintf_formats_number_for_u_with_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10u", 1234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10u", 3456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10u", 123499U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10u", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3u", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.0u", 0U), 0);
  CU_ASSERT_STRING_EQUAL(buf, "");
}

static void test_sprintf_formats_number_for_u_with_hash_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10u", 1234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10u", 3456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10u", 123499U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10u", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3u", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.0u", 0U), 0);
  CU_ASSERT_STRING_EQUAL(buf, "");
}

static void test_sprintf_formats_number_for_u_with_width_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5u", 1234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     01234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5u", 3456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     03456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5u", 123499U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5u", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10u", 1234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10u", 3456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10u", 123499U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10u", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
}

static void test_sprintf_formats_number_for_u_with_0_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010u", 1234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010u", 3456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010u", 123499U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010u", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03u", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_u_with_0_and_plus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010u", 1234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010u", 3456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010u", 123499U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010u", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03u", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_u_with_0_and_space_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010u", 1234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010u", 3456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010u", 123499U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010u", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03u", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_u_with_0_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010u", 1234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010u", 3456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010u", 123499U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010u", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03u", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_u_with_0_and_plus_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010u", 1234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010u", 3456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010u", 123499U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010u", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03u", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_u_with_0_and_space_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010u", 1234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010u", 3456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010u", 123499U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010u", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03u", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_u_with_0_and_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10u", 1234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10u", 3456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000003456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10u", 123499U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10u", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3u", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000"); 
}

static void test_sprintf_formats_number_for_u_with_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10u", 1234U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "1234      ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10u", 3456U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "3456      ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10u", 123499U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "123499    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10u", 0U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0         ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3u", 1234U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3u", 3456U), 4);
  CU_ASSERT_STRING_EQUAL(buf, "3456");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3u", 123499U), 6);
  CU_ASSERT_STRING_EQUAL(buf, "123499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3u", 0U), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0  ");
}

static void test_sprintf_formats_number_for_x(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%x", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%x", 0x12af), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%x", 0xfa1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%x", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_x_with_apostrophe(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'x", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'x", 0x12af), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'x", 0xfa1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'x", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_x_with_plus(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+x", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+x", 0x12af), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+x", 0xfa1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+x", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_x_with_space(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% x", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% x", 0x12af), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% x", 0xfa1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% x", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_x_with_hash(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#x", 0x1234), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0x1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#x", 0x12af), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0x12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#x", 0xfa1299), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0xfa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#x", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0x0");
}

static void test_sprintf_formats_number_for_x_with_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10x", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "      1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10x", 0x12af), 10);
  CU_ASSERT_STRING_EQUAL(buf, "      12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10x", 0xfa1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10x", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "         0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3x", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3x", 0x12af), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3x", 0xfa1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3x", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "  0");
}

static void test_sprintf_formats_number_for_x_with_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10x", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10x", 0x12af), 10);
  CU_ASSERT_STRING_EQUAL(buf, "00000012af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10x", 0xfa1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10x", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3x", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3x", 0x12af), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3x", 0xfa1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3x", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.0x", 0), 0);
  CU_ASSERT_STRING_EQUAL(buf, "");
}

static void test_sprintf_formats_number_for_x_with_hash_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10x", 0x1234), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0x0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10x", 0x12af), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0x00000012af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10x", 0xfa1299), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0x0000fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10x", 0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0x0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3x", 0x1234), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0x1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3x", 0x12af), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0x12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3x", 0xfa1299), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0xfa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3x", 0), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0x000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.0x", 0), 0);
  CU_ASSERT_STRING_EQUAL(buf, "");
}

static void test_sprintf_formats_number_for_x_with_width_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5x", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     01234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5x", 0x12af), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     012af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5x", 0xfa1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5x", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10x", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10x", 0x12af), 10);
  CU_ASSERT_STRING_EQUAL(buf, "00000012af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10x", 0xfa1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10x", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
}

static void test_sprintf_formats_number_for_x_with_0_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010x", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010x", 0x12af), 10);
  CU_ASSERT_STRING_EQUAL(buf, "00000012af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010x", 0xfa1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010x", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03x", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03x", 0x12af), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03x", 0xfa1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03x", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
}

static void test_sprintf_formats_number_for_x_with_0_and_plus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010x", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010x", 0x12af), 10);
  CU_ASSERT_STRING_EQUAL(buf, "00000012af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010x", 0xfa1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010x", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03x", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03x", 0x12af), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03x", 0xfa1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03x", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
}

static void test_sprintf_formats_number_for_x_with_0_and_space_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010x", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010x", 0x12af), 10);
  CU_ASSERT_STRING_EQUAL(buf, "00000012af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010x", 0xfa1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010x", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03x", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03x", 0x12af), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03x", 0xfa1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03x", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
}

static void test_sprintf_formats_number_for_x_with_0_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010x", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x00001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010x", 0x12af), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x000012af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010x", 0xfa1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x00fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010x", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x00000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03x", 0x1234), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0x1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03x", 0x12af), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0x12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03x", 0xfa1299), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0xfa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03x", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0x0");
}

static void test_sprintf_formats_number_for_x_with_0_and_plus_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010x", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x00001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010x", 0x12af), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x000012af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010x", 0xfa1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x00fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010x", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x00000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03x", 0x1234), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0x1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03x", 0x12af), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0x12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03x", 0xfa1299), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0xfa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03x", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0x0");
}

static void test_sprintf_formats_number_for_x_with_0_and_space_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010x", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x00001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010x", 0x12af), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x000012af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010x", 0xfa1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x00fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010x", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x00000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03x", 0x1234), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0x1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03x", 0x12af), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0x12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03x", 0xfa1299), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0xfa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03x", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0x0");
}

static void test_sprintf_formats_number_for_x_with_0_and_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10x", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10x", 0x12af), 10);
  CU_ASSERT_STRING_EQUAL(buf, "00000012af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10x", 0xfa1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10x", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3x", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3x", 0x12af), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3x", 0xfa1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3x", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
}

static void test_sprintf_formats_number_for_x_with_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10x", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "1234      ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10x", 0x12af), 10);
  CU_ASSERT_STRING_EQUAL(buf, "12af      ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10x", 0xfa1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "fa1299    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10x", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0         ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3x", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3x", 0x12af), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12af");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3x", 0xfa1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "fa1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3x", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0  ");
}

static void test_sprintf_formats_number_for_uppercase_x(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%X", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%X", 0x12AF), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%X", 0xFA1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%X", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_uppercase_x_with_apostrophe(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'X", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'X", 0x12AF), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'X", 0xFA1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'X", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_uppercase_x_with_plus(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+X", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+X", 0x12AF), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+X", 0xFA1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+X", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_uppercase_x_with_space(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% X", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% X", 0x12AF), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% X", 0xFA1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% X", 0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
}

static void test_sprintf_formats_number_for_uppercase_x_with_hash(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#X", 0x1234), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0X1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#X", 0x12AF), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0X12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#X", 0xFA1299), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0XFA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#X", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0X0");
}

static void test_sprintf_formats_number_for_uppercase_x_with_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10X", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "      1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10X", 0x12AF), 10);
  CU_ASSERT_STRING_EQUAL(buf, "      12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10X", 0xFA1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10X", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "         0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3X", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3X", 0x12AF), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3X", 0xFA1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%3X", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "  0");
}

static void test_sprintf_formats_number_for_uppercase_x_with_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10X", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10X", 0x12AF), 10);
  CU_ASSERT_STRING_EQUAL(buf, "00000012AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10X", 0xFA1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10X", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3X", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3X", 0x12AF), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3X", 0xFA1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.3X", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.0X", 0), 0);
  CU_ASSERT_STRING_EQUAL(buf, "");
}

static void test_sprintf_formats_number_for_uppercase_x_with_hash_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10X", 0x1234), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0X0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10X", 0x12AF), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0X00000012AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10X", 0xFA1299), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0X0000FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.10X", 0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0X0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3X", 0x1234), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0X1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3X", 0x12AF), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0X12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3X", 0xFA1299), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0XFA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.3X", 0), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0X000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.0X", 0), 0);
  CU_ASSERT_STRING_EQUAL(buf, "");
}

static void test_sprintf_formats_number_for_uppercase_x_with_width_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5X", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     01234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5X", 0x12AF), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     012AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5X", 0xFA1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.5X", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "     00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10X", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10X", 0x12AF), 10);
  CU_ASSERT_STRING_EQUAL(buf, "00000012AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10X", 0xFA1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5.10X", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
}

static void test_sprintf_formats_number_for_uppercase_x_with_0_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010X", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010X", 0x12AF), 10);
  CU_ASSERT_STRING_EQUAL(buf, "00000012AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010X", 0xFA1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%010X", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03X", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03X", 0x12AF), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03X", 0xFA1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%03X", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
}

static void test_sprintf_formats_number_for_uppercase_x_with_0_and_plus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010X", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010X", 0x12AF), 10);
  CU_ASSERT_STRING_EQUAL(buf, "00000012AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010X", 0xFA1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+010X", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03X", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03X", 0x12AF), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03X", 0xFA1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+03X", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
}

static void test_sprintf_formats_number_for_uppercase_x_with_0_and_space_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010X", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010X", 0x12AF), 10);
  CU_ASSERT_STRING_EQUAL(buf, "00000012AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010X", 0xFA1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 010X", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03X", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03X", 0x12AF), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03X", 0xFA1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 03X", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
}

static void test_sprintf_formats_number_for_uppercase_x_with_0_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010X", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0X00001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010X", 0x12AF), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0X000012AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010X", 0xFA1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0X00FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#010X", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0X00000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03X", 0x1234), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0X1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03X", 0x12AF), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0X12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03X", 0xFA1299), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0XFA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#03X", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0X0");
}

static void test_sprintf_formats_number_for_uppercase_x_with_0_and_plus_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010X", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0X00001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010X", 0x12AF), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0X000012AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010X", 0xFA1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0X00FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#010X", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0X00000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03X", 0x1234), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0X1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03X", 0x12AF), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0X12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03X", 0xFA1299), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0XFA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+#03X", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0X0");
}

static void test_sprintf_formats_number_for_uppercase_x_with_0_and_space_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010X", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0X00001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010X", 0x12AF), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0X000012AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010X", 0xFA1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0X00FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #010X", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0X00000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03X", 0x1234), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0X1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03X", 0x12AF), 6);
  CU_ASSERT_STRING_EQUAL(buf, "0X12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03X", 0xFA1299), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0XFA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% #03X", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0X0");
}

static void test_sprintf_formats_number_for_uppercase_x_with_0_and_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10X", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000001234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10X", 0x12AF), 10);
  CU_ASSERT_STRING_EQUAL(buf, "00000012AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10X", 0xFA1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-10X", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3X", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3X", 0x12AF), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3X", 0xFA1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-3X", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "000");
}

static void test_sprintf_formats_number_for_uppercase_x_with_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10X", 0x1234), 10);
  CU_ASSERT_STRING_EQUAL(buf, "1234      ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10X", 0x12AF), 10);
  CU_ASSERT_STRING_EQUAL(buf, "12AF      ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10X", 0xFA1299), 10);
  CU_ASSERT_STRING_EQUAL(buf, "FA1299    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10X", 0), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0         ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3X", 0x1234), 4);
  CU_ASSERT_STRING_EQUAL(buf, "1234");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3X", 0x12AF), 4);
  CU_ASSERT_STRING_EQUAL(buf, "12AF");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3X", 0xFA1299), 6);
  CU_ASSERT_STRING_EQUAL(buf, "FA1299");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-3X", 0), 3);
  CU_ASSERT_STRING_EQUAL(buf, "0  ");
}

static void test_sprintf_formats_number_for_i_with_length_modifier(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%hhi", SCHAR_MAX), 3);
  CU_ASSERT_STRING_EQUAL(buf, "127");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%hhi", SCHAR_MIN), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-128");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%hi", SHRT_MAX), 5);
  CU_ASSERT_STRING_EQUAL(buf, "32767");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%hi", SHRT_MIN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-32768");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%i", INT_MAX), 10);
  CU_ASSERT_STRING_EQUAL(buf, "2147483647");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%i", INT_MIN), 11);
  CU_ASSERT_STRING_EQUAL(buf, "-2147483648");
#if LONG_BIT == 64
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%li", LONG_MAX), 19);
  CU_ASSERT_STRING_EQUAL(buf, "9223372036854775807");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%li", LONG_MIN), 20);
  CU_ASSERT_STRING_EQUAL(buf, "-9223372036854775808");
#else
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%li", LONG_MAX), 10);
  CU_ASSERT_STRING_EQUAL(buf, "2147483647");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%li", LONG_MIN), 11);
  CU_ASSERT_STRING_EQUAL(buf, "-2147483648");
#endif
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%lli", LLONG_MAX), 19);
  CU_ASSERT_STRING_EQUAL(buf, "9223372036854775807");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%lli", LLONG_MIN), 20);
  CU_ASSERT_STRING_EQUAL(buf, "-9223372036854775808");
}

static void test_sprintf_formats_number_for_u_with_length_modifier(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%hhu", UCHAR_MAX), 3);
  CU_ASSERT_STRING_EQUAL(buf, "255");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%hu", USHRT_MAX), 5);
  CU_ASSERT_STRING_EQUAL(buf, "65535");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%u", UINT_MAX), 10);
  CU_ASSERT_STRING_EQUAL(buf, "4294967295");
#if LONG_BIT == 64
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%lu", ULONG_MAX), 20);
  CU_ASSERT_STRING_EQUAL(buf, "18446744073709551615");
#else
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%lu", ULONG_MAX), 10);
  CU_ASSERT_STRING_EQUAL(buf, "4294967295");
#endif
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%llu", ULLONG_MAX), 20);
  CU_ASSERT_STRING_EQUAL(buf, "18446744073709551615");
}

static void test_sprintf_formats_number_for_f(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%f", 12.34), 9);
  CU_ASSERT_STRING_EQUAL(buf, "12.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%f", -34.56), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-34.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%f", 123.499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%f", 0.123), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%f", 12.34e9), 18);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%f", -34.56e-12), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%f", 123.499e23), 33);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%f", 0.0), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%f", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%f", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%f", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_f_with_apostrophe(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'f", 12.34), 9);
  CU_ASSERT_STRING_EQUAL(buf, "12.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'f", -34.56), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-34.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'f", 123.499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'f", 0.123), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'f", 12.34e9), 18);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'f", -34.56e-12), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'f", 123.499e23), 33);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'f", 0.0), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'f", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'f", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'f", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_f_with_plus(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+f", 12.34), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+12.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+f", -34.56), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-34.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+f", 123.499), 11);
  CU_ASSERT_STRING_EQUAL(buf, "+123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+f", 0.123), 9);
  CU_ASSERT_STRING_EQUAL(buf, "+0.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+f", 12.34e9), 19);
  CU_ASSERT_STRING_EQUAL(buf, "+12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+f", -34.56e-12), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+f", 123.499e23), 34);
  CU_ASSERT_STRING_EQUAL(buf, "+12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+f", 0.0), 9);
  CU_ASSERT_STRING_EQUAL(buf, "+0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+f", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+f", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+f", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+nan");
}

static void test_sprintf_formats_number_for_f_with_space(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% f", 12.34), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 12.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% f", -34.56), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-34.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% f", 123.499), 11);
  CU_ASSERT_STRING_EQUAL(buf, " 123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% f", 0.123), 9);
  CU_ASSERT_STRING_EQUAL(buf, " 0.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% f", 12.34e9), 19);
  CU_ASSERT_STRING_EQUAL(buf, " 12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% f", -34.56e-12), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% f", 123.499e23), 34);
  CU_ASSERT_STRING_EQUAL(buf, " 12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% f", 0.0), 9);
  CU_ASSERT_STRING_EQUAL(buf, " 0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% f", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% f", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% f", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_f_with_hash(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#f", 12.34), 9);
  CU_ASSERT_STRING_EQUAL(buf, "12.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#f", -34.56), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-34.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#f", 123.499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#f", 0.123), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#f", 12.34e9), 18);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#f", -34.56e-12), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#f", 123.499e23), 33);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#f", 0.0), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#f", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#f", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#f", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_f_with_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14f", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "     12.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14f", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "    -34.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14f", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "    123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14f", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "      0.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14f", 12.34e9), 18);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14f", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "     -0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14f", 123.499e23), 33);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14f", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "      0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14f", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14f", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14f", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6f", 12.34), 9);
  CU_ASSERT_STRING_EQUAL(buf, "12.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6f", -34.56), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-34.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6f", 123.499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6f", 0.123), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6f", 12.34e9), 18);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6f", -34.56e-12), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6f", 123.499e23), 33);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6f", 0.0), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6f", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6f", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6f", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_f_with_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5f", 12.34), 8);
  CU_ASSERT_STRING_EQUAL(buf, "12.34000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5f", -34.56), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5f", 123.499), 9);
  CU_ASSERT_STRING_EQUAL(buf, "123.49900");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5f", 0.123), 7);
  CU_ASSERT_STRING_EQUAL(buf, "0.12300");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5f", 12.34e9), 17);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5f", -34.56e-12), 8);
  CU_ASSERT_STRING_EQUAL(buf, "-0.00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5f", 123.499e23), 32);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5f", 0.0), 7);
  CU_ASSERT_STRING_EQUAL(buf, "0.00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5f", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5f", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5f", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.0f", 12.34), 2);
  CU_ASSERT_STRING_EQUAL(buf, "12");
}

static void test_sprintf_formats_number_for_f_with_hash_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5f", 12.34), 8);
  CU_ASSERT_STRING_EQUAL(buf, "12.34000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5f", -34.56), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5f", 123.499), 9);
  CU_ASSERT_STRING_EQUAL(buf, "123.49900");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5f", 0.123), 7);
  CU_ASSERT_STRING_EQUAL(buf, "0.12300");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5f", 12.34e9), 17);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5f", -34.56e-12), 8);
  CU_ASSERT_STRING_EQUAL(buf, "-0.00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5f", 123.499e23), 32);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5f", 0.0), 7);
  CU_ASSERT_STRING_EQUAL(buf, "0.00000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5f", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5f", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5f", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.0f", 12.34), 3);
  CU_ASSERT_STRING_EQUAL(buf, "12.");
}

static void test_sprintf_formats_number_for_f_with_hash_and_width_and_precision_0(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#5.0f", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "  12.");
}

static void test_sprintf_formats_number_for_f_with_0_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014f", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000012.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014f", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-000034.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014f", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014f", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014f", 12.34e9), 18);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014f", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014f", 123.499e23), 33);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014f", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014f", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014f", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014f", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06f", 12.34), 9);
  CU_ASSERT_STRING_EQUAL(buf, "12.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06f", -34.56), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-34.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06f", 123.499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06f", 0.123), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06f", 12.34e9), 18);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06f", -34.56e-12), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06f", 123.499e23), 33);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06f", 0.0), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06f", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06f", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06f", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_f_with_0_and_plus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014f", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+000012.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014f", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-000034.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014f", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+000123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014f", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+000000.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014f", 12.34e9), 19);
  CU_ASSERT_STRING_EQUAL(buf, "+12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014f", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014f", 123.499e23), 34);
  CU_ASSERT_STRING_EQUAL(buf, "+12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014f", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014f", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          +inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014f", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014f", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          +nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06f", 12.34), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+12.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06f", -34.56), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-34.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06f", 123.499), 11);
  CU_ASSERT_STRING_EQUAL(buf, "+123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06f", 0.123), 9);
  CU_ASSERT_STRING_EQUAL(buf, "+0.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06f", 12.34e9), 19);
  CU_ASSERT_STRING_EQUAL(buf, "+12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06f", -34.56e-12), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06f", 123.499e23), 34);
  CU_ASSERT_STRING_EQUAL(buf, "+12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06f", 0.0), 9);
  CU_ASSERT_STRING_EQUAL(buf, "+0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06f", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  +inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06f", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06f", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  +nan");
}

static void test_sprintf_formats_number_for_f_with_0_and_space_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014f", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 000012.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014f", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-000034.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014f", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 000123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014f", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 000000.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014f", 12.34e9), 19);
  CU_ASSERT_STRING_EQUAL(buf, " 12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014f", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014f", 123.499e23), 34);
  CU_ASSERT_STRING_EQUAL(buf, " 12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014f", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014f", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014f", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014f", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06f", 12.34), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 12.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06f", -34.56), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-34.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06f", 123.499), 11);
  CU_ASSERT_STRING_EQUAL(buf, " 123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06f", 0.123), 9);
  CU_ASSERT_STRING_EQUAL(buf, " 0.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06f", 12.34e9), 19);
  CU_ASSERT_STRING_EQUAL(buf, " 12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06f", -34.56e-12), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06f", 123.499e23), 34);
  CU_ASSERT_STRING_EQUAL(buf, " 12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06f", 0.0), 9);
  CU_ASSERT_STRING_EQUAL(buf, " 0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06f", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06f", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06f", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_f_with_0_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014f", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000012.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014f", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-000034.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014f", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014f", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014f", 12.34e9), 18);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014f", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014f", 123.499e23), 33);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014f", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014f", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014f", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014f", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06f", 12.34), 9);
  CU_ASSERT_STRING_EQUAL(buf, "12.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06f", -34.56), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-34.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06f", 123.499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06f", 0.123), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06f", 12.34e9), 18);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06f", -34.56e-12), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06f", 123.499e23), 33);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06f", 0.0), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06f", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06f", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06f", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_f_with_0_and_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14f", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000012.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14f", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-000034.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14f", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14f", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14f", 12.34e9), 18);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14f", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14f", 123.499e23), 33);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14f", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14f", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14f", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14f", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6f", 12.34), 9);
  CU_ASSERT_STRING_EQUAL(buf, "12.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6f", -34.56), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-34.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6f", 123.499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6f", 0.123), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6f", 12.34e9), 18);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6f", -34.56e-12), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6f", 123.499e23), 33);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6f", 0.0), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6f", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6f", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6f", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_f_with_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14f", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "12.340000     ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14f", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-34.560000    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14f", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "123.499000    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14f", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0.123000      ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14f", 12.34e9), 18);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14f", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-0.000000     ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14f", 123.499e23), 33);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14f", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000      ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14f", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "inf           ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14f", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-inf          ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14f", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "nan           ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6f", 12.34), 9);
  CU_ASSERT_STRING_EQUAL(buf, "12.340000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6f", -34.56), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-34.560000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6f", 123.499), 10);
  CU_ASSERT_STRING_EQUAL(buf, "123.499000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6f", 0.123), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.123000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6f", 12.34e9), 18);
  CU_ASSERT_STRING_EQUAL(buf, "12340000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6f", -34.56e-12), 9);
  CU_ASSERT_STRING_EQUAL(buf, "-0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6f", 123.499e23), 33);
  CU_ASSERT_STRING_EQUAL(buf, "12349900000000000000000000.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6f", 0.0), 8);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6f", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "inf   ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6f", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-inf  ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6f", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "nan   ");
}

static void test_sprintf_formats_number_for_e(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%e", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%e", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%e", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%e", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%e", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%e", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%e", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%e", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%e", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%e", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%e", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_e_with_apostrophe(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'e", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'e", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'e", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'e", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'e", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'e", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'e", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'e", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'e", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'e", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'e", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_e_with_plus(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+e", 12.34), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+e", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+e", 123.499), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+e", 0.123), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+e", 12.34e9), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+e", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+e", 123.499e23), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+e", 0.0), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+0.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+e", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+e", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+e", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+nan");
}

static void test_sprintf_formats_number_for_e_with_space(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% e", 12.34), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% e", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% e", 123.499), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% e", 0.123), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% e", 12.34e9), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% e", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% e", 123.499e23), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% e", 0.0), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 0.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% e", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% e", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% e", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_e_with_hash(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#e", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#e", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#e", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#e", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#e", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#e", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#e", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#e", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#e", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#e", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#e", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_e_with_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16e", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, "    1.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16e", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "   -3.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16e", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, "    1.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16e", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, "    1.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16e", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, "    1.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16e", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "   -3.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16e", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, "    1.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16e", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, "    0.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16e", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16e", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16e", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6e", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6e", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6e", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6e", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6e", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6e", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6e", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6e", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6e", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6e", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6e", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_e_with_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5e", 12.34), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23400e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5e", -34.56), 12);
  CU_ASSERT_STRING_EQUAL(buf, "-3.45600e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5e", 123.499), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5e", 0.123), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5e", 12.34e9), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23400e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5e", -34.56e-12), 12);
  CU_ASSERT_STRING_EQUAL(buf, "-3.45600e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5e", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5e", 0.0), 11);
  CU_ASSERT_STRING_EQUAL(buf, "0.00000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5e", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5e", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5e", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_e_with_0_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016e", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016e", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016e", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016e", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016e", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016e", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016e", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016e", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00000.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016e", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016e", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016e", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06e", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06e", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06e", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06e", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06e", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06e", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06e", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06e", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06e", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06e", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06e", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_e_with_0_and_plus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016e", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, "+0001.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016e", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016e", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, "+0001.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016e", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, "+0001.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016e", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, "+0001.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016e", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016e", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, "+0001.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016e", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, "+0000.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016e", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            +inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016e", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016e", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            +nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06e", 12.34), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06e", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06e", 123.499), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06e", 0.123), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06e", 12.34e9), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06e", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06e", 123.499e23), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06e", 0.0), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+0.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06e", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  +inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06e", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06e", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  +nan");
}

static void test_sprintf_formats_number_for_e_with_0_and_space_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016e", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, " 0001.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016e", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016e", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, " 0001.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016e", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, " 0001.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016e", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, " 0001.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016e", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016e", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, " 0001.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016e", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, " 0000.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016e", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016e", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016e", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06e", 12.34), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06e", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06e", 123.499), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06e", 0.123), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06e", 12.34e9), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06e", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06e", 123.499e23), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06e", 0.0), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 0.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06e", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06e", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06e", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_e_with_0_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016e", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016e", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016e", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016e", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016e", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016e", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016e", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016e", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00000.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016e", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016e", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016e", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06e", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06e", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06e", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06e", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06e", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06e", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06e", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06e", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06e", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06e", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06e", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_e_with_0_and_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16e", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16e", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16e", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16e", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16e", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16e", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16e", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16e", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00000.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16e", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16e", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16e", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6e", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6e", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6e", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6e", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6e", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6e", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6e", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6e", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6e", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6e", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6e", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_e_with_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16e", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+01    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16e", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e+01   ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16e", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+02    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16e", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000e-01    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16e", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+10    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16e", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e-11   ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16e", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+25    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16e", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000e+00    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16e", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "inf             ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16e", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-inf            ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16e", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "nan             ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6e", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6e", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6e", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6e", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000e-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6e", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6e", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6e", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6e", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000e+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6e", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "inf   ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6e", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-inf  ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6e", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "nan   ");
}

static void test_sprintf_formats_number_for_uppercase_e(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%E", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%E", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%E", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%E", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%E", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%E", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%E", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%E", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%E", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%E", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%E", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_uppercase_e_with_apostrophe(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'E", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'E", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'E", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'E", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'E", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'E", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'E", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'E", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'E", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'E", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'E", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_uppercase_e_with_plus(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+E", 12.34), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+E", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+E", 123.499), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+E", 0.123), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+E", 12.34e9), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+E", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+E", 123.499e23), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+E", 0.0), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+0.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+E", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+E", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+E", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+nan");
}

static void test_sprintf_formats_number_for_uppercase_e_with_space(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% E", 12.34), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% E", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% E", 123.499), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% E", 0.123), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% E", 12.34e9), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% E", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% E", 123.499e23), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% E", 0.0), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 0.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% E", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% E", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% E", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_uppercase_e_with_hash(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#E", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#E", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#E", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#E", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#E", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#E", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#E", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#E", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#E", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#E", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#E", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_uppercase_e_with_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16E", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, "    1.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16E", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "   -3.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16E", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, "    1.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16E", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, "    1.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16E", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, "    1.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16E", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "   -3.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16E", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, "    1.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16E", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, "    0.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16E", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16E", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%16E", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6E", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6E", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6E", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6E", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6E", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6E", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6E", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6E", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6E", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6E", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%6E", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_uppercase_e_with_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5E", 12.34), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23400E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5E", -34.56), 12);
  CU_ASSERT_STRING_EQUAL(buf, "-3.45600E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5E", 123.499), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5E", 0.123), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5E", 12.34e9), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23400E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5E", -34.56e-12), 12);
  CU_ASSERT_STRING_EQUAL(buf, "-3.45600E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5E", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5E", 0.0), 11);
  CU_ASSERT_STRING_EQUAL(buf, "0.00000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5E", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5E", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5E", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_uppercase_e_with_0_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016E", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016E", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016E", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016E", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016E", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016E", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016E", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016E", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00000.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016E", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016E", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%016E", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06E", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06E", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06E", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06E", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06E", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06E", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06E", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06E", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06E", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06E", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%06E", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_uppercase_e_with_0_and_plus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016E", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, "+0001.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016E", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016E", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, "+0001.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016E", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, "+0001.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016E", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, "+0001.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016E", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016E", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, "+0001.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016E", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, "+0000.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016E", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            +inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016E", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+016E", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            +nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06E", 12.34), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06E", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06E", 123.499), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06E", 0.123), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06E", 12.34e9), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06E", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06E", 123.499e23), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06E", 0.0), 13);
  CU_ASSERT_STRING_EQUAL(buf, "+0.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06E", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  +inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06E", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+06E", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  +nan");
}

static void test_sprintf_formats_number_for_uppercase_e_with_0_and_space_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016E", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, " 0001.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016E", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016E", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, " 0001.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016E", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, " 0001.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016E", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, " 0001.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016E", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016E", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, " 0001.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016E", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, " 0000.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016E", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016E", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 016E", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06E", 12.34), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06E", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06E", 123.499), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06E", 0.123), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06E", 12.34e9), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06E", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06E", 123.499e23), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06E", 0.0), 13);
  CU_ASSERT_STRING_EQUAL(buf, " 0.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06E", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06E", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 06E", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_uppercase_e_with_0_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016E", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016E", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016E", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016E", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016E", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016E", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016E", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016E", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00000.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016E", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016E", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#016E", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06E", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06E", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06E", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06E", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06E", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06E", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06E", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06E", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06E", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06E", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#06E", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_uppercase_e_with_0_and_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16E", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16E", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16E", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16E", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16E", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16E", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-0003.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16E", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00001.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16E", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, "00000.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16E", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16E", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "            -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-16E", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "             nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6E", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6E", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6E", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6E", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6E", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6E", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6E", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6E", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6E", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6E", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "  -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-6E", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "   nan");
}

static void test_sprintf_formats_number_for_uppercase_e_with_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16E", 12.34), 16);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+01    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16E", -34.56), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E+01   ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16E", 123.499), 16);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+02    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16E", 0.123), 16);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000E-01    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16E", 12.34e9), 16);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+10    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16E", -34.56e-12), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E-11   ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16E", 123.499e23), 16);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+25    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16E", 0.0), 16);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000E+00    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16E", HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "inf             ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16E", -HUGE_VAL), 16);
  CU_ASSERT_STRING_EQUAL(buf, "-inf            ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-16E", NAN), 16);
  CU_ASSERT_STRING_EQUAL(buf, "nan             ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6E", 12.34), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6E", -34.56), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E+01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6E", 123.499), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+02");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6E", 0.123), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.230000E-01");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6E", 12.34e9), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234000E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6E", -34.56e-12), 13);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456000E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6E", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "1.234990E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6E", 0.0), 12);
  CU_ASSERT_STRING_EQUAL(buf, "0.000000E+00");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6E", HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "inf   ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6E", -HUGE_VAL), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-inf  ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-6E", NAN), 6);
  CU_ASSERT_STRING_EQUAL(buf, "nan   ");
}

static void test_sprintf_formats_number_for_g(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%g", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%g", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%g", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%g", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%g", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%g", 0.0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%g", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%g", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_g_with_apostrophe(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'g", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'g", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'g", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'g", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'g", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'g", 0.0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'g", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'g", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_g_with_plus(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+g", 12.34), 6);
  CU_ASSERT_STRING_EQUAL(buf, "+12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+g", 123.499), 8);
  CU_ASSERT_STRING_EQUAL(buf, "+123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+g", 0.123), 6);
  CU_ASSERT_STRING_EQUAL(buf, "+0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+g", 12.34e9), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+g", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "+1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+g", 0.0), 2);
  CU_ASSERT_STRING_EQUAL(buf, "+0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+g", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+g", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+nan");
}

static void test_sprintf_formats_number_for_g_with_space(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% g", 12.34), 6);
  CU_ASSERT_STRING_EQUAL(buf, " 12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% g", 123.499), 8);
  CU_ASSERT_STRING_EQUAL(buf, " 123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% g", 0.123), 6);
  CU_ASSERT_STRING_EQUAL(buf, " 0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% g", 12.34e9), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% g", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, " 1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% g", 0.0), 2);
  CU_ASSERT_STRING_EQUAL(buf, " 0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% g", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% g", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_g_with_hash(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#g", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#g", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#g", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#g", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#g", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#g", 0.0), 2);
  CU_ASSERT_STRING_EQUAL(buf, "0.");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#g", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#g", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_g_with_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14g", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "         12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14g", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "        -34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14g", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "       123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14g", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "         0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14g", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, "     1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14g", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "    -3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14g", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, "   1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14g", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "             0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14g", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14g", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14g", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4g", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4g", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4g", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4g", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4g", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4g", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, "   0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4g", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4g", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_g_with_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5g", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5g", 123.499), 5);
  CU_ASSERT_STRING_EQUAL(buf, "123.5");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5g", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5g", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5g", 123.499e23), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.235e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5g", 0.0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5g", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5g", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.2g", 12.34), 2);
  CU_ASSERT_STRING_EQUAL(buf, "12");
}

static void test_sprintf_formats_number_for_g_with_hash_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5g", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5g", 123.499), 5);
  CU_ASSERT_STRING_EQUAL(buf, "123.5");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5g", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5g", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5g", 123.499e23), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.235e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5g", 0.0), 2);
  CU_ASSERT_STRING_EQUAL(buf, "0.");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5g", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5g", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.2g", 12.34), 3);
  CU_ASSERT_STRING_EQUAL(buf, "12.");
}

static void test_sprintf_formats_number_for_g_with_hash_and_width_and_precision_2(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#5.2g", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "  12.");
}

static void test_sprintf_formats_number_for_g_with_0_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014g", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "00000000012.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014g", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-0000000034.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014g", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014g", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014g", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, "000001.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014g", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-00003.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014g", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0001.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014g", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "00000000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014g", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014g", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014g", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04g", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04g", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04g", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04g", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04g", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04g", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, "0000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04g", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04g", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_g_with_0_and_plus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014g", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+0000000012.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014g", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-0000000034.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014g", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+000000123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014g", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+000000000.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014g", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+00001.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014g", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-00003.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014g", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+001.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014g", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+0000000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014g", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          +inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014g", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014g", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          +nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04g", 12.34), 6);
  CU_ASSERT_STRING_EQUAL(buf, "+12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04g", 123.499), 8);
  CU_ASSERT_STRING_EQUAL(buf, "+123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04g", 0.123), 6);
  CU_ASSERT_STRING_EQUAL(buf, "+0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04g", 12.34e9), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04g", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "+1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04g", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04g", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04g", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+nan");
}

static void test_sprintf_formats_number_for_g_with_0_and_space_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014g", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 0000000012.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014g", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-0000000034.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014g", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 000000123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014g", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 000000000.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014g", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 00001.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014g", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-00003.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014g", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 001.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014g", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 0000000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014g", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014g", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014g", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04g", 12.34), 6);
  CU_ASSERT_STRING_EQUAL(buf, " 12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04g", 123.499), 8);
  CU_ASSERT_STRING_EQUAL(buf, " 123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04g", 0.123), 6);
  CU_ASSERT_STRING_EQUAL(buf, " 0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04g", 12.34e9), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04g", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, " 1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04g", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, " 000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04g", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04g", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_g_with_0_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014g", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "00000000012.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014g", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-0000000034.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014g", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014g", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014g", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, "000001.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014g", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-00003.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014g", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0001.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014g", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000000.");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014g", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014g", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014g", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04g", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04g", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04g", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04g", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04g", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04g", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, "000.");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04g", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04g", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_g_with_0_and_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14g", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "00000000012.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14g", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-0000000034.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14g", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14g", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14g", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, "000001.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14g", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-00003.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14g", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0001.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14g", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "00000000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14g", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14g", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14g", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4g", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4g", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4g", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4g", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4g", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4g", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, "0000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4g", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4g", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_g_with_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14g", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "12.34         ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14g", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56        ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14g", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "123.499       ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14g", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0.123         ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14g", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, "1.234e+10     ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14g", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14g", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499e+25   ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14g", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0             ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14g", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "inf           ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14g", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-inf          ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14g", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "nan           ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4g", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4g", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4g", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4g", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4g", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234e+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4g", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456e-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4g", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499e+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4g", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, "0   ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4g", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "inf ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4g", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4g", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, "nan ");
}

static void test_sprintf_formats_number_for_uppercase_g(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%G", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%G", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%G", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%G", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%G", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%G", 0.0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%G", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%G", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_uppercase_g_with_apostrophe(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'G", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'G", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'G", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'G", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'G", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'G", 0.0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'G", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%'G", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_uppercase_g_with_plus(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+G", 12.34), 6);
  CU_ASSERT_STRING_EQUAL(buf, "+12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+G", 123.499), 8);
  CU_ASSERT_STRING_EQUAL(buf, "+123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+G", 0.123), 6);
  CU_ASSERT_STRING_EQUAL(buf, "+0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+G", 12.34e9), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+G", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "+1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+G", 0.0), 2);
  CU_ASSERT_STRING_EQUAL(buf, "+0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+G", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+G", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+nan");
}

static void test_sprintf_formats_number_for_uppercase_g_with_space(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% G", 12.34), 6);
  CU_ASSERT_STRING_EQUAL(buf, " 12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% G", 123.499), 8);
  CU_ASSERT_STRING_EQUAL(buf, " 123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% G", 0.123), 6);
  CU_ASSERT_STRING_EQUAL(buf, " 0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% G", 12.34e9), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% G", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, " 1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% G", 0.0), 2);
  CU_ASSERT_STRING_EQUAL(buf, " 0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% G", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% G", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_uppercase_g_with_hash(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#G", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#G", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#G", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#G", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#G", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#G", 0.0), 2);
  CU_ASSERT_STRING_EQUAL(buf, "0.");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#G", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#G", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
}

static void test_sprintf_formats_number_for_uppercase_g_with_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14G", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "         12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14G", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "        -34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14G", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "       123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14G", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "         0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14G", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, "     1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14G", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "    -3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14G", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, "   1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14G", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "             0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14G", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14G", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%14G", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4G", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4G", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4G", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4G", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4G", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4G", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, "   0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4G", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4G", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_uppercase_g_with_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5G", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5G", 123.499), 5);
  CU_ASSERT_STRING_EQUAL(buf, "123.5");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5G", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5G", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5G", 123.499e23), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.235E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5G", 0.0), 1);
  CU_ASSERT_STRING_EQUAL(buf, "0");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5G", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.5G", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.2G", 12.34), 2);
  CU_ASSERT_STRING_EQUAL(buf, "12");
}

static void test_sprintf_formats_number_for_uppercase_g_with_hash_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5G", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5G", 123.499), 5);
  CU_ASSERT_STRING_EQUAL(buf, "123.5");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5G", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5G", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5G", 123.499e23), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.235E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5G", 0.0), 2);
  CU_ASSERT_STRING_EQUAL(buf, "0.");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5G", HUGE_VAL), 3);
  CU_ASSERT_STRING_EQUAL(buf, "inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.5G", NAN), 3);
  CU_ASSERT_STRING_EQUAL(buf, "nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#.2G", 12.34), 3);
  CU_ASSERT_STRING_EQUAL(buf, "12.");
}

static void test_sprintf_formats_number_for_uppercase_g_with_hash_and_width_and_precision_2(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#5.2G", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "  12.");
}

static void test_sprintf_formats_number_for_uppercase_g_with_0_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014G", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "00000000012.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014G", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-0000000034.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014G", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014G", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014G", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, "000001.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014G", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-00003.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014G", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0001.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014G", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "00000000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014G", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014G", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%014G", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04G", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04G", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04G", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04G", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04G", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04G", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, "0000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04G", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%04G", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_uppercase_g_with_0_and_plus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014G", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+0000000012.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014G", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-0000000034.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014G", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+000000123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014G", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+000000000.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014G", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+00001.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014G", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-00003.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014G", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+001.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014G", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "+0000000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014G", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          +inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014G", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+014G", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          +nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04G", 12.34), 6);
  CU_ASSERT_STRING_EQUAL(buf, "+12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04G", 123.499), 8);
  CU_ASSERT_STRING_EQUAL(buf, "+123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04G", 0.123), 6);
  CU_ASSERT_STRING_EQUAL(buf, "+0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04G", 12.34e9), 10);
  CU_ASSERT_STRING_EQUAL(buf, "+1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04G", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, "+1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04G", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04G", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%+04G", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, "+nan");
}

static void test_sprintf_formats_number_for_uppercase_g_with_0_and_space_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014G", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 0000000012.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014G", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-0000000034.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014G", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 000000123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014G", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 000000000.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014G", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 00001.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014G", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-00003.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014G", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 001.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014G", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, " 0000000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014G", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014G", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 014G", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04G", 12.34), 6);
  CU_ASSERT_STRING_EQUAL(buf, " 12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04G", 123.499), 8);
  CU_ASSERT_STRING_EQUAL(buf, " 123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04G", 0.123), 6);
  CU_ASSERT_STRING_EQUAL(buf, " 0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04G", 12.34e9), 10);
  CU_ASSERT_STRING_EQUAL(buf, " 1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04G", 123.499e23), 12);
  CU_ASSERT_STRING_EQUAL(buf, " 1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04G", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, " 000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04G", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "% 04G", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_uppercase_g_with_0_and_hash_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014G", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "00000000012.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014G", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-0000000034.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014G", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014G", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014G", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, "000001.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014G", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-00003.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014G", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0001.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014G", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000000.");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014G", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014G", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#014G", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04G", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04G", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04G", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04G", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04G", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04G", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, "000.");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04G", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%#04G", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_uppercase_g_with_0_and_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14G", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "00000000012.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14G", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-0000000034.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14G", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14G", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0000000000.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14G", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, "000001.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14G", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-00003.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14G", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0001.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14G", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "00000000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14G", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14G", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "          -inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-14G", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "           nan");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4G", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4G", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4G", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4G", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4G", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4G", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, "0000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4G", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, " inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%0-4G", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, " nan");
}

static void test_sprintf_formats_number_for_uppercase_g_with_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14G", 12.34), 14);
  CU_ASSERT_STRING_EQUAL(buf, "12.34         ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14G", -34.56), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56        ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14G", 123.499), 14);
  CU_ASSERT_STRING_EQUAL(buf, "123.499       ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14G", 0.123), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0.123         ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14G", 12.34e9), 14);
  CU_ASSERT_STRING_EQUAL(buf, "1.234E+10     ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14G", -34.56e-12), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14G", 123.499e23), 14);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499E+25   ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14G", 0.0), 14);
  CU_ASSERT_STRING_EQUAL(buf, "0             ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14G", HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "inf           ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14G", -HUGE_VAL), 14);
  CU_ASSERT_STRING_EQUAL(buf, "-inf          ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-14G", NAN), 14);
  CU_ASSERT_STRING_EQUAL(buf, "nan           ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4G", 12.34), 5);
  CU_ASSERT_STRING_EQUAL(buf, "12.34");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4G", -34.56), 6);
  CU_ASSERT_STRING_EQUAL(buf, "-34.56");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4G", 123.499), 7);
  CU_ASSERT_STRING_EQUAL(buf, "123.499");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4G", 0.123), 5);
  CU_ASSERT_STRING_EQUAL(buf, "0.123");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4G", 12.34e9), 9);
  CU_ASSERT_STRING_EQUAL(buf, "1.234E+10");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4G", -34.56e-12), 10);
  CU_ASSERT_STRING_EQUAL(buf, "-3.456E-11");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4G", 123.499e23), 11);
  CU_ASSERT_STRING_EQUAL(buf, "1.23499E+25");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4G", 0.0), 4);
  CU_ASSERT_STRING_EQUAL(buf, "0   ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4G", HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "inf ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4G", -HUGE_VAL), 4);
  CU_ASSERT_STRING_EQUAL(buf, "-inf");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4G", NAN), 4);
  CU_ASSERT_STRING_EQUAL(buf, "nan ");
}

static void test_sprintf_formats_character_for_c(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%c", 'a'), 1);
  CU_ASSERT_STRING_EQUAL(buf, "a");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%c", 'b'), 1);
  CU_ASSERT_STRING_EQUAL(buf, "b");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%c", 'c'), 1);
  CU_ASSERT_STRING_EQUAL(buf, "c");
}

static void test_sprintf_formats_character_for_c_with_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5c", 'a'), 5);
  CU_ASSERT_STRING_EQUAL(buf, "    a");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5c", 'b'), 5);
  CU_ASSERT_STRING_EQUAL(buf, "    b");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%5c", 'c'), 5);
  CU_ASSERT_STRING_EQUAL(buf, "    c");
}

static void test_sprintf_formats_character_for_c_with_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-5c", 'a'), 5);
  CU_ASSERT_STRING_EQUAL(buf, "a    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-5c", 'b'), 5);
  CU_ASSERT_STRING_EQUAL(buf, "b    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-5c", 'c'), 5);
  CU_ASSERT_STRING_EQUAL(buf, "c    ");
}

static void test_sprintf_formats_string_for_s(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%s", "abcdef"), 6);
  CU_ASSERT_STRING_EQUAL(buf, "abcdef");
}

static void test_sprintf_formats_string_for_s_with_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10s", "abcdef"), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    abcdef");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4s", "abcdef"), 6);
  CU_ASSERT_STRING_EQUAL(buf, "abcdef");
}

static void test_sprintf_formats_string_for_s_with_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.10s", "abcdef"), 6);
  CU_ASSERT_STRING_EQUAL(buf, "abcdef");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%.4s", "abcdef"), 4);
  CU_ASSERT_STRING_EQUAL(buf, "abcd");
}

static void test_sprintf_formats_string_for_s_with_width_and_precision(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.8s", "abcdef"), 10);
  CU_ASSERT_STRING_EQUAL(buf, "    abcdef");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4.8s", "abcdef"), 6);
  CU_ASSERT_STRING_EQUAL(buf, "abcdef");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10.2s", "abcdef"), 10);
  CU_ASSERT_STRING_EQUAL(buf, "        ab");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4.2s", "abcdef"), 4);
  CU_ASSERT_STRING_EQUAL(buf, "  ab");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%8.10s", "abcdef"), 8);
  CU_ASSERT_STRING_EQUAL(buf, "  abcdef");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%8.10s", "abcdef"), 8);
  CU_ASSERT_STRING_EQUAL(buf, "  abcdef");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4.10s", "abcdef"), 6);
  CU_ASSERT_STRING_EQUAL(buf, "abcdef");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%4.10s", "abcdef"), 6);
  CU_ASSERT_STRING_EQUAL(buf, "abcdef");
}

static void test_sprintf_formats_string_for_s_with_minus_and_width(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-10s", "abcdef"), 10);
  CU_ASSERT_STRING_EQUAL(buf, "abcdef    ");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%-4s", "abcdef"), 6);
  CU_ASSERT_STRING_EQUAL(buf, "abcdef");
}

static void test_sprintf_formats_pointer_for_p(void)
{
  static char buf[100];
#if LONG_BIT == 64
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%p", NULL), 18);
  CU_ASSERT_STRING_EQUAL(buf, "0x0000000000000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%p", (void *) 0x1234567890abcdefUL), 18);
  CU_ASSERT_STRING_EQUAL(buf, "0x1234567890abcdef");
#else
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%p", NULL), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x00000000");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%p", (void *) 0x12345678U), 10);
  CU_ASSERT_STRING_EQUAL(buf, "0x12345678");
#endif
}

static void test_sprintf_stores_number_of_written_bytes_to_variable(void)
{
  static char buf[100];
  int n1, n2, n3;
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%nn=%d%n s=%s%n", &n1, 1234, &n2, "abc", &n3), 12);
  CU_ASSERT_STRING_EQUAL(buf, "n=1234 s=abc");
  CU_ASSERT_EQUAL(n1, 0);
  CU_ASSERT_EQUAL(n2, 6);
  CU_ASSERT_EQUAL(n3, 12);
}

static void test_sprintf_formats_percent_for_percent(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "abc%%def"), 7);
  CU_ASSERT_STRING_EQUAL(buf, "abc%def");
}

static void test_sprintf_formats_invlid_conversion_specificator(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%k"), 2);
  CU_ASSERT_STRING_EQUAL(buf, "%k");
  CU_ASSERT_EQUAL(retrolibc_sprintf(buf, "%10"), 3);
  CU_ASSERT_STRING_EQUAL(buf, "%10");
}

static void test_snprintf_formats_values(void)
{
  static char buf[100];
  CU_ASSERT_EQUAL(retrolibc_snprintf(buf, 30, "n=%d s=%s f=%f\n", 1234, "abc", 34.56), 25);
  CU_ASSERT_STRING_EQUAL(buf, "n=1234 s=abc f=34.560000\n");
  CU_ASSERT_EQUAL(retrolibc_snprintf(buf, 10, "n=%d s=%s f=%f\n", 1234, "abc", 34.56), 9);
  CU_ASSERT_STRING_EQUAL(buf, "n=1234 s=");
}

static void test_fprintf_prints_values_to_file(void)
{
  static char s[100];
  FILE *f;
  clean_current_dir();
  f = fopen("test.txt", "w");
  CU_ASSERT_EQUAL(retrolibc_fprintf(f, "n=%d s=%s f=%f\n", 1234, "abc", 34.56), 25);
  fclose(f);
  CU_ASSERT_FATAL(read_from_file("test.txt", s, 100));
  CU_ASSERT_STRING_EQUAL(s, "n=1234 s=abc f=34.560000\n");
}

int add_fprintf_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("fprintf suite", initialize_fprintf, cleanup_fprintf)) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats values", test_sprintf_formats_values) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d", test_sprintf_formats_number_for_d) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with '", test_sprintf_formats_number_for_d_with_apostrophe) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with +", test_sprintf_formats_number_for_d_with_plus) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with space", test_sprintf_formats_number_for_d_with_space) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with #", test_sprintf_formats_number_for_d_with_hash) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with width", test_sprintf_formats_number_for_d_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with precision", test_sprintf_formats_number_for_d_with_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with # and precision", test_sprintf_formats_number_for_d_with_hash_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with width and precision", test_sprintf_formats_number_for_d_with_width_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with 0 and width", test_sprintf_formats_number_for_d_with_0_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with 0 and + and width", test_sprintf_formats_number_for_d_with_0_and_plus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with 0 and space and width", test_sprintf_formats_number_for_d_with_0_and_space_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with 0 and # and width", test_sprintf_formats_number_for_d_with_0_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with 0 and + and # and width", test_sprintf_formats_number_for_d_with_0_and_plus_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with 0 and space and # and width", test_sprintf_formats_number_for_d_with_0_and_space_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with 0 and - and width", test_sprintf_formats_number_for_d_with_0_and_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for d with - and width", test_sprintf_formats_number_for_d_with_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i", test_sprintf_formats_number_for_i) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with '", test_sprintf_formats_number_for_i_with_apostrophe) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with +", test_sprintf_formats_number_for_i_with_plus) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with space", test_sprintf_formats_number_for_i_with_space) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with #", test_sprintf_formats_number_for_i_with_hash) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with width", test_sprintf_formats_number_for_i_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with precision", test_sprintf_formats_number_for_i_with_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with # and precision", test_sprintf_formats_number_for_i_with_hash_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with width and precision", test_sprintf_formats_number_for_i_with_width_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with 0 and width", test_sprintf_formats_number_for_i_with_0_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with 0 and + and width", test_sprintf_formats_number_for_i_with_0_and_plus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with 0 and space and width", test_sprintf_formats_number_for_i_with_0_and_space_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with 0 and # and width", test_sprintf_formats_number_for_i_with_0_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with 0 and + and # and width", test_sprintf_formats_number_for_i_with_0_and_plus_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with 0 and space and # and width", test_sprintf_formats_number_for_i_with_0_and_space_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with 0 and - and width", test_sprintf_formats_number_for_i_with_0_and_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with - and width", test_sprintf_formats_number_for_i_with_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o", test_sprintf_formats_number_for_o) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with '", test_sprintf_formats_number_for_o_with_apostrophe) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with +", test_sprintf_formats_number_for_o_with_plus) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with space", test_sprintf_formats_number_for_o_with_space) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with #", test_sprintf_formats_number_for_o_with_hash) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with width", test_sprintf_formats_number_for_o_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with precision", test_sprintf_formats_number_for_o_with_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with # and precision", test_sprintf_formats_number_for_o_with_hash_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with width and precision", test_sprintf_formats_number_for_o_with_width_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with 0 and width", test_sprintf_formats_number_for_o_with_0_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with 0 and + and width", test_sprintf_formats_number_for_o_with_0_and_plus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with 0 and space and width", test_sprintf_formats_number_for_o_with_0_and_space_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with 0 and # and width", test_sprintf_formats_number_for_o_with_0_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with 0 and + and # and width", test_sprintf_formats_number_for_o_with_0_and_plus_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with 0 and space and # and width", test_sprintf_formats_number_for_o_with_0_and_space_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with 0 and - and width", test_sprintf_formats_number_for_o_with_0_and_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for o with - and width", test_sprintf_formats_number_for_o_with_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u", test_sprintf_formats_number_for_u) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with '", test_sprintf_formats_number_for_u_with_apostrophe) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with +", test_sprintf_formats_number_for_u_with_plus) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with space", test_sprintf_formats_number_for_u_with_space) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with #", test_sprintf_formats_number_for_u_with_hash) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with width", test_sprintf_formats_number_for_u_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with precision", test_sprintf_formats_number_for_u_with_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with # and precision", test_sprintf_formats_number_for_u_with_hash_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with width and precision", test_sprintf_formats_number_for_u_with_width_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with 0 and width", test_sprintf_formats_number_for_u_with_0_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with 0 and + and width", test_sprintf_formats_number_for_u_with_0_and_plus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with 0 and space and width", test_sprintf_formats_number_for_u_with_0_and_space_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with 0 and # and width", test_sprintf_formats_number_for_u_with_0_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with 0 and + and # and width", test_sprintf_formats_number_for_u_with_0_and_plus_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with 0 and space and # and width", test_sprintf_formats_number_for_u_with_0_and_space_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with 0 and - and width", test_sprintf_formats_number_for_u_with_0_and_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with - and width", test_sprintf_formats_number_for_u_with_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x", test_sprintf_formats_number_for_x) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with '", test_sprintf_formats_number_for_x_with_apostrophe) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with +", test_sprintf_formats_number_for_x_with_plus) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with space", test_sprintf_formats_number_for_x_with_space) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with #", test_sprintf_formats_number_for_x_with_hash) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with width", test_sprintf_formats_number_for_x_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with precision", test_sprintf_formats_number_for_x_with_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with # and precision", test_sprintf_formats_number_for_x_with_hash_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with width and precision", test_sprintf_formats_number_for_x_with_width_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with 0 and width", test_sprintf_formats_number_for_x_with_0_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with 0 and + and width", test_sprintf_formats_number_for_x_with_0_and_plus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with 0 and space and width", test_sprintf_formats_number_for_x_with_0_and_space_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with 0 and # and width", test_sprintf_formats_number_for_x_with_0_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with 0 and + and # and width", test_sprintf_formats_number_for_x_with_0_and_plus_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with 0 and space and # and width", test_sprintf_formats_number_for_x_with_0_and_space_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with 0 and - and width", test_sprintf_formats_number_for_x_with_0_and_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for x with - and width", test_sprintf_formats_number_for_x_with_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X", test_sprintf_formats_number_for_uppercase_x) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with '", test_sprintf_formats_number_for_uppercase_x_with_apostrophe) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with +", test_sprintf_formats_number_for_uppercase_x_with_plus) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with space", test_sprintf_formats_number_for_uppercase_x_with_space) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with #", test_sprintf_formats_number_for_uppercase_x_with_hash) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with width", test_sprintf_formats_number_for_uppercase_x_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with precision", test_sprintf_formats_number_for_uppercase_x_with_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with # and precision", test_sprintf_formats_number_for_uppercase_x_with_hash_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with width and precision", test_sprintf_formats_number_for_uppercase_x_with_width_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with 0 and width", test_sprintf_formats_number_for_uppercase_x_with_0_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with 0 and + and width", test_sprintf_formats_number_for_uppercase_x_with_0_and_plus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with 0 and space and width", test_sprintf_formats_number_for_uppercase_x_with_0_and_space_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with 0 and # and width", test_sprintf_formats_number_for_uppercase_x_with_0_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with 0 and + and # and width", test_sprintf_formats_number_for_uppercase_x_with_0_and_plus_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with 0 and space and # and width", test_sprintf_formats_number_for_uppercase_x_with_0_and_space_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with 0 and - and width", test_sprintf_formats_number_for_uppercase_x_with_0_and_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for X with - and width", test_sprintf_formats_number_for_uppercase_x_with_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for i with length modifier", test_sprintf_formats_number_for_i_with_length_modifier) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for u with length modifier", test_sprintf_formats_number_for_u_with_length_modifier) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f", test_sprintf_formats_number_for_f) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with '", test_sprintf_formats_number_for_f_with_apostrophe) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with +", test_sprintf_formats_number_for_f_with_plus) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with space", test_sprintf_formats_number_for_f_with_space) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with #", test_sprintf_formats_number_for_f_with_hash) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with width", test_sprintf_formats_number_for_f_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with precision", test_sprintf_formats_number_for_f_with_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with # and precision", test_sprintf_formats_number_for_f_with_hash_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with # and width and precision 0", test_sprintf_formats_number_for_f_with_hash_and_width_and_precision_0) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with 0 and width", test_sprintf_formats_number_for_f_with_0_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with 0 and + and width", test_sprintf_formats_number_for_f_with_0_and_plus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with 0 and space and width", test_sprintf_formats_number_for_f_with_0_and_space_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with 0 and # and width", test_sprintf_formats_number_for_f_with_0_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with 0 and - and width", test_sprintf_formats_number_for_f_with_0_and_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for f with - and width", test_sprintf_formats_number_for_f_with_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for e", test_sprintf_formats_number_for_e) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for e with '", test_sprintf_formats_number_for_e_with_apostrophe) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for e with +", test_sprintf_formats_number_for_e_with_plus) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for e with space", test_sprintf_formats_number_for_e_with_space) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for e with #", test_sprintf_formats_number_for_e_with_hash) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for e with width", test_sprintf_formats_number_for_e_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for e with precision", test_sprintf_formats_number_for_e_with_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for e with 0 and width", test_sprintf_formats_number_for_e_with_0_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for e with 0 and + and width", test_sprintf_formats_number_for_e_with_0_and_plus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for e with 0 and space and width", test_sprintf_formats_number_for_e_with_0_and_space_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for e with 0 and # and width", test_sprintf_formats_number_for_e_with_0_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for e with 0 and - and width", test_sprintf_formats_number_for_e_with_0_and_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for e with - and width", test_sprintf_formats_number_for_e_with_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for E", test_sprintf_formats_number_for_uppercase_e) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for E with '", test_sprintf_formats_number_for_uppercase_e_with_apostrophe) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for E with +", test_sprintf_formats_number_for_uppercase_e_with_plus) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for E with space", test_sprintf_formats_number_for_uppercase_e_with_space) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for E with #", test_sprintf_formats_number_for_uppercase_e_with_hash) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for E with width", test_sprintf_formats_number_for_uppercase_e_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for E with precision", test_sprintf_formats_number_for_uppercase_e_with_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for E with 0 and width", test_sprintf_formats_number_for_uppercase_e_with_0_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for E with 0 and + and width", test_sprintf_formats_number_for_uppercase_e_with_0_and_plus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for E with 0 and space and width", test_sprintf_formats_number_for_uppercase_e_with_0_and_space_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for E with 0 and # and width", test_sprintf_formats_number_for_uppercase_e_with_0_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for E with 0 and - and width", test_sprintf_formats_number_for_uppercase_e_with_0_and_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for E with - and width", test_sprintf_formats_number_for_uppercase_e_with_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g", test_sprintf_formats_number_for_g) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g witb '", test_sprintf_formats_number_for_g_with_apostrophe) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g with +", test_sprintf_formats_number_for_g_with_plus) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g with space", test_sprintf_formats_number_for_g_with_space) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g with #", test_sprintf_formats_number_for_g_with_hash) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g with width", test_sprintf_formats_number_for_g_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g with precision", test_sprintf_formats_number_for_g_with_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g with # and precision", test_sprintf_formats_number_for_g_with_hash_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g with # and width and precision 2", test_sprintf_formats_number_for_g_with_hash_and_width_and_precision_2) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g with 0 and width", test_sprintf_formats_number_for_g_with_0_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g with 0 and + and width", test_sprintf_formats_number_for_g_with_0_and_plus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g with 0 and space and width", test_sprintf_formats_number_for_g_with_0_and_space_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g with 0 and # and width", test_sprintf_formats_number_for_g_with_0_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g with 0 and - and width", test_sprintf_formats_number_for_g_with_0_and_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for g with - and width", test_sprintf_formats_number_for_g_with_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G", test_sprintf_formats_number_for_uppercase_g) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G witb '", test_sprintf_formats_number_for_uppercase_g_with_apostrophe) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G with +", test_sprintf_formats_number_for_uppercase_g_with_plus) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G with space", test_sprintf_formats_number_for_uppercase_g_with_space) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G with #", test_sprintf_formats_number_for_uppercase_g_with_hash) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G with width", test_sprintf_formats_number_for_uppercase_g_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G with precision", test_sprintf_formats_number_for_uppercase_g_with_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G with # and precision", test_sprintf_formats_number_for_uppercase_g_with_hash_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G with # and width and precision 2", test_sprintf_formats_number_for_uppercase_g_with_hash_and_width_and_precision_2) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G with 0 and width", test_sprintf_formats_number_for_uppercase_g_with_0_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G with 0 and + and width", test_sprintf_formats_number_for_uppercase_g_with_0_and_plus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G with 0 and space and width", test_sprintf_formats_number_for_uppercase_g_with_0_and_space_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G with 0 and # and width", test_sprintf_formats_number_for_uppercase_g_with_0_and_hash_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G with 0 and - and width", test_sprintf_formats_number_for_uppercase_g_with_0_and_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats number for G with - and width", test_sprintf_formats_number_for_uppercase_g_with_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats character for c", test_sprintf_formats_character_for_c) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats character for c with width", test_sprintf_formats_character_for_c_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats character for c with - and width", test_sprintf_formats_character_for_c_with_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats string for s", test_sprintf_formats_string_for_s) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats string for s with width", test_sprintf_formats_string_for_s_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats string for s with precision", test_sprintf_formats_string_for_s_with_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats string for s with width and precision", test_sprintf_formats_string_for_s_with_width_and_precision) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats string for s with - and width", test_sprintf_formats_string_for_s_with_minus_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats pointer for p", test_sprintf_formats_pointer_for_p) == NULL) return -1;
  if(CU_add_test(suite, "sprintf stores number of written bytes to variable", test_sprintf_stores_number_of_written_bytes_to_variable) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats % for %", test_sprintf_formats_percent_for_percent) == NULL) return -1;
  if(CU_add_test(suite, "sprintf formats invalid conversion specificator", test_sprintf_formats_invlid_conversion_specificator) == NULL) return -1;
  if(CU_add_test(suite, "snprintf formats values", test_snprintf_formats_values) == NULL) return -1;
  if(CU_add_test(suite, "fprintf prints values to file", test_fprintf_prints_values_to_file) == NULL) return -1;
  return 0;
}

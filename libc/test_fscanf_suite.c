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

static int initialize_fscanf(void)
{ return 0; }

static int cleanup_fscanf(void)
{
  clean_current_dir();
  return 0;
}

static void test_sscanf_parses_values(void)
{
  int x;
  static char s[100];
  float y;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234 abc 34.56", "%d%s%f", &x, s, &y), 3);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_STRING_EQUAL(s, "abc");
  CU_ASSERT(fabs(y - 34.56) <= 0.0001);
}

static void test_sscanf_parses_number_for_d(void)
{
  int x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%d", &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-3456", "%d", &x), 1);
  CU_ASSERT_EQUAL(x, -3456);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123499", "%d", &x), 1);
  CU_ASSERT_EQUAL(x, 123499);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+1234", "%d", &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%d", &x), 1);
  CU_ASSERT_EQUAL(x, 0);
}

static void test_sscanf_parses_number_for_i(void)
{
  int x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-3456", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, -3456);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123499", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, 123499);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+1234", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, 0);
}

static void test_sscanf_parses_number_for_i_and_prefix_0(void)
{
  int x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("01234", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, 01234);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-03456", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, -03456);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0123477", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, 0123477);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+01234", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, 01234);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, 0);
}

static void test_sscanf_parses_number_for_i_and_prefix_0x(void)
{
  int x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("0x1234", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, 0x1234);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-0x12aF", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, -0x12af);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0XfA1299", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, 0xfa1299);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+0X1234", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, 0x1234);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0x0", "%i", &x), 1);
  CU_ASSERT_EQUAL(x, 0);
}

static void test_sscanf_parses_number_for_o(void)
{
  unsigned x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%o", &x), 1);
  CU_ASSERT_EQUAL(x, 01234U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-3456", "%o", &x), 1);
  CU_ASSERT_EQUAL(x, (unsigned) (-03456));
  CU_ASSERT_EQUAL(retrolibc_sscanf("123477", "%o", &x), 1);
  CU_ASSERT_EQUAL(x, 0123477U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+1234", "%o", &x), 1);
  CU_ASSERT_EQUAL(x, 01234U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%o", &x), 1);
  CU_ASSERT_EQUAL(x, 0U);
}

static void test_sscanf_parses_number_for_u(void)
{
  unsigned x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%u", &x), 1);
  CU_ASSERT_EQUAL(x, 1234U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-3456", "%u", &x), 1);
  CU_ASSERT_EQUAL(x, (unsigned) (-3456));
  CU_ASSERT_EQUAL(retrolibc_sscanf("123499", "%u", &x), 1);
  CU_ASSERT_EQUAL(x, 123499U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+1234", "%u", &x), 1);
  CU_ASSERT_EQUAL(x, 1234U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%u", &x), 1);
  CU_ASSERT_EQUAL(x, 0U);
}

static void test_sscanf_parses_number_for_x(void)
{
  unsigned x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%x", &x), 1);
  CU_ASSERT_EQUAL(x, 0x1234U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-12aF", "%x", &x), 1);
  CU_ASSERT_EQUAL(x, (unsigned) (-0x12af));
  CU_ASSERT_EQUAL(retrolibc_sscanf("fA1299", "%x", &x), 1);
  CU_ASSERT_EQUAL(x, 0xfa1299U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+1234", "%x", &x), 1);
  CU_ASSERT_EQUAL(x, 0x1234U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%x", &x), 1);
  CU_ASSERT_EQUAL(x, 0U);
}

static void test_sscanf_parses_number_for_x_and_prefix_0x(void)
{
  unsigned x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("0x1234", "%x", &x), 1);
  CU_ASSERT_EQUAL(x, 0x1234U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-0x12aF", "%x", &x), 1);
  CU_ASSERT_EQUAL(x, (unsigned) (-0x12af));
  CU_ASSERT_EQUAL(retrolibc_sscanf("0XfA1299", "%x", &x), 1);
  CU_ASSERT_EQUAL(x, 0xfa1299U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+0X1234", "%x", &x), 1);
  CU_ASSERT_EQUAL(x, 0x1234U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0x0", "%x", &x), 1);
  CU_ASSERT_EQUAL(x, 0U);
}

static void test_sscanf_parses_number_for_uppercase_x(void)
{
  unsigned x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%X", &x), 1);
  CU_ASSERT_EQUAL(x, 0x1234U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-12aF", "%X", &x), 1);
  CU_ASSERT_EQUAL(x, (unsigned) (-0x12af));
  CU_ASSERT_EQUAL(retrolibc_sscanf("fA1299", "%X", &x), 1);
  CU_ASSERT_EQUAL(x, 0xfa1299U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+1234", "%X", &x), 1);
  CU_ASSERT_EQUAL(x, 0x1234U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%X", &x), 1);
  CU_ASSERT_EQUAL(x, 0U);
}

static void test_sscanf_parses_number_for_uppercase_x_and_prefix_0x(void)
{
  unsigned x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("0x1234", "%X", &x), 1);
  CU_ASSERT_EQUAL(x, 0x1234U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-0x12aF", "%X", &x), 1);
  CU_ASSERT_EQUAL(x, (unsigned) (-0x12af));
  CU_ASSERT_EQUAL(retrolibc_sscanf("0XfA1299", "%X", &x), 1);
  CU_ASSERT_EQUAL(x, 0xfa1299U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+0X1234", "%X", &x), 1);
  CU_ASSERT_EQUAL(x, 0x1234U);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0x0", "%X", &x), 1);
  CU_ASSERT_EQUAL(x, 0U);
}

static void test_sscanf_parses_number_for_d_and_spaces(void)
{
  int x;
  CU_ASSERT_EQUAL(retrolibc_sscanf(" \t 1234", "%d", &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(retrolibc_sscanf("\t\t-1234", "%d", &x), 1);
  CU_ASSERT_EQUAL(x, -1234);
}

static void test_sscanf_parses_number_for_d_and_width(void)
{
  int x, y;
  CU_ASSERT_EQUAL(retrolibc_sscanf("123499", "%3d%d", &x, &y), 2);
  CU_ASSERT_EQUAL(x, 123);
  CU_ASSERT_EQUAL(y, 499);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-123499", "%3d%d", &x, &y), 2);
  CU_ASSERT_EQUAL(x, -12);
  CU_ASSERT_EQUAL(y, 3499);
  CU_ASSERT_EQUAL(retrolibc_sscanf(" \t 123499", "%3d%d", &x, &y), 2);
  CU_ASSERT_EQUAL(x, 123);
  CU_ASSERT_EQUAL(y, 499);
  CU_ASSERT_EQUAL(retrolibc_sscanf("\t\t-123499", "%3d%d", &x, &y), 2);
  CU_ASSERT_EQUAL(x, -12);
  CU_ASSERT_EQUAL(y, 3499);
}

static void test_sscanf_parses_number_for_d_with_length_modifier(void)
{
  signed char hhx;
  short hx;
  int x;
  long lx;
  long long llx;
  CU_ASSERT_EQUAL(retrolibc_sscanf("127", "%hhd", &hhx), 1);
  CU_ASSERT_EQUAL(hhx, SCHAR_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-128", "%hhd", &hhx), 1);
  CU_ASSERT_EQUAL(hhx, SCHAR_MIN);
  CU_ASSERT_EQUAL(retrolibc_sscanf("32767", "%hd", &hx), 1);
  CU_ASSERT_EQUAL(hx, SHRT_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-32768", "%hd", &hx), 1);
  CU_ASSERT_EQUAL(hx, SHRT_MIN);
  CU_ASSERT_EQUAL(retrolibc_sscanf("2147483647", "%d", &x), 1);
  CU_ASSERT_EQUAL(x, INT_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-2147483648", "%d", &x), 1);
  CU_ASSERT_EQUAL(x, INT_MIN);
#if LONG_BIT == 64
  CU_ASSERT_EQUAL(retrolibc_sscanf("9223372036854775807", "%ld", &lx), 1);
  CU_ASSERT_EQUAL(lx, LONG_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-9223372036854775808", "%ld", &lx), 1);
  CU_ASSERT_EQUAL(lx, LONG_MIN);
#else
  CU_ASSERT_EQUAL(retrolibc_sscanf("2147483647", "%ld", &lx), 1);
  CU_ASSERT_EQUAL(lx, LONG_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-2147483648", "%ld", &lx), 1);
  CU_ASSERT_EQUAL(lx, LONG_MIN);
#endif
  CU_ASSERT_EQUAL(retrolibc_sscanf("9223372036854775807", "%lld", &llx), 1);
  CU_ASSERT_EQUAL(llx, LLONG_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-9223372036854775808", "%lld", &llx), 1);
  CU_ASSERT_EQUAL(llx, LLONG_MIN);
}

static void test_sscanf_parses_number_for_d_with_length_modifier_and_overflow(void)
{
  signed char hhx;
  short hx;
  int x;
  long lx;
  long long llx;
  CU_ASSERT_EQUAL(retrolibc_sscanf("128", "%hhd", &hhx), 1);
  CU_ASSERT_EQUAL(hhx, SCHAR_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-129", "%hhd", &hhx), 1);
  CU_ASSERT_EQUAL(hhx, SCHAR_MIN);
  CU_ASSERT_EQUAL(retrolibc_sscanf("32768", "%hd", &hx), 1);
  CU_ASSERT_EQUAL(hx, SHRT_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-32769", "%hd", &hx), 1);
  CU_ASSERT_EQUAL(hx, SHRT_MIN);
  CU_ASSERT_EQUAL(retrolibc_sscanf("2147483648", "%d", &x), 1);
  CU_ASSERT_EQUAL(x, INT_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-2147483649", "%d", &x), 1);
  CU_ASSERT_EQUAL(x, INT_MIN);
#if LONG_BIT == 64
  CU_ASSERT_EQUAL(retrolibc_sscanf("9223372036854775808", "%ld", &lx), 1);
  CU_ASSERT_EQUAL(lx, LONG_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-9223372036854775809", "%ld", &lx), 1);
  CU_ASSERT_EQUAL(lx, LONG_MIN);
#else
  CU_ASSERT_EQUAL(retrolibc_sscanf("2147483648", "%ld", &lx), 1);
  CU_ASSERT_EQUAL(lx, LONG_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-2147483649", "%ld", &lx), 1);
  CU_ASSERT_EQUAL(lx, LONG_MIN);
#endif
  CU_ASSERT_EQUAL(retrolibc_sscanf("9223372036854775808", "%lld", &llx), 1);
  CU_ASSERT_EQUAL(llx, LLONG_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-9223372036854775809", "%lld", &llx), 1);
  CU_ASSERT_EQUAL(llx, LLONG_MIN);
}

static void test_sscanf_parses_number_for_d_with_length_modifier_and_star(void)
{
  signed char hhx;
  short hx;
  int x;
  long lx;
  long long llx;
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 127", "%*hhd%hhd", &hhx), 1);
  CU_ASSERT_EQUAL(hhx, SCHAR_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 32767", "%*hd%hd", &hx), 1);
  CU_ASSERT_EQUAL(hx, SHRT_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 2147483647", "%*d%d", &x), 1);
  CU_ASSERT_EQUAL(x, INT_MAX);
#if LONG_BIT == 64
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 9223372036854775807", "%*ld%ld", &lx), 1);
  CU_ASSERT_EQUAL(lx, LONG_MAX);
#else
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 2147483647", "%*ld%ld", &lx), 1);
  CU_ASSERT_EQUAL(lx, LONG_MAX);
#endif
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 9223372036854775807", "%*lld%lld", &llx), 1);
  CU_ASSERT_EQUAL(llx, LLONG_MAX);
}

static void test_sscanf_parses_number_for_u_with_length_modifier(void)
{
  unsigned char hhx;
  unsigned short hx;
  unsigned x;
  unsigned long lx;
  unsigned long long llx;
  CU_ASSERT_EQUAL(retrolibc_sscanf("255", "%hhu", &hhx), 1);
  CU_ASSERT_EQUAL(hhx, UCHAR_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("65535", "%hu", &hx), 1);
  CU_ASSERT_EQUAL(hx, USHRT_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("4294967295", "%u", &x), 1);
  CU_ASSERT_EQUAL(x, UINT_MAX);
#if LONG_BIT == 64
  CU_ASSERT_EQUAL(retrolibc_sscanf("18446744073709551615", "%lu", &lx), 1);
  CU_ASSERT_EQUAL(lx, ULONG_MAX);
#else
  CU_ASSERT_EQUAL(retrolibc_sscanf("4294967295", "%lu", &lx), 1);
  CU_ASSERT_EQUAL(lx, ULONG_MAX);
#endif
  CU_ASSERT_EQUAL(retrolibc_sscanf("18446744073709551615", "%llu", &llx), 1);
  CU_ASSERT_EQUAL(llx, ULLONG_MAX);
}

static void test_sscanf_parses_number_for_u_with_length_modifier_and_overflow(void)
{
  unsigned char hhx;
  unsigned short hx;
  unsigned x;
  unsigned long lx;
  unsigned long long llx;
  CU_ASSERT_EQUAL(retrolibc_sscanf("256", "%hhu", &hhx), 1);
  CU_ASSERT_EQUAL(hhx, UCHAR_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("65536", "%hu", &hx), 1);
  CU_ASSERT_EQUAL(hx, USHRT_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("4294967296", "%u", &x), 1);
  CU_ASSERT_EQUAL(x, UINT_MAX);
#if LONG_BIT == 64
  CU_ASSERT_EQUAL(retrolibc_sscanf("18446744073709551616", "%lu", &lx), 1);
  CU_ASSERT_EQUAL(lx, ULONG_MAX);
#else
  CU_ASSERT_EQUAL(retrolibc_sscanf("4294967296", "%lu", &lx), 1);
  CU_ASSERT_EQUAL(lx, ULONG_MAX);
#endif
  CU_ASSERT_EQUAL(retrolibc_sscanf("18446744073709551616", "%llu", &llx), 1);
  CU_ASSERT_EQUAL(llx, ULLONG_MAX);
}

static void test_sscanf_parses_number_for_u_with_length_modifier_and_star(void)
{
  unsigned char hhx;
  unsigned short hx;
  unsigned x;
  unsigned long lx;
  unsigned long long llx;
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 255", "%*hhu%hhu", &hhx), 1);
  CU_ASSERT_EQUAL(hhx, UCHAR_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 65535", "%*hu%hu", &hx), 1);
  CU_ASSERT_EQUAL(hx, USHRT_MAX);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 4294967295", "%*u%u", &x), 1);
  CU_ASSERT_EQUAL(x, UINT_MAX);
#if LONG_BIT == 64
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 18446744073709551615", "%*lu%lu", &lx), 1);
  CU_ASSERT_EQUAL(lx, ULONG_MAX);
#else
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 4294967295", "%*lu%lu", &lx), 1);
  CU_ASSERT_EQUAL(lx, ULONG_MAX);
#endif
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 18446744073709551615", "%*llu%llu", &llx), 1);
  CU_ASSERT_EQUAL(llx, ULLONG_MAX);
}

static void test_sscanf_does_not_match_for_ix(void)
{
  int x;
  CU_ASSERT_EQUAL(retrolibc_sscanf(" \t ", "%i", &x), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-", "%i", &x), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0x", "%i", &x), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0X", "%i", &x), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0x", "%x", &x), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0X", "%x", &x), 0);
}

static void test_sscanf_parses_number_for_f(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, 1234.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-3456", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, -3456.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123499", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, 123499.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+1234", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, 1234.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_f_and_dot(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34", "%f", &x), 1);
  CU_ASSERT(fabs(x - 12.34) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56", "%f", &x), 1);
  CU_ASSERT(fabs(x - -34.56) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499", "%f", &x), 1);
  CU_ASSERT(fabs(x - 123.499) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+.123", "%f", &x), 1);
  CU_ASSERT(fabs(x - 0.123) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0.0", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_f_and_exponent(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34e9", "%f", &x), 1);
  CU_ASSERT(fabs(x - 12.34e9) <= 0.0001e9);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56e-12", "%f", &x), 1);
  CU_ASSERT(fabs(x - -34.56e-12) <= 0.0001e-12);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499e+23", "%f", &x), 1);
  CU_ASSERT(fabs(x - 123.499e23) <= 0.0001e23);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0e500", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34E9", "%f", &x), 1);
  CU_ASSERT(fabs(x - 12.34e9) <= 0.0001e9);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56E-12", "%f", &x), 1);
  CU_ASSERT(fabs(x - -34.56e-12) <= 0.0001e-12);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499E+23", "%f", &x), 1);
  CU_ASSERT(fabs(x - 123.499e23) <= 0.0001e23);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0E500", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_e(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%e", &x), 1);
  CU_ASSERT_EQUAL(x, 1234.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-3456", "%e", &x), 1);
  CU_ASSERT_EQUAL(x, -3456.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123499", "%e", &x), 1);
  CU_ASSERT_EQUAL(x, 123499.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+1234", "%e", &x), 1);
  CU_ASSERT_EQUAL(x, 1234.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%e", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_e_and_dot(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34", "%e", &x), 1);
  CU_ASSERT(fabs(x - 12.34) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56", "%e", &x), 1);
  CU_ASSERT(fabs(x - -34.56) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499", "%e", &x), 1);
  CU_ASSERT(fabs(x - 123.499) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+.123", "%e", &x), 1);
  CU_ASSERT(fabs(x - 0.123) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0.0", "%e", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_e_and_exponent(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34e9", "%e", &x), 1);
  CU_ASSERT(fabs(x - 12.34e9) <= 0.0001e9);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56e-12", "%e", &x), 1);
  CU_ASSERT(fabs(x - -34.56e-12) <= 0.0001e-12);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499e+23", "%e", &x), 1);
  CU_ASSERT(fabs(x - 123.499e23) <= 0.0001e23);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0e500", "%e", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34E9", "%e", &x), 1);
  CU_ASSERT(fabs(x - 12.34e9) <= 0.0001e9);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56E-12", "%e", &x), 1);
  CU_ASSERT(fabs(x - -34.56e-12) <= 0.0001e-12);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499E+23", "%e", &x), 1);
  CU_ASSERT(fabs(x - 123.499e23) <= 0.0001e23);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0E500", "%e", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_g(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%g", &x), 1);
  CU_ASSERT_EQUAL(x, 1234.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-3456", "%g", &x), 1);
  CU_ASSERT_EQUAL(x, -3456.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123499", "%g", &x), 1);
  CU_ASSERT_EQUAL(x, 123499.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+1234", "%g", &x), 1);
  CU_ASSERT_EQUAL(x, 1234.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%g", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_g_and_dot(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34", "%g", &x), 1);
  CU_ASSERT(fabs(x - 12.34) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56", "%g", &x), 1);
  CU_ASSERT(fabs(x - -34.56) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499", "%g", &x), 1);
  CU_ASSERT(fabs(x - 123.499) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+.123", "%g", &x), 1);
  CU_ASSERT(fabs(x - 0.123) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0.0", "%g", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_g_and_exponent(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34e9", "%g", &x), 1);
  CU_ASSERT(fabs(x - 12.34e9) <= 0.0001e9);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56e-12", "%g", &x), 1);
  CU_ASSERT(fabs(x - -34.56e-12) <= 0.0001e-12);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499e+23", "%g", &x), 1);
  CU_ASSERT(fabs(x - 123.499e23) <= 0.0001e23);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0e500", "%g", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34E9", "%g", &x), 1);
  CU_ASSERT(fabs(x - 12.34e9) <= 0.0001e9);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56E-12", "%g", &x), 1);
  CU_ASSERT(fabs(x - -34.56e-12) <= 0.0001e-12);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499E+23", "%g", &x), 1);
  CU_ASSERT(fabs(x - 123.499e23) <= 0.0001e23);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0E500", "%g", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_uppercase_e(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%E", &x), 1);
  CU_ASSERT_EQUAL(x, 1234.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-3456", "%E", &x), 1);
  CU_ASSERT_EQUAL(x, -3456.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123499", "%E", &x), 1);
  CU_ASSERT_EQUAL(x, 123499.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+1234", "%E", &x), 1);
  CU_ASSERT_EQUAL(x, 1234.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%E", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_uppercase_e_and_dot(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34", "%E", &x), 1);
  CU_ASSERT(fabs(x - 12.34) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56", "%E", &x), 1);
  CU_ASSERT(fabs(x - -34.56) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499", "%E", &x), 1);
  CU_ASSERT(fabs(x - 123.499) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+.123", "%E", &x), 1);
  CU_ASSERT(fabs(x - 0.123) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0.0", "%E", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_uppercase_e_and_exponent(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34e9", "%E", &x), 1);
  CU_ASSERT(fabs(x - 12.34e9) <= 0.0001e9);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56e-12", "%E", &x), 1);
  CU_ASSERT(fabs(x - -34.56e-12) <= 0.0001e-12);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499e+23", "%E", &x), 1);
  CU_ASSERT(fabs(x - 123.499e23) <= 0.0001e23);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0e500", "%E", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34E9", "%E", &x), 1);
  CU_ASSERT(fabs(x - 12.34e9) <= 0.0001e9);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56E-12", "%E", &x), 1);
  CU_ASSERT(fabs(x - -34.56e-12) <= 0.0001e-12);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499E+23", "%E", &x), 1);
  CU_ASSERT(fabs(x - 123.499e23) <= 0.0001e23);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0E500", "%E", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_uppercase_g(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%G", &x), 1);
  CU_ASSERT_EQUAL(x, 1234.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-3456", "%G", &x), 1);
  CU_ASSERT_EQUAL(x, -3456.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123499", "%G", &x), 1);
  CU_ASSERT_EQUAL(x, 123499.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+1234", "%G", &x), 1);
  CU_ASSERT_EQUAL(x, 1234.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%G", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_uppercase_g_and_dot(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34", "%G", &x), 1);
  CU_ASSERT(fabs(x - 12.34) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56", "%G", &x), 1);
  CU_ASSERT(fabs(x - -34.56) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499", "%G", &x), 1);
  CU_ASSERT(fabs(x - 123.499) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+.123", "%G", &x), 1);
  CU_ASSERT(fabs(x - 0.123) <= 0.0001);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0.0", "%G", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_uppercase_g_and_exponent(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34e9", "%G", &x), 1);
  CU_ASSERT(fabs(x - 12.34e9) <= 0.0001e9);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56e-12", "%G", &x), 1);
  CU_ASSERT(fabs(x - -34.56e-12) <= 0.0001e-12);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499e+23", "%G", &x), 1);
  CU_ASSERT(fabs(x - 123.499e23) <= 0.0001e23);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0e500", "%G", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("12.34E9", "%G", &x), 1);
  CU_ASSERT(fabs(x - 12.34e9) <= 0.0001e9);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-34.56E-12", "%G", &x), 1);
  CU_ASSERT(fabs(x - -34.56e-12) <= 0.0001e-12);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123.499E+23", "%G", &x), 1);
  CU_ASSERT(fabs(x - 123.499e23) <= 0.0001e23);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0E500", "%G", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_f_and_spaces(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf(" \t 1234", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, 1234.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("\t\t-1234", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, -1234.0f);
}

static void test_sscanf_parses_number_for_f_and_width(void)
{
  float x, y;
  CU_ASSERT_EQUAL(retrolibc_sscanf("123499", "%3f%f", &x, &y), 2);
  CU_ASSERT_EQUAL(x, 123.0f);
  CU_ASSERT_EQUAL(y, 499.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-123499", "%3f%f", &x, &y), 2);
  CU_ASSERT_EQUAL(x, -12.0f);
  CU_ASSERT_EQUAL(y, 3499.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf(" \t 123499", "%3f%f", &x, &y), 2);
  CU_ASSERT_EQUAL(x, 123.0f);
  CU_ASSERT_EQUAL(y, 499.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("\t\t-123499", "%3f%f", &x, &y), 2);
  CU_ASSERT_EQUAL(x, -12.0f);
  CU_ASSERT_EQUAL(y, 3499.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123e12", "%5f%f", &x, &y), 2);
  CU_ASSERT(fabs(x - 123.0e1) <= 0.0001e1);
  CU_ASSERT_EQUAL(y, 2.0f);
}

static void test_sscanf_parses_number_for_f_and_overflow(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1.234E500", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, HUGE_VAL);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-1.234E500", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, -HUGE_VAL);
}

static void test_sscanf_parses_number_for_f_and_underflow(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1.234E-500", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
}

static void test_sscanf_parses_number_for_f_with_length_modifier(void)
{
  float x;
  double lx;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, 1234.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-3456", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, -3456.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123499", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, 123499.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%f", &x), 1);
  CU_ASSERT_EQUAL(x, 0.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%lf", &lx), 1);
  CU_ASSERT_EQUAL(lx, 1234.0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-3456", "%lf", &lx), 1);
  CU_ASSERT_EQUAL(lx, -3456.0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123499", "%lf", &lx), 1);
  CU_ASSERT_EQUAL(lx, 123499.0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%lf", &lx), 1);
  CU_ASSERT_EQUAL(lx, 0.0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234", "%llf", &lx), 1);
  CU_ASSERT_EQUAL(lx, 1234.0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-3456", "%llf", &lx), 1);
  CU_ASSERT_EQUAL(lx, -3456.0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("123499", "%llf", &lx), 1);
  CU_ASSERT_EQUAL(lx, 123499.0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0", "%llf", &lx), 1);
  CU_ASSERT_EQUAL(lx, 0.0);
}

static void test_sscanf_parses_number_for_f_with_length_modifier_and_star(void)
{
  float x;
  double lx;
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 1234", "%*f%f", &x), 1);
  CU_ASSERT_EQUAL(x, 1234.0f);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 1234", "%*lf%lf", &lx), 1);
  CU_ASSERT_EQUAL(lx, 1234.0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 1234", "%*llf%llf", &lx), 1);
  CU_ASSERT_EQUAL(lx, 1234.0);
}

static void test_sscanf_does_not_match_for_f(void)
{
  float x;
  CU_ASSERT_EQUAL(retrolibc_sscanf(" \t ", "%f", &x), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("-", "%f", &x), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("+", "%f", &x), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234e", "%f", &x), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234E+", "%f", &x), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234e-", "%f", &x), 0);
}

static void test_sscanf_parses_string_for_s(void)
{
  static char s[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf("abcdef", "%s", s), 1);
  CU_ASSERT_STRING_EQUAL(s, "abcdef");
}

static void test_sscanf_parses_string_for_s_and_spaces(void)
{
  static char s[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf(" \t abcdef", "%s", s), 1);
  CU_ASSERT_STRING_EQUAL(s, "abcdef");
}

static void test_sscanf_parses_string_for_s_with_width(void)
{
  static char s[100];
  static char s2[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf("abcdef", "%10s%s", s, s2), 1);
  CU_ASSERT_STRING_EQUAL(s, "abcdef");
  CU_ASSERT_EQUAL(retrolibc_sscanf("abcdef", "%4s%s", s, s2), 2);
  CU_ASSERT_STRING_EQUAL(s, "abcd");
  CU_ASSERT_STRING_EQUAL(s2, "ef");
  CU_ASSERT_EQUAL(retrolibc_sscanf(" \t abcdef", "%10s%s", s, s2), 1);
  CU_ASSERT_STRING_EQUAL(s, "abcdef");
  CU_ASSERT_EQUAL(retrolibc_sscanf(" \t abcdef", "%4s%s", s, s2), 2);
  CU_ASSERT_STRING_EQUAL(s, "abcd");
  CU_ASSERT_STRING_EQUAL(s2, "ef");
}

static void test_sscanf_parses_string_for_s_with_star(void)
{
  static char s[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf("abcdef ABCDEF", "%*s%s", s), 1);
  CU_ASSERT_STRING_EQUAL(s, "ABCDEF");
}

static void test_sscanf_does_not_match_for_s(void)
{
  static char s[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf("", "%s", s), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf(" \t ", "%s", s), 0);
}

static void test_sscanf_parses_string_for_bracket(void)
{
  static char s[100];
  static char s2[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf("abcdef", "%[abc]%s", s, s2), 2);
  CU_ASSERT_STRING_EQUAL(s, "abc");
  CU_ASSERT_STRING_EQUAL(s2, "def");
}

static void test_sscanf_parses_string_for_bracket_caret(void)
{
  static char s[100];
  static char s2[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf("defabc", "%[^abc]%s", s, s2), 2);
  CU_ASSERT_STRING_EQUAL(s, "def");
  CU_ASSERT_STRING_EQUAL(s2, "abc");
}

static void test_sscanf_parses_string_for_bracket_with_range(void)
{
  static char s[100];
  static char s2[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf("abcdef1ABCDEF", "%[a-z1]%s", s, s2), 2);
  CU_ASSERT_STRING_EQUAL(s, "abcdef1");
  CU_ASSERT_STRING_EQUAL(s2, "ABCDEF");
}

static void test_sscanf_parses_string_for_bracket_caret_with_range(void)
{
  static char s[100];
  static char s2[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf("ABCDEFz1", "%[^a-z1]%s", s, s2), 2);
  CU_ASSERT_STRING_EQUAL(s, "ABCDEF");
  CU_ASSERT_STRING_EQUAL(s2, "z1");
}

static void test_sscanf_parses_string_for_bracket_with_width(void)
{
  static char s[100];
  static char s2[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf("abcdef", "%10[abcdef]%s", s, s2), 1);
  CU_ASSERT_STRING_EQUAL(s, "abcdef");
  CU_ASSERT_EQUAL(retrolibc_sscanf("abcdef", "%4[abcdef]%s", s, s2), 2);
  CU_ASSERT_STRING_EQUAL(s, "abcd");
  CU_ASSERT_STRING_EQUAL(s2, "ef");
}

static void test_sscanf_parses_string_for_bracket_with_star(void)
{
  static char s[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf("abcDEF", "%*[abc]%[A-Z]", s), 1);
  CU_ASSERT_STRING_EQUAL(s, "DEF");
}

static void test_sscanf_does_not_match_for_bracket(void)
{
  static char s[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf("", "%[abc]", s), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf(" \t ", "%[abc]", s), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("ABC", "%[abc]", s), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf(" \t abc", "%[abc]", s), 0);
}

static void test_sscanf_parses_character_for_c(void)
{
  char c;
  CU_ASSERT_EQUAL(retrolibc_sscanf("a", "%c", &c), 1);
  CU_ASSERT_EQUAL(c, 'a');
  CU_ASSERT_EQUAL(retrolibc_sscanf("b", "%c", &c), 1);
  CU_ASSERT_EQUAL(c, 'b');
  CU_ASSERT_EQUAL(retrolibc_sscanf("c", "%c", &c), 1);
  CU_ASSERT_EQUAL(c, 'c');
  CU_ASSERT_EQUAL(retrolibc_sscanf(" ", "%c", &c), 1);
  CU_ASSERT_EQUAL(c, ' ');
  CU_ASSERT_EQUAL(retrolibc_sscanf("\t", "%c", &c), 1);
  CU_ASSERT_EQUAL(c, '\t');
}

static void test_sscanf_parses_characters_for_c_with_width(void)
{
  static char c[6];
  CU_ASSERT_EQUAL(retrolibc_sscanf("abcdef", "%6c", c), 1);
  CU_ASSERT_EQUAL(c[0], 'a');
  CU_ASSERT_EQUAL(c[1], 'b');
  CU_ASSERT_EQUAL(c[2], 'c');
  CU_ASSERT_EQUAL(c[3], 'd');
  CU_ASSERT_EQUAL(c[4], 'e');
  CU_ASSERT_EQUAL(c[5], 'f');
}

static void test_sscanf_parses_character_for_c_with_star(void)
{
  char c;
  CU_ASSERT_EQUAL(retrolibc_sscanf("ab", "%*c%c", &c), 1);
  CU_ASSERT_EQUAL(c, 'b');
}

static void test_sscanf_does_not_match_for_c(void)
{
  char c;
  CU_ASSERT_EQUAL(retrolibc_sscanf("", "%c", &c), 0);
}

static void test_sscanf_parses_pointer_for_p(void)
{
  void *p;
#if LONG_BIT == 64
  CU_ASSERT_EQUAL(retrolibc_sscanf("0x0000000000000000", "%p", &p), 1);
  CU_ASSERT_PTR_NULL(p);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0x1234567890abcdef", "%p", &p), 1);
  CU_ASSERT_PTR_EQUAL(p, (void *) 0x1234567890abcdefUL);
#else
  CU_ASSERT_EQUAL(retrolibc_sscanf("0x00000000", "%p", &p), 1);
  CU_ASSERT_PTR_NULL(p);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0x12345678", "%p", &p), 1);
  CU_ASSERT_PTR_EQUAL(p, (void *) 0x12345678U);
#endif
}

static void test_sscanf_parses_pointer_for_p_with_star(void)
{
  void *p;
#if LONG_BIT == 64
  CU_ASSERT_EQUAL(retrolibc_sscanf("1 0x0000000000000000", "%*p%p", &p), 1);
  CU_ASSERT_PTR_NULL(p);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 0x1234567890abcdef", "%*p%p", &p), 1);
  CU_ASSERT_PTR_EQUAL(p, (void *) 0x1234567890abcdefUL);
#else
  CU_ASSERT_EQUAL(retrolibc_sscanf("1 0x00000000", "%*p%p", &p), 1);
  CU_ASSERT_PTR_NULL(p);
  CU_ASSERT_EQUAL(retrolibc_sscanf("0 0x12345678", "%*p%p", &p), 1);
  CU_ASSERT_PTR_EQUAL(p, (void *) 0x12345678U);
#endif
}

static void test_sscanf_stores_number_of_readed_bytes_to_variable(void)
{
  int x;
  static char s[100];
  int n1, n2, n3;
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234 abc", "%n%d%n%s%n", &n1, &x, &n2, s, &n3), 2);
  CU_ASSERT_EQUAL(n1, 0);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(n2, 4);
  CU_ASSERT_STRING_EQUAL(s, "abc");
  CU_ASSERT_EQUAL(n3, 8);
}

static void test_sscanf_does_not_stores_number_of_readed_bytes_to_variable_for_star(void)
{
  int x;
  static char s[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234 abc", "%*n%d%*n%s%*n", &x, s), 2);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_STRING_EQUAL(s, "abc");
}

static void test_sscanf_parses_percent_for_percent(void)
{
  int x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("%1234", "%%%d", &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(retrolibc_sscanf("$3456", "%%%d", &x), 0);
}

static void test_sscanf_parses_invalid_conversion_specificator(void)
{
  int x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("%k1234", "%k%d", &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(retrolibc_sscanf("xx1234", "%k%d", &x), 0);
}

static void test_sscanf_parses_values_without_space_separator(void)
{
  int x;
  static char s[100];
  float f;
  static char s2[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234abc", "%d%s", &x, s), 2);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_STRING_EQUAL(s, "abc");
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234def", "%f%s", &f, s), 2);
  CU_ASSERT_EQUAL(f, 1234.0f);
  CU_ASSERT_STRING_EQUAL(s, "def");
  CU_ASSERT_EQUAL(retrolibc_sscanf("1234e10ghi", "%f%s", &f, s), 2);
  CU_ASSERT(fabs(f - 1234e10) <= 0.0001e10);
  CU_ASSERT_STRING_EQUAL(s, "ghi");
  CU_ASSERT_EQUAL(retrolibc_sscanf("abcdef", "%[abc]%s", s, s2), 2);
  CU_ASSERT_STRING_EQUAL(s, "abc");
  CU_ASSERT_STRING_EQUAL(s2, "def");
}

static void test_sscanf_parses_character(void)
{
  int x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("a1234", "a%d", &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(retrolibc_sscanf("b3456", "b%d", &x), 1);
  CU_ASSERT_EQUAL(x, 3456);
  CU_ASSERT_EQUAL(retrolibc_sscanf("c123499", "c%d", &x), 1);
  CU_ASSERT_EQUAL(x, 123499);
}

static void test_sscanf_stops_for_invalid_character(void)
{
  int x;
  CU_ASSERT_EQUAL(retrolibc_sscanf("d1234", "a%d", &x), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("e3456", "b%d", &x), 0);
  CU_ASSERT_EQUAL(retrolibc_sscanf("f123499", "c%d", &x), 0);
}

static void test_sscanf_parses_spaces(void)
{
  static char s[100];
  CU_ASSERT_EQUAL(retrolibc_sscanf(" \t abc", " %[a-z]", s), 1);
  CU_ASSERT_STRING_EQUAL(s, "abc");
  CU_ASSERT_EQUAL(retrolibc_sscanf(" def", "\t\n%[a-z]", s), 1);
  CU_ASSERT_STRING_EQUAL(s, "def");
  CU_ASSERT_EQUAL(retrolibc_sscanf("ghi", " %[a-z]", s), 1);
  CU_ASSERT_STRING_EQUAL(s, "ghi");
  CU_ASSERT_EQUAL(retrolibc_sscanf("\n\njkl", " %[a-z]", s), 1);
  CU_ASSERT_STRING_EQUAL(s, "jkl");
}

static void test_fscanf_reads_values(void)
{
  FILE *f;
  int x;
  static char s[100];
  float y;
  clean_current_dir();
  CU_ASSERT_FATAL(write_to_file("test.txt", "1234 abc 34.56"));
  f = fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_fscanf(f, "%d%s%f", &x, s, &y), 3);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_STRING_EQUAL(s, "abc");
  CU_ASSERT(fabs(y - 34.56) <= 0.0001);
  fclose(f);
}

static void test_fscanf_reads_value_2_times(void)
{
  FILE *f;
  int x;
  static char s[100];
  clean_current_dir();
  CU_ASSERT_FATAL(write_to_file("test.txt", "1234abc"));
  f = fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_fscanf(f, "%d", &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(retrolibc_fscanf(f, "%s", s), 1);
  CU_ASSERT_STRING_EQUAL(s, "abc");
  fclose(f);
}

static void test_fscanf_reads_value_2_times_with_uppercase_x(void)
{
  FILE *f;
  int x;
  static char s[100];
  clean_current_dir();
  CU_ASSERT_FATAL(write_to_file("test.txt", "1234Xabc"));
  f = fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_fscanf(f, "%dx", &x), 1);
  CU_ASSERT_EQUAL(x, 1234);
  CU_ASSERT_EQUAL(retrolibc_fscanf(f, "%s", s), 1);
  CU_ASSERT_STRING_EQUAL(s, "Xabc");
  fclose(f);
}

static void test_fscanf_returns_eof(void)
{
  FILE *f;
  int x;
  static char s[100];
  float y;
  clean_current_dir();
  CU_ASSERT_FATAL(write_to_file("test.txt", ""));
  f = fopen("test.txt", "r");
  CU_ASSERT_EQUAL(retrolibc_fscanf(f, "%d%s%f", &x, s, &y), EOF);
  fclose(f);
}

int add_fscanf_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("fscanf suite", initialize_fscanf, cleanup_fscanf)) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses values", test_sscanf_parses_values) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for d", test_sscanf_parses_number_for_d) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for i", test_sscanf_parses_number_for_i) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for i and prefix 0", test_sscanf_parses_number_for_i_and_prefix_0) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for i and prefix 0x", test_sscanf_parses_number_for_i_and_prefix_0x) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for o", test_sscanf_parses_number_for_o) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for u", test_sscanf_parses_number_for_u) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for x", test_sscanf_parses_number_for_x) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for x and prefix 0x", test_sscanf_parses_number_for_x_and_prefix_0x) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for X", test_sscanf_parses_number_for_uppercase_x) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for X and prefix 0x", test_sscanf_parses_number_for_uppercase_x_and_prefix_0x) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for d and spaces", test_sscanf_parses_number_for_d_and_spaces) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for d and width", test_sscanf_parses_number_for_d_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for d with length modifier", test_sscanf_parses_number_for_d_with_length_modifier) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for d with length modifier and overflow", test_sscanf_parses_number_for_d_with_length_modifier_and_overflow) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for d with length modifier and *", test_sscanf_parses_number_for_d_with_length_modifier_and_star) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for u with length modifier", test_sscanf_parses_number_for_u_with_length_modifier) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for u with length modifier and overflow", test_sscanf_parses_number_for_u_with_length_modifier_and_overflow) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for u with length modifier and *", test_sscanf_parses_number_for_u_with_length_modifier_and_star) == NULL) return -1;
  if(CU_add_test(suite, "sscanf doesn't match for ix", test_sscanf_does_not_match_for_ix) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for f", test_sscanf_parses_number_for_f) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for f and dot", test_sscanf_parses_number_for_f_and_dot) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for f and exponent", test_sscanf_parses_number_for_f_and_exponent) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for e", test_sscanf_parses_number_for_e) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for e and dot", test_sscanf_parses_number_for_e_and_dot) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for e and exponent", test_sscanf_parses_number_for_e_and_exponent) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for g", test_sscanf_parses_number_for_g) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for g and dot", test_sscanf_parses_number_for_g_and_dot) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for g and exponent", test_sscanf_parses_number_for_g_and_exponent) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for E", test_sscanf_parses_number_for_uppercase_e) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for E and dot", test_sscanf_parses_number_for_uppercase_e_and_dot) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for E and exponent", test_sscanf_parses_number_for_uppercase_e_and_exponent) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for G", test_sscanf_parses_number_for_uppercase_g) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for G and dot", test_sscanf_parses_number_for_uppercase_g_and_dot) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for G and exponent", test_sscanf_parses_number_for_uppercase_g_and_exponent) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for f and spaces", test_sscanf_parses_number_for_f_and_spaces) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for f and width", test_sscanf_parses_number_for_f_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for f and spaces", test_sscanf_parses_number_for_f_and_spaces) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for f and width", test_sscanf_parses_number_for_f_and_width) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for f and overflow", test_sscanf_parses_number_for_f_and_overflow) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for f and underflow", test_sscanf_parses_number_for_f_and_underflow) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for f and length modifier", test_sscanf_parses_number_for_f_with_length_modifier) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses number for f and length modifier and *", test_sscanf_parses_number_for_f_with_length_modifier_and_star) == NULL) return -1;
  if(CU_add_test(suite, "sscanf doesn't match for f", test_sscanf_does_not_match_for_f) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses string for s", test_sscanf_parses_string_for_s) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses string for s and spaces", test_sscanf_parses_string_for_s_and_spaces) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses string for s with width", test_sscanf_parses_string_for_s_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses string for s with *", test_sscanf_parses_string_for_s_with_star) == NULL) return -1;
  if(CU_add_test(suite, "sscanf doesn't match for s", test_sscanf_does_not_match_for_s) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses string for []", test_sscanf_parses_string_for_bracket) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses string for [^]", test_sscanf_parses_string_for_bracket_caret) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses string for [] with range", test_sscanf_parses_string_for_bracket_with_range) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses string for [^] with range", test_sscanf_parses_string_for_bracket_caret_with_range) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses string for [] with width", test_sscanf_parses_string_for_bracket_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses string for [] with *", test_sscanf_parses_string_for_bracket_with_star) == NULL) return -1;
  if(CU_add_test(suite, "sscanf doesn't match for []", test_sscanf_does_not_match_for_bracket) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses character for c", test_sscanf_parses_character_for_c) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses characters for c with width", test_sscanf_parses_characters_for_c_with_width) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses character for c with star", test_sscanf_parses_character_for_c_with_star) == NULL) return -1;
  if(CU_add_test(suite, "sscanf doesn't match for c", test_sscanf_does_not_match_for_c) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses pointer for p", test_sscanf_parses_pointer_for_p) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses pointer for p with *", test_sscanf_parses_pointer_for_p_with_star) == NULL) return -1;
  if(CU_add_test(suite, "sscanf stores number of readed bytes to variable", test_sscanf_stores_number_of_readed_bytes_to_variable) == NULL) return -1;
  if(CU_add_test(suite, "sscanf doesn't store number of readed bytes to variable for *", test_sscanf_does_not_stores_number_of_readed_bytes_to_variable_for_star) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses % for %", test_sscanf_parses_percent_for_percent) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses invalid conversion specificator", test_sscanf_parses_invalid_conversion_specificator) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses values without space separator", test_sscanf_parses_values_without_space_separator) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses character", test_sscanf_parses_character) == NULL) return -1;
  if(CU_add_test(suite, "sscanf stops for invalid character", test_sscanf_stops_for_invalid_character) == NULL) return -1;
  if(CU_add_test(suite, "sscanf parses spaces", test_sscanf_parses_spaces) == NULL) return -1;
  if(CU_add_test(suite, "fscanf reads values", test_fscanf_reads_values) == NULL) return -1;
  if(CU_add_test(suite, "fscanf reads value 2 times", test_fscanf_reads_value_2_times) == NULL) return -1;
  if(CU_add_test(suite, "fscanf reads value 2 times with X", test_fscanf_reads_value_2_times_with_uppercase_x) == NULL) return -1;
  if(CU_add_test(suite, "fscanf returns EOF", test_fscanf_returns_eof) == NULL) return -1;
  return 0;
}

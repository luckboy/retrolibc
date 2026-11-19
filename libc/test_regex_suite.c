/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <ctype.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "retrolibc.h"
#include "priv_regex.h"

static int initialize_regex(void)
{ return 0; }

static int cleanup_regex(void)
{ return 0; }

static int eq_op(retrolibc_regex_t *reg, ssize_t *ip, int op)
{ 
  int r;
  if(reg->__bytecode == NULL) return 0;
  r = (reg->__bytecode[*ip] == op);
  (*ip)++;
  return r;
}

static int eq_op_with_char(retrolibc_regex_t *reg, ssize_t *ip, int op, int c)
{ 
  int r;
  if(reg->__bytecode == NULL) return 0;
  r = (reg->__bytecode[*ip] == op);
  r &= (reg->__bytecode[*ip + 1] == (c & 0xff));
  (*ip) += 2;
  return r;
}

static int eq_op_with_int(retrolibc_regex_t *reg, ssize_t *ip, int op, int x)
{ 
  int r;
  if(reg->__bytecode == NULL) return 0;
  r = (reg->__bytecode[*ip] == op);
  r &= ((reg->__bytecode[*ip + 1]) == ((x >> 24) & 0xff));
  r &= ((reg->__bytecode[*ip + 2]) == ((x >> 16) & 0xff));
  r &= (reg->__bytecode[*ip + 3] == ((x >> 8) & 0xff));
  r &= (reg->__bytecode[*ip + 4] == (x & 0xff));
  (*ip) += 5;
  return r;
}

static int eq_op_with_range(retrolibc_regex_t *reg, ssize_t *ip, int op, int c, int c2)
{ 
  int r; 
  if(reg->__bytecode == NULL) return 0;
  r = (reg->__bytecode[*ip] == op);
  r &= (reg->__bytecode[*ip + 1] == (c & 0xff));
  r &= (reg->__bytecode[*ip + 2] == (c2 & 0xff));
  (*ip) += 3;
  return r;
}

static void test_regcomp_compiles_regular_expression_for_bre(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "abc", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_anchors(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "^abc$", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_SANCHOR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_EANCHOR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_concatenation(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\(abc\\)\\(def\\)", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 2);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 0), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'd'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'e'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'f'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_star(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab*c", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B, 6), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B, -6), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_3_duplication(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab\\{3\\}c", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_3_comma_duplication(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab\\{3,\\}c", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_2_3_duplication(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab\\{2,3\\}c", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B_JUMP, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_1_3_duplication(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab\\{1,3\\}c", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B_JUMP, 7), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B_JUMP, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_1_duplication(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab\\{1\\}c", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_0_duplication(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab\\{0\\}c", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_JUMP, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_subexpression(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\(abc\\)", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_nested_subexpression(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\(abc\\(def\\)ghi\\)", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 2);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'd'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'e'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'f'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'g'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'h'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'i'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_empty_subexpression(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\(\\)", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 0), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_back_references(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\(abc\\)\\(def\\)\\1\\2", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 2);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 0), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'd'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'e'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'f'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 1), 1);
  CU_ASSERT_EQUAL(eq_op_with_char(&reg, &ip, RE_OP_BACK_REF, 0), 1);
  CU_ASSERT_EQUAL(eq_op_with_char(&reg, &ip, RE_OP_BACK_REF, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_back_references_with_nested_subexpression(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\(abc\\(def\\)ghi\\2\\)\\1", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 2);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'd'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'e'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'f'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'g'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'h'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'i'), 1);
  CU_ASSERT_EQUAL(eq_op_with_char(&reg, &ip, RE_OP_BACK_REF, 1), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 0), 1);
  CU_ASSERT_EQUAL(eq_op_with_char(&reg, &ip, RE_OP_BACK_REF, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_brackets(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[abc]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_brackets_with_caret(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[^abc]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_NOT), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_brackets_with_ctype(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[a[:alpha:]c]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_ALPHA), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_brackets_with_range(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[ab-cd]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_range(&reg, &ip, RE_OP_RANGE, 'b', 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'd'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_brackets_with_left_bracket(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[ab[c]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, '['), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_escape(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab\\.c", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, '.'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_dot(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab.c", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_ANY), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_0234(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab\234c", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_char(&reg, &ip, RE_OP_MATCH, '\234'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_alnum(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:alnum:]]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_ALNUM), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_alpha(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:alpha:]]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_ALPHA), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_blank(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:blank:]]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BLANK), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_cntrl(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:cntrl:]]", 0), 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_CNTRL), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_digit(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:digit:]]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_DIGIT), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_graph(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:graph:]]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_GRAPH), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_lower(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:lower:]]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_LOWER), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_print(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:print:]]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_PRINT), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_punct(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:punct:]]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_PUNCT), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_space(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:space:]]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_SPACE), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_upper(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:upper:]]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_UPPER), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_xdigit(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:xdigit:]]", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_XDIGIT), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_bre_and_subexpression_and_star(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\(abc\\)*", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B, 18), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 0), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B, -18), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_empty_regular_expression_for_bre(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "", 0), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "abc", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_alternation(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "abc|def|ghi", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B, 8), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_JUMP, 21), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B, 8), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'd'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'e'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'f'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_JUMP, 8), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 3), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'g'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'h'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'i'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_anchors(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "^abc$", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_SANCHOR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_EANCHOR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_concatenation(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "(abc)(def)", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 2);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 0), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'd'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'e'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'f'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_star(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab*c", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B, 6), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B, -6), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_plus(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab+c", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B, -6), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_question(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab?c", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_3_duplication(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab{3}c", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_3_comma_duplication(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab{3,}c", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_2_3_duplication(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab{2,3}c", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B_JUMP, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_1_3_duplication(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab{1,3}c", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B_JUMP, 7), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B_JUMP, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_1_duplication(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab{1}c", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_NOP, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_0_duplication(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab{0}c", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_JUMP, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_subexpression(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "(abc)", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_nested_subexpression(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "(abc(def)ghi)", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 2);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'd'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'e'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'f'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 1), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'g'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'h'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'i'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_empty_subexpression(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "()", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 0), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_brackets(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[abc]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_brackets_with_caret(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[^abc]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_NOT), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_brackets_with_ctype(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[a[:alpha:]c]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_ALPHA), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_brackets_with_range(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[ab-cd]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op_with_range(&reg, &ip, RE_OP_RANGE, 'b', 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'd'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_brackets_with_left_bracket(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[ab[c]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, '['), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_escape(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab\\.c", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, '.'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_dot(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab.c", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_ANY), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_0234(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "ab\234c", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_char(&reg, &ip, RE_OP_MATCH, '\234'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_alnum(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:alnum:]]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_ALNUM), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_alpha(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:alpha:]]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_ALPHA), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_blank(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:blank:]]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BLANK), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_cntrl(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:cntrl:]]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_CNTRL), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_digit(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:digit:]]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_DIGIT), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_graph(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:graph:]]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_GRAPH), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_lower(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:lower:]]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_LOWER), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_print(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:print:]]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_PRINT), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_punct(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:punct:]]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_PUNCT), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_space(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:space:]]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_SPACE), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_upper(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:upper:]]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_UPPER), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_xdigit(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:xdigit:]]", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_XDIGIT), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_BR_END), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_regular_expression_for_ere_and_subexpression_and_star(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "(abc)*", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B, 18), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_SO, 0), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'a'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'c'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_SAVE_EO, 0), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_B, -18), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_compiles_empty_regular_expression_for_ere(void)
{
  retrolibc_regex_t reg;
  ssize_t ip = 0;
  reg.__bytecode = NULL;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(reg.re_nsub, 0);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, RE_OP_END), 1);
  if(reg.__bytecode != NULL) free(reg.__bytecode);
}

static void test_regcomp_complains_on_invalid_regular_expression_for_bre(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "a^bc", 0), RETROLIBC_REG_BADPAT);
}

static void test_regcomp_complains_on_invalid_character_class_name_for_bre(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:xxx:]]", 0), RETROLIBC_REG_ECTYPE);
}

static void test_regcomp_complains_on_trailing_backslash_for_bre(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\", 0), RETROLIBC_REG_EESCAPE);
}

static void test_regcomp_complains_on_invalid_back_reference_for_bre(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\1", 0), RETROLIBC_REG_ESUBREG);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\(abc\\)\\2", 0), RETROLIBC_REG_ESUBREG);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\(abc\\1\\)", 0), RETROLIBC_REG_ESUBREG);
}

static void test_regcomp_complains_on_unmatched_left_bracket_or_left_bracket_caret_for_bre(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[abc", 0), RETROLIBC_REG_EBRACK);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[^abc", 0), RETROLIBC_REG_EBRACK);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "abc]", 0), RETROLIBC_REG_EBRACK);
}

static void test_regcomp_complains_on_unmatched_left_parenthesis_or_backslash_left_parenthesis_for_bre(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\(abc", 0), RETROLIBC_REG_EPAREN);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "abc\\)", 0), RETROLIBC_REG_EPAREN);
}

static void test_regcomp_complains_on_unmatched_backslash_left_brace_for_bre(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\{1\\}", 0), RETROLIBC_REG_EBRACE);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "a\\{1", 0), RETROLIBC_REG_EBRACE);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "a\\}", 0), RETROLIBC_REG_EBRACE);
}

static void test_regcomp_complains_on_invalid_content_of_backslahs_left_brace_backslash_right_brace_for_bre(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "a\\{a\\}", 0), RETROLIBC_REG_BADBR);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "a\\{3,2\\}", 0), RETROLIBC_REG_BADBR);
}

static void test_regcomp_complains_on_invalid_range_end_for_bre(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[a-]", 0), RETROLIBC_REG_ERANGE);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[a-", 0), RETROLIBC_REG_ERANGE);
}

static void test_regcomp_complains_on_premature_end_of_regular_expression_for_bre(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "*", 0), RETROLIBC_REG_BADRPT);
}

static void test_regcomp_complains_on_invalid_regular_expression_for_ere(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "a^bc", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_BADPAT);
}

static void test_regcomp_complains_on_invalid_character_class_name_for_ere(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[[:xxx:]]", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_ECTYPE);
}

static void test_regcomp_complains_on_trailing_backslash_for_ere(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "\\", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_EESCAPE);
}

static void test_regcomp_complains_on_unmatched_left_bracket_or_left_bracket_caret_for_ere(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[abc", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_EBRACK);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[^abc", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_EBRACK);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "abc]", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_EBRACK);
}

static void test_regcomp_complains_on_unmatched_left_parenthesis_or_backslash_left_parenthesis_for_ere(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "(abc", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_EPAREN);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "abc)", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_EPAREN);
}

static void test_regcomp_complains_on_unmatched_backslash_left_brace_for_ere(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "{1}", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_EBRACE);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "a{1", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_EBRACE);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "a}", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_EBRACE);
}

static void test_regcomp_complains_on_invalid_content_of_backslahs_left_brace_backslash_right_brace_for_ere(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "a{a}", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_BADBR);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "a{3,2}", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_BADBR);
}

static void test_regcomp_complains_on_invalid_range_end_for_ere(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[a-]", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_ERANGE);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "[a-", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_ERANGE);
}

static void test_regcomp_complains_on_premature_end_of_regular_expression_for_ere(void)
{
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "*", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_BADRPT);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "+", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_BADRPT);
  CU_ASSERT_EQUAL(retrolibc_regcomp(&reg, "?", RETROLIBC_REG_EXTENDED), RETROLIBC_REG_BADRPT);
}

static void test_regexec_interprets_bytecode(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "abc", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "aaaabc", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 3);
  CU_ASSERT_EQUAL(matches[0].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "def", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_star(void)
{
  static retrolibc_regmatch_t matches[2];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "\\(abc\\)*d", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcd", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcabcd", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 7);
  CU_ASSERT_EQUAL(matches[1].re_so, 3);
  CU_ASSERT_EQUAL(matches[1].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcabcabcd", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 10);
  CU_ASSERT_EQUAL(matches[1].re_so, 6);
  CU_ASSERT_EQUAL(matches[1].re_eo, 9);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "d", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "ghi", 2, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_2_3_duplication(void)
{
  static retrolibc_regmatch_t matches[2];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "\\(abc\\)\\{2,3\\}d", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcabcd", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 7);
  CU_ASSERT_EQUAL(matches[1].re_so, 3);
  CU_ASSERT_EQUAL(matches[1].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcabcabcd", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 10);
  CU_ASSERT_EQUAL(matches[1].re_so, 6);
  CU_ASSERT_EQUAL(matches[1].re_eo, 9);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcabcabce", 2, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "def", 2, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_1_3_duplication(void)
{
  static retrolibc_regmatch_t matches[2];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "\\(abc\\)\\{1,3\\}d", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcd", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcabcd", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 7);
  CU_ASSERT_EQUAL(matches[1].re_so, 3);
  CU_ASSERT_EQUAL(matches[1].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcabcabcd", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 10);
  CU_ASSERT_EQUAL(matches[1].re_so, 6);
  CU_ASSERT_EQUAL(matches[1].re_eo, 9);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcabcabce", 2, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "def", 2, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_plus(void)
{
  static retrolibc_regmatch_t matches[2];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "(abc)+d", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcd", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcabcd", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 7);
  CU_ASSERT_EQUAL(matches[1].re_so, 3);
  CU_ASSERT_EQUAL(matches[1].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcabcabcd", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 10);
  CU_ASSERT_EQUAL(matches[1].re_so, 6);
  CU_ASSERT_EQUAL(matches[1].re_eo, 9);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "d", 2, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "ghi", 2, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_question(void)
{
  static retrolibc_regmatch_t matches[2];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "(abc)?d", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcd", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "d", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "ghi", 2, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_alternation(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "abc|def|ghi", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "def", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "ghi", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "jkl", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_star_with_alternation(void)
{
  static retrolibc_regmatch_t matches[2];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "(abc|def)*g", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcg", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "defg", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcabcg", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 7);
  CU_ASSERT_EQUAL(matches[1].re_so, 3);
  CU_ASSERT_EQUAL(matches[1].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcdefg", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 7);
  CU_ASSERT_EQUAL(matches[1].re_so, 3);
  CU_ASSERT_EQUAL(matches[1].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "defabcg", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 7);
  CU_ASSERT_EQUAL(matches[1].re_so, 3);
  CU_ASSERT_EQUAL(matches[1].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "defdefg", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 7);
  CU_ASSERT_EQUAL(matches[1].re_so, 3);
  CU_ASSERT_EQUAL(matches[1].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "g", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "jkl", 2, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_attack_by_star_with_alternation(void)
{
  static retrolibc_regmatch_t matches[2];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "(a|aa)*b", RETROLIBC_REG_EXTENDED), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "ab", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 2);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "aaaab", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 5);
  CU_ASSERT_EQUAL(matches[1].re_so, 3);
  CU_ASSERT_EQUAL(matches[1].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "aaaac", 2, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "aaaaaac", 2, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_2_subexpressions(void)
{
  static retrolibc_regmatch_t matches[3];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "\\(abc\\)\\(def\\)", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcdef", 3, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 6);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  CU_ASSERT_EQUAL(matches[2].re_so, 3);
  CU_ASSERT_EQUAL(matches[2].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "def", 3, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  CU_ASSERT_EQUAL(matches[2].re_so, -1);
  CU_ASSERT_EQUAL(matches[2].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_nested_subexpression(void)
{
  static retrolibc_regmatch_t matches[3];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "\\(abc\\(def\\)ghi\\)", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcdefghi", 3, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 9);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 9);
  CU_ASSERT_EQUAL(matches[2].re_so, 3);
  CU_ASSERT_EQUAL(matches[2].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "def", 3, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  CU_ASSERT_EQUAL(matches[2].re_so, -1);
  CU_ASSERT_EQUAL(matches[2].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_2_back_references(void)
{
  static retrolibc_regmatch_t matches[3];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "\\(abc\\)\\(def\\)\\1\\2", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcdefabcdef", 3, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 12);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  CU_ASSERT_EQUAL(matches[2].re_so, 3);
  CU_ASSERT_EQUAL(matches[2].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcdef", 3, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  CU_ASSERT_EQUAL(matches[2].re_so, -1);
  CU_ASSERT_EQUAL(matches[2].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_back_references_with_nested_subexpression(void)
{
  static retrolibc_regmatch_t matches[3];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "\\(abc\\(def\\)\\2ghi\\)\\1", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcdefdefghiabcdefdefghi", 3, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 24);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 12);
  CU_ASSERT_EQUAL(matches[2].re_so, 3);
  CU_ASSERT_EQUAL(matches[2].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcdef", 3, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  CU_ASSERT_EQUAL(matches[2].re_so, -1);
  CU_ASSERT_EQUAL(matches[2].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcdefdefghi", 3, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  CU_ASSERT_EQUAL(matches[2].re_so, -1);
  CU_ASSERT_EQUAL(matches[2].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_brackets(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[abc]def", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "adef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "bdef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "cdef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "ddef", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_brackets_with_caret(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[^abc]def", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "adef", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "bdef", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "cdef", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "ddef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "\ndef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_brackets_with_ctype(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[a[:alpha:]c]def", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "adef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "bdef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "cdef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "Adef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "1def", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_brackets_with_range(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[ad-fi]def", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "adef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "cdef", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "ddef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "edef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "fdef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "gdef", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "idef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_dot(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, ".", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "a", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "b", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "c", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "A", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "1", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "\n", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_0234(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "\234", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "\234", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "a", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "b", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "c", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_lowercase_letter(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "a", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "a", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "A", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_uppercase_letter(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "A", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "a", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "A", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_alnum(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:alnum:]]", 0), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isalnum(c) ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_alpha(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:alpha:]]", 0), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isalpha(c) ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_blank(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:blank:]]", 0), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = ((c == ' ' || c == '\t') ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_cntrl(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:cntrl:]]", 0), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = ((iscntrl(c) && c != '\0') ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_digit(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:digit:]]", 0), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isdigit(c) ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_graph(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:graph:]]", 0), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isgraph(c) ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_lower(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:lower:]]", 0), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (islower(c) ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_print(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:print:]]", 0), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isprint(c) ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_punct(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:punct:]]", 0), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (ispunct(c) ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_space(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:space:]]", 0), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isspace(c) ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_upper(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:upper:]]", 0), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isupper(c) ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_xdigit(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:xdigit:]]", 0), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isxdigit(c) ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_back_reference_with_lowercase_subexpression(void)
{
  static retrolibc_regmatch_t matches[2];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "\\(abc\\)\\1", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcabc", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 6);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcABC", 2, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_back_reference_with_uppercase_subexpression(void)
{
  static retrolibc_regmatch_t matches[2];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "\\(ABC\\)\\1", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "ABCabc", 2, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(matches[1].re_so, -1);
  CU_ASSERT_EQUAL(matches[1].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "ABCABC", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 6);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_lowercase_letter_and_reg_icase(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "a", RETROLIBC_REG_ICASE), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "a", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "A", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_uppercase_letter_and_reg_icase(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "A", RETROLIBC_REG_ICASE), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "a", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "A", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_lowercase_range_and_reg_icase(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[a-z]", RETROLIBC_REG_ICASE), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "a", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "A", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_uppercase_range_and_reg_icase(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[A-Z]", RETROLIBC_REG_ICASE), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "a", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "A", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_lower_and_reg_icase(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:lower:]]", RETROLIBC_REG_ICASE), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isalpha(c) ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_upper_and_reg_icase(void)
{
  static char buf[128];
  static char expected_buf[128];
  static char s[2];
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[[:upper:]]", RETROLIBC_REG_ICASE), 0);
  int c;
  for(c = 0; c < 128; c++) expected_buf[c] = (isalpha(c) ? '1' : '0');
  for(c = 0; c < 128; c++) {
    s[0] = c;
    s[1] = '\0';
    buf[c] = (retrolibc_regexec(&reg, s, 1, matches, 0) == 0 ? '1' : '0');
  }
  CU_ASSERT_EQUAL(memcmp(buf, expected_buf, 128), 0);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_back_reference_with_lowercase_subexpression_and_reg_icase(void)
{
  static retrolibc_regmatch_t matches[2];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "\\(abc\\)\\1", RETROLIBC_REG_ICASE), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcabc", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 6);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abcABC", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 6);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_back_reference_with_uppercase_subexpression_and_reg_icase(void)
{
  static retrolibc_regmatch_t matches[2];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "\\(ABC\\)\\1", RETROLIBC_REG_ICASE), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "ABCabc", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 6);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "ABCABC", 2, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 6);
  CU_ASSERT_EQUAL(matches[1].re_so, 0);
  CU_ASSERT_EQUAL(matches[1].re_eo, 3);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_reg_nosub(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "abc", RETROLIBC_REG_NOSUB), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "aaaabc", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "def", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_caret(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "^abc", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "aaaabc", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "aa\nabcaaa", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, RETROLIBC_REG_NOTBOL), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, RETROLIBC_REG_NOTEOL), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 3);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_dollar(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "abc$", 0), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "aaaabc", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 3);
  CU_ASSERT_EQUAL(matches[0].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "aaaabc\naa", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, RETROLIBC_REG_NOTBOL), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, RETROLIBC_REG_NOTEOL), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_caret_and_reg_newline(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "^abc", RETROLIBC_REG_NEWLINE), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "aaaabc", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "aa\nabcaaa", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 3);
  CU_ASSERT_EQUAL(matches[0].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, RETROLIBC_REG_NOTBOL), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "\nabc", 1, matches, RETROLIBC_REG_NOTBOL), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 1);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, RETROLIBC_REG_NOTEOL), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 3);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_dollar_and_reg_newline(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "abc$", RETROLIBC_REG_NEWLINE), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "aaaabc", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 3);
  CU_ASSERT_EQUAL(matches[0].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "aaaabc\naa", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 3);
  CU_ASSERT_EQUAL(matches[0].re_eo, 6);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, RETROLIBC_REG_NOTBOL), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 3);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc", 1, matches, RETROLIBC_REG_NOTEOL), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "abc\n", 1, matches, RETROLIBC_REG_NOTEOL), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 3);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_brackets_with_caret_and_reg_newline(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, "[^abc]def", RETROLIBC_REG_NEWLINE), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "adef", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "bdef", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "cdef", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "ddef", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 4);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "\ndef", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regexec_interprets_bytecode_for_dot_and_reg_newline(void)
{
  static retrolibc_regmatch_t matches[1];
  retrolibc_regex_t reg;
  CU_ASSERT_EQUAL_FATAL(retrolibc_regcomp(&reg, ".", RETROLIBC_REG_NEWLINE), 0);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "a", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "b", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "c", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "A", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "1", 1, matches, 0), 0);
  CU_ASSERT_EQUAL(matches[0].re_so, 0);
  CU_ASSERT_EQUAL(matches[0].re_eo, 1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "\n", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  CU_ASSERT_EQUAL(retrolibc_regexec(&reg, "", 1, matches, 0), RETROLIBC_REG_NOTMATCH);
  CU_ASSERT_EQUAL(matches[0].re_so, -1);
  CU_ASSERT_EQUAL(matches[0].re_eo, -1);
  retrolibc_regfree(&reg);
}

static void test_regerror_copies_error_message(void)
{
  static char buf[100];
  retrolibc_regex_t reg = { NULL, 0, 0 };
  const char *s;
  s = "Success";
  CU_ASSERT_EQUAL(retrolibc_regerror(0, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "No match";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_NOTMATCH, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Invalid regular expression";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_BADPAT, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Invalid collation character";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_ECOLLATE, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Invalid character class name";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_ECTYPE, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Trailing backslash";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_EESCAPE, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Invalid back reference";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_ESUBREG, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Unmatched [ or [^";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_EBRACK, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Unmatched ( or \\(";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_EPAREN, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Unmatched \\{";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_EBRACE, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Invalid content of \\{\\}";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_BADBR, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Invalid range end";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_ERANGE, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Memory exhausted";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_ESPACE, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Premature end of regular expression";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_BADRPT, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Unimplemented function";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_ENOSYS, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Unknown error";
  CU_ASSERT_EQUAL(retrolibc_regerror(-1, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
  s = "Unknown error";
  CU_ASSERT_EQUAL(retrolibc_regerror(15, &reg, buf, 100), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, s);
}

static void test_regerror_copies_error_message_part(void)
{
  static char buf[10];
  retrolibc_regex_t reg = { NULL, 0, 0 };
  const char *s;
  s = "Invalid regular expression";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_BADPAT, &reg, buf, 10), strlen(s) + 1);
  CU_ASSERT_STRING_EQUAL(buf, "Invalid r");
}

static void test_regerror_returns_number_of_bytes(void)
{
  retrolibc_regex_t reg = { NULL, 0, 0 };
  const char *s;
  s = "Success";
  CU_ASSERT_EQUAL(retrolibc_regerror(0, &reg, NULL, 0), strlen(s) + 1);
  s = "No match";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_NOTMATCH, &reg, NULL, 0), strlen(s) + 1);
  s = "Invalid regular expression";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_BADPAT, &reg, NULL, 0), strlen(s) + 1);
  s = "Invalid collation character";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_ECOLLATE, &reg, NULL, 0), strlen(s) + 1);
  s = "Invalid character class name";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_ECTYPE, &reg, NULL, 0), strlen(s) + 1);
  s = "Trailing backslash";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_EESCAPE, &reg, NULL, 0), strlen(s) + 1);
  s = "Invalid back reference";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_ESUBREG, &reg, NULL, 0), strlen(s) + 1);
  s = "Unmatched [ or [^";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_EBRACK, &reg, NULL, 0), strlen(s) + 1);
  s = "Unmatched ( or \\(";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_EPAREN, &reg, NULL, 0), strlen(s) + 1);
  s = "Unmatched \\{";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_EBRACE, &reg, NULL, 0), strlen(s) + 1);
  s = "Invalid content of \\{\\}";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_BADBR, &reg, NULL, 0), strlen(s) + 1);
  s = "Invalid range end";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_ERANGE, &reg, NULL, 0), strlen(s) + 1);
  s = "Memory exhausted";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_ESPACE, &reg, NULL, 0), strlen(s) + 1);
  s = "Premature end of regular expression";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_BADRPT, &reg, NULL, 0), strlen(s) + 1);
  s = "Unimplemented function";
  CU_ASSERT_EQUAL(retrolibc_regerror(RETROLIBC_REG_ENOSYS, &reg, NULL, 0), strlen(s) + 1);
  s = "Unknown error";
  CU_ASSERT_EQUAL(retrolibc_regerror(-1, &reg, NULL, 0), strlen(s) + 1);
  s = "Unknown error";
  CU_ASSERT_EQUAL(retrolibc_regerror(15, &reg, NULL, 0), strlen(s) + 1);
}

int add_regex_suite(void)
{
  CU_pSuite suite;
  if((suite = CU_add_suite("regex suite", initialize_regex, cleanup_regex)) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE", test_regcomp_compiles_regular_expression_for_bre) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and anchors", test_regcomp_compiles_regular_expression_for_bre_and_anchors) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and concatenation", test_regcomp_compiles_regular_expression_for_bre_and_concatenation) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and *", test_regcomp_compiles_regular_expression_for_bre_and_star) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and 3 duplication", test_regcomp_compiles_regular_expression_for_bre_and_3_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and 3, duplication", test_regcomp_compiles_regular_expression_for_bre_and_3_comma_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and 2,3 duplication", test_regcomp_compiles_regular_expression_for_bre_and_2_3_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and 1,3 duplication", test_regcomp_compiles_regular_expression_for_bre_and_1_3_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and 1 duplication", test_regcomp_compiles_regular_expression_for_bre_and_1_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and 0 duplication", test_regcomp_compiles_regular_expression_for_bre_and_0_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and subexpression", test_regcomp_compiles_regular_expression_for_bre_and_subexpression) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and nested subexpression", test_regcomp_compiles_regular_expression_for_bre_and_nested_subexpression) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and empty subexpression", test_regcomp_compiles_regular_expression_for_bre_and_empty_subexpression) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and back references", test_regcomp_compiles_regular_expression_for_bre_and_back_references) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and back references with nested subexpression", test_regcomp_compiles_regular_expression_for_bre_and_back_references_with_nested_subexpression) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and brackets", test_regcomp_compiles_regular_expression_for_bre_and_brackets) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and brackets with caret", test_regcomp_compiles_regular_expression_for_bre_and_brackets_with_caret) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and brackets with ctype", test_regcomp_compiles_regular_expression_for_bre_and_brackets_with_ctype) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and brackets with range", test_regcomp_compiles_regular_expression_for_bre_and_brackets_with_range) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and brackets with [", test_regcomp_compiles_regular_expression_for_bre_and_brackets_with_left_bracket) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and escape", test_regcomp_compiles_regular_expression_for_bre_and_escape) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and .", test_regcomp_compiles_regular_expression_for_bre_and_dot) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and 0234", test_regcomp_compiles_regular_expression_for_bre_and_0234) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and alnum", test_regcomp_compiles_regular_expression_for_bre_and_alnum) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and alpha", test_regcomp_compiles_regular_expression_for_bre_and_alpha) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and blank", test_regcomp_compiles_regular_expression_for_bre_and_blank) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and cntrl", test_regcomp_compiles_regular_expression_for_bre_and_cntrl) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and digit", test_regcomp_compiles_regular_expression_for_bre_and_digit) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and graph", test_regcomp_compiles_regular_expression_for_bre_and_graph) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and lower", test_regcomp_compiles_regular_expression_for_bre_and_lower) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and print", test_regcomp_compiles_regular_expression_for_bre_and_print) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and punct", test_regcomp_compiles_regular_expression_for_bre_and_punct) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and space", test_regcomp_compiles_regular_expression_for_bre_and_space) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and punct", test_regcomp_compiles_regular_expression_for_bre_and_upper) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and xdigit", test_regcomp_compiles_regular_expression_for_bre_and_xdigit) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for BRE and subexpression and *", test_regcomp_compiles_regular_expression_for_bre_and_subexpression_and_star) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles empty regular expression for BRE", test_regcomp_compiles_empty_regular_expression_for_bre) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE", test_regcomp_compiles_regular_expression_for_ere) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and alternation", test_regcomp_compiles_regular_expression_for_ere_and_alternation) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and anchors", test_regcomp_compiles_regular_expression_for_ere_and_anchors) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and concatenation", test_regcomp_compiles_regular_expression_for_ere_and_concatenation) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and *", test_regcomp_compiles_regular_expression_for_ere_and_star) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and +", test_regcomp_compiles_regular_expression_for_ere_and_plus) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and ?", test_regcomp_compiles_regular_expression_for_ere_and_question) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and 3 duplication", test_regcomp_compiles_regular_expression_for_ere_and_3_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and 3, duplication", test_regcomp_compiles_regular_expression_for_ere_and_3_comma_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and 2,3 duplication", test_regcomp_compiles_regular_expression_for_ere_and_2_3_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and 1,3 duplication", test_regcomp_compiles_regular_expression_for_ere_and_1_3_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and 1 duplication", test_regcomp_compiles_regular_expression_for_ere_and_1_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and 0 duplication", test_regcomp_compiles_regular_expression_for_ere_and_0_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and subexpression", test_regcomp_compiles_regular_expression_for_ere_and_subexpression) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and nested subexpression", test_regcomp_compiles_regular_expression_for_ere_and_nested_subexpression) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and empty subexpression", test_regcomp_compiles_regular_expression_for_ere_and_empty_subexpression) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and brackets", test_regcomp_compiles_regular_expression_for_ere_and_brackets) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and brackets with caret", test_regcomp_compiles_regular_expression_for_ere_and_brackets_with_caret) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and brackets with ctype", test_regcomp_compiles_regular_expression_for_ere_and_brackets_with_ctype) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and brackets with range", test_regcomp_compiles_regular_expression_for_ere_and_brackets_with_range) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and brackets with [", test_regcomp_compiles_regular_expression_for_ere_and_brackets_with_left_bracket) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and escape", test_regcomp_compiles_regular_expression_for_ere_and_escape) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and .", test_regcomp_compiles_regular_expression_for_ere_and_dot) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and 0234", test_regcomp_compiles_regular_expression_for_ere_and_0234) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and alnum", test_regcomp_compiles_regular_expression_for_ere_and_alnum) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and alpha", test_regcomp_compiles_regular_expression_for_ere_and_alpha) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and blank", test_regcomp_compiles_regular_expression_for_ere_and_blank) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and cntrl", test_regcomp_compiles_regular_expression_for_ere_and_cntrl) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and digit", test_regcomp_compiles_regular_expression_for_ere_and_digit) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and graph", test_regcomp_compiles_regular_expression_for_ere_and_graph) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and lower", test_regcomp_compiles_regular_expression_for_ere_and_lower) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and print", test_regcomp_compiles_regular_expression_for_ere_and_print) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and punct", test_regcomp_compiles_regular_expression_for_ere_and_punct) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and space", test_regcomp_compiles_regular_expression_for_ere_and_space) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and punct", test_regcomp_compiles_regular_expression_for_ere_and_upper) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and xdigit", test_regcomp_compiles_regular_expression_for_ere_and_xdigit) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles regular expression for ERE and subexpression and *", test_regcomp_compiles_regular_expression_for_ere_and_subexpression_and_star) == NULL) return -1;
  if(CU_add_test(suite, "regcomp compiles empty regular expression for ERE", test_regcomp_compiles_empty_regular_expression_for_ere) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on invalid regular expression for BRE", test_regcomp_complains_on_invalid_regular_expression_for_bre) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on invalid character class name for BRE", test_regcomp_complains_on_invalid_character_class_name_for_bre) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on trailing backslash for BRE", test_regcomp_complains_on_trailing_backslash_for_bre) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on invalid back reference for BRE", test_regcomp_complains_on_invalid_back_reference_for_bre) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on unmatched [ or [^ for BRE", test_regcomp_complains_on_unmatched_left_bracket_or_left_bracket_caret_for_bre) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on unmatched ( or \\( for BRE", test_regcomp_complains_on_unmatched_left_parenthesis_or_backslash_left_parenthesis_for_bre) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on unmatched \\{ for BRE", test_regcomp_complains_on_unmatched_backslash_left_brace_for_bre) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on invalid content of \\{\\} for BRE", test_regcomp_complains_on_invalid_content_of_backslahs_left_brace_backslash_right_brace_for_bre) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on invalid range end for BRE", test_regcomp_complains_on_invalid_range_end_for_bre) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on premature end of regular expression for BRE", test_regcomp_complains_on_premature_end_of_regular_expression_for_bre) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on invalid regular expression for ERE", test_regcomp_complains_on_invalid_regular_expression_for_ere) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on invalid character class name for ERE", test_regcomp_complains_on_invalid_character_class_name_for_ere) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on trailing backslash for ERE", test_regcomp_complains_on_trailing_backslash_for_ere) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on unmatched [ or [^ for ERE", test_regcomp_complains_on_unmatched_left_bracket_or_left_bracket_caret_for_ere) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on unmatched ( or \\( for ERE", test_regcomp_complains_on_unmatched_left_parenthesis_or_backslash_left_parenthesis_for_ere) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on unmatched \\{ for ERE", test_regcomp_complains_on_unmatched_backslash_left_brace_for_ere) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on invalid content of \\{\\} for BRE", test_regcomp_complains_on_invalid_content_of_backslahs_left_brace_backslash_right_brace_for_ere) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on invalid range end for ERE", test_regcomp_complains_on_invalid_range_end_for_ere) == NULL) return -1;
  if(CU_add_test(suite, "regcomp complains on premature end of regular expression for ERE", test_regcomp_complains_on_premature_end_of_regular_expression_for_ere) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode", test_regexec_interprets_bytecode) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for *", test_regexec_interprets_bytecode_for_star) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for 2,3 duplication", test_regexec_interprets_bytecode_for_2_3_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for 1,3 duplication", test_regexec_interprets_bytecode_for_1_3_duplication) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for +", test_regexec_interprets_bytecode_for_plus) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for ?", test_regexec_interprets_bytecode_for_question) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for alternation", test_regexec_interprets_bytecode_for_alternation) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for * with alternation", test_regexec_interprets_bytecode_for_star_with_alternation) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for attack by * with alternation", test_regexec_interprets_bytecode_for_attack_by_star_with_alternation) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for 2 subexpressions", test_regexec_interprets_bytecode_for_2_subexpressions) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for nested subexpression", test_regexec_interprets_bytecode_for_nested_subexpression) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for 2 back references", test_regexec_interprets_bytecode_for_2_back_references) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for back references with nested subexpression", test_regexec_interprets_bytecode_for_back_references_with_nested_subexpression) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for brackets", test_regexec_interprets_bytecode_for_brackets) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for brackets with ^", test_regexec_interprets_bytecode_for_brackets_with_caret) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for brackets with ctype", test_regexec_interprets_bytecode_for_brackets_with_ctype) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for brackets with range", test_regexec_interprets_bytecode_for_brackets_with_range) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for .", test_regexec_interprets_bytecode_for_dot) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for 0234", test_regexec_interprets_bytecode_for_0234) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for lowercase letter", test_regexec_interprets_bytecode_for_lowercase_letter) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for uppercase letter", test_regexec_interprets_bytecode_for_uppercase_letter) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for alnum", test_regexec_interprets_bytecode_for_alnum) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for alpha", test_regexec_interprets_bytecode_for_alpha) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for blank", test_regexec_interprets_bytecode_for_blank) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for cntrl", test_regexec_interprets_bytecode_for_cntrl) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for digit", test_regexec_interprets_bytecode_for_digit) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for graph", test_regexec_interprets_bytecode_for_graph) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for lower", test_regexec_interprets_bytecode_for_lower) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for print", test_regexec_interprets_bytecode_for_print) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for punct", test_regexec_interprets_bytecode_for_punct) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for space", test_regexec_interprets_bytecode_for_space) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for upper", test_regexec_interprets_bytecode_for_upper) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for xdigit", test_regexec_interprets_bytecode_for_xdigit) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for back reference with lowercase subexpression", test_regexec_interprets_bytecode_for_back_reference_with_lowercase_subexpression) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for back reference with uppercase subexpression", test_regexec_interprets_bytecode_for_back_reference_with_uppercase_subexpression) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for lowercase letter and REG_ICASE", test_regexec_interprets_bytecode_for_lowercase_letter_and_reg_icase) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for uppercase letter and REG_ICASE", test_regexec_interprets_bytecode_for_uppercase_letter_and_reg_icase) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for lowercase range and REG_ICASE", test_regexec_interprets_bytecode_for_lowercase_range_and_reg_icase) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for uppercase range and REG_ICASE", test_regexec_interprets_bytecode_for_uppercase_range_and_reg_icase) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for lower and REG_ICASE", test_regexec_interprets_bytecode_for_lower_and_reg_icase) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for upper and REG_ICASE", test_regexec_interprets_bytecode_for_upper_and_reg_icase) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for back reference with lowercase subexpression and REG_ICASE", test_regexec_interprets_bytecode_for_back_reference_with_lowercase_subexpression_and_reg_icase) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for back reference with uppercase subexpression and REG_ICASE", test_regexec_interprets_bytecode_for_back_reference_with_uppercase_subexpression_and_reg_icase) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for REG_NOSUB", test_regexec_interprets_bytecode_for_reg_nosub) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for ^", test_regexec_interprets_bytecode_for_caret) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for $", test_regexec_interprets_bytecode_for_dollar) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for ^ and REG_NEWLINE", test_regexec_interprets_bytecode_for_caret_and_reg_newline) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for $ and REG_NEWLINE", test_regexec_interprets_bytecode_for_dollar_and_reg_newline) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for brackets with ^ and REG_NEWLINE", test_regexec_interprets_bytecode_for_brackets_with_caret_and_reg_newline) == NULL) return -1;
  if(CU_add_test(suite, "regexec interprets bytecode for . and REG_NEWLINE", test_regexec_interprets_bytecode_for_dot_and_reg_newline) == NULL) return -1;
  if(CU_add_test(suite, "regerror copies error message", test_regerror_copies_error_message) == NULL) return -1;
  if(CU_add_test(suite, "regerror copies error message part", test_regerror_copies_error_message_part) == NULL) return -1;
  if(CU_add_test(suite, "regerror returns number of bytes", test_regerror_returns_number_of_bytes) == NULL) return -1;
  return 0;
}

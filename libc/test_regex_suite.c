/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
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
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_BFJ, 1), 1);
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
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_BFJ, 7), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_BFJ, 1), 1);
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
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_BFJ, 1), 1);
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
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_BFJ, 7), 1);
  CU_ASSERT_EQUAL(eq_op(&reg, &ip, 'b'), 1);
  CU_ASSERT_EQUAL(eq_op_with_int(&reg, &ip, RE_OP_BFJ, 1), 1);
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
  return 0;
}

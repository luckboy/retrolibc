/*
 * Copyright (c) 2026 Łukasz Szpakowski
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "priv_regex.h"

#define BYTECODE_STEP   1024
#define STACK_STEP      64

struct compiler
{
  short cflags;
  unsigned char *bytecode;
  ssize_t bytecode_size;
  ssize_t bytecode_capacity;
  size_t paren_level;
  size_t nsub;
  size_t nsub1;
  signed char errcode;
};

static int comp_get_int(struct compiler *comp, ssize_t i)
{ return (int) ((comp->bytecode[i] << 24) | (comp->bytecode[i + 1] << 16) | (comp->bytecode[i + 2] << 8) | comp->bytecode[i + 3]); }

static void comp_set_int(struct compiler *comp, ssize_t i, int x)
{
  comp->bytecode[i] = x >> 24;
  comp->bytecode[i + 1] = x >> 16;
  comp->bytecode[i + 2] = x >> 8;
  comp->bytecode[i + 3] = x;
}

static int comp_set_addr(struct compiler *comp, ssize_t i, ssize_t addr, ssize_t ip)
{
  ssize_t diff = addr - ip;
  if(diff > INT_MAX || diff < INT_MAX) return -1;
  comp_set_int(comp, i, diff);
  return 0;
}

static int add_block(struct compiler *comp, unsigned char *s, ssize_t n)
{
  if(comp->bytecode_size > LONG_MAX - n) {
    comp->errcode = REG_ESPACE;
    return -1;
  }
  if(comp->bytecode_size + n > comp->bytecode_capacity) {
    unsigned char *new_bytecode;
    comp->bytecode_capacity = ((comp->bytecode_size + n + BYTECODE_STEP - 1) / BYTECODE_STEP) * BYTECODE_STEP;
    new_bytecode = realloc(comp->bytecode, comp->bytecode_capacity);
    if(new_bytecode == NULL) {
      comp->errcode = REG_ESPACE;
      return -1;
    }
    comp->bytecode = new_bytecode;
  }
  memcpy(comp->bytecode + comp->bytecode_size, s, n);
  comp->bytecode_size += n;
  return 0;
}

static int add_op(struct compiler *comp, int op)
{
  unsigned char instr = op;
  return add_block(comp, &instr, 1);
}

static int add_op_with_char(struct compiler *comp, int op, int c)
{
  unsigned char instr[2];
  instr[0] = op;
  instr[1] = c;
  return add_block(comp, instr, 2);
}

static int add_op_with_int(struct compiler *comp, int op, int x)
{
  unsigned char instr[5];
  instr[0] = op;
  instr[1] = x >> 24;
  instr[2] = x >> 16;
  instr[3] = x >> 8;
  instr[4] = x;
  return add_block(comp, instr, 5);
}

static int add_op_with_addr(struct compiler *comp, int op, ssize_t addr)
{
  ssize_t ip = comp->bytecode_size + 5;
  ssize_t diff = addr - ip;
  if(diff > INT_MAX || diff < INT_MAX) {
    comp->errcode = REG_ESPACE;
    return -1;
  }
  return add_op_with_int(comp, op, diff);
}

static int add_op_with_size(struct compiler *comp, int op, size_t x)
{
  if(x >= (size_t) INT_MAX) {
    comp->errcode = REG_ESPACE;
    return -1;
  }
  return add_op_with_int(comp, op, (int) x);
}

static int add_op_with_range(struct compiler *comp, int op, int c, int c2)
{
  unsigned char instr[3];
  instr[0] = op;
  instr[1] = c;
  instr[2] = c2;
  return add_block(comp, instr, 3);
}

static int add_match(struct compiler *comp, int c)
{
  if(c <= 127) {
    if(add_op(comp, c) == -1) return -1;
  } else {
    if(add_op_with_char(comp, RE_OP_MATCH, c) == -1) return -1;
  }
  return 0;
}

static long parse_long(const char *s, char **endptr)
{
  if(isdigit(*s)) {
    int saved_errno = errno;
    long n;
    errno = 0;
    n = strtol(s, endptr, 10);
    if(n == LONG_MAX && errno == ERANGE) {
      errno = saved_errno;
      return -1L;
    }
    return n;
  } else {
    return -1L;
  }
}

static const char *compile2(struct compiler *comp, const char *s);
static const char *compile1(struct compiler *comp, const char *s);

static const char *compile7(struct compiler *comp, const char *s, int is_in_brackets)
{
  switch(*s) {
  case '[':
    if(s[1] == ':') {
      s += 2;
      if(strncmp(s, "alnum", 5) == 0) {
        if(add_op(comp, RE_OP_ALNUM) == -1) return NULL;
      } else if(strncmp(s, "alpha", 5) == 0) {
        if(add_op(comp, RE_OP_ALPHA) == -1) return NULL;
      } else if(strncmp(s, "blank", 5) == 0) {
        if(add_op(comp, RE_OP_BLANK) == -1) return NULL;
      } else if(strncmp(s, "cntrl", 5) == 0) {
        if(add_op(comp, RE_OP_CNTRL) == -1) return NULL;
      } else if(strncmp(s, "digit", 5) == 0) {
        if(add_op(comp, RE_OP_DIGIT) == -1) return NULL;
      } else if(strncmp(s, "graph", 5) == 0) {
        if(add_op(comp, RE_OP_GRAPH) == -1) return NULL;
      } else if(strncmp(s, "lower", 5) == 0) {
        if(add_op(comp, RE_OP_LOWER) == -1) return NULL;
      } else if(strncmp(s, "print", 5) == 0) {
        if(add_op(comp, RE_OP_PRINT) == -1) return NULL;
      } else if(strncmp(s, "punct", 5) == 0) {
        if(add_op(comp, RE_OP_PUNCT) == -1) return NULL;
      } else if(strncmp(s, "punct", 5) == 0) {
        if(add_op(comp, RE_OP_PUNCT) == -1) return NULL;
      } else if(strncmp(s, "space", 5) == 0) {
        if(add_op(comp, RE_OP_SPACE) == -1) return NULL;
      } else if(strncmp(s, "upper", 5) == 0) {
        if(add_op(comp, RE_OP_UPPER) == -1) return NULL;
      } else if(strncmp(s, "xdigit", 6) == 0) {
        if(add_op(comp, RE_OP_XDIGIT) == -1) return NULL;
        s++;
      } else {
        comp->errcode = REG_ECTYPE;
        return NULL;
      }
      s += 5;
      if(*s != ':' || s[1] != ']') {
        comp->errcode = REG_EBRACK;
        return NULL;
      }
      s += 2;
    } else {
      if(!is_in_brackets)
      if(add_match(comp, s[1]) == -1) return NULL;
    }
    break;
  case '\\':
    if(s[1] == '\0') {
      comp->errcode = REG_EESCAPE;
      return NULL;
    }
    if(!is_in_brackets) {
      if((comp->cflags & REG_EXTENDED) == 0) {
        if(s[1] == '(' || s[1] == ')' || s[1] == '{' || s[1] == '}' || (s[1] >= '1' && s[1] <= '9')) return s;
      }
    }
    if(add_match(comp, s[1]) == -1) return NULL;
    s += 2;
    break;
  default:
    if(!is_in_brackets) {
      if(*s == '*' || *s == '^' || *s == '$') return s;
      if((comp->cflags & REG_EXTENDED) != 0) {
        if(*s == '(' || *s == ')' || *s == '{' || *s == '}' || *s == '+' || *s == '?' || *s == '|') return s;
      }
      if(*s == '.') {
        if(add_op(comp, RE_OP_ANY) == -1) return NULL;
      } else {
        if(add_match(comp, *s) == -1) return NULL;
      }
      s++;
    } else {
      if(s[1] == '-') {
        if(s[2] == ']' || s[2] == '\0') {
          comp->errcode = REG_ERANGE;
          return NULL;
        }
        if(add_op_with_range(comp, RE_OP_RANGE, *s, s[2]) == -1) return NULL;
        s += 3;
      } else {
        if(add_match(comp, *s) == -1) return NULL;
        s++;
      }
    }
    break;
  }
  return s;
}

static const char *compile6(struct compiler *comp, const char *s)
{
  if(*s == '[') {
    if(s[1] == '^') {
      if(add_op(comp, RE_OP_BR_NOT) == -1) return NULL;
      s += 2;
    } else {
      if(add_op(comp, RE_OP_BR) == -1) return NULL;
      s++;
    }
    while(*s != ']') {
      if((s = compile7(comp, s, 1)) == NULL) return NULL;
    }
    if(add_op(comp, RE_OP_BR_END) == -1) return NULL;
    s++;
  } else {
    if((s = compile7(comp, s, 0)) == NULL) return NULL;
  }
  return s;
}

static const char *compile5(struct compiler *comp, const char *s)
{
  if((comp->cflags & REG_EXTENDED) != 0) {
    if(*s == '(') {
      size_t sub = comp->nsub;
      s++;
      if(add_op_with_size(comp, RE_OP_SAVE_MSO, sub) == -1) return NULL;
      comp->nsub++;
      comp->paren_level++;
      if((s = compile1(comp, s)) == NULL) return NULL;
      comp->paren_level--;
      if(add_op_with_size(comp, RE_OP_SAVE_MEO, sub) == -1) return NULL;
      if(*s != ')') {
        comp->errcode = REG_EPAREN;
        return NULL;
      }
      s++;
    } else {
      if((s = compile6(comp, s)) == NULL) return NULL;
    }
  } else {
    if(*s == '\\' && s[1] == '(') {
      size_t sub = comp->nsub;
      size_t sub1 = comp->nsub1;
      s += 2;
      if(add_op_with_size(comp, RE_OP_SAVE_MSO, sub) == -1) return NULL;
      comp->nsub++;
      if(comp->paren_level == 0) {
        if(add_op_with_size(comp, RE_OP_SAVE_S1SO, sub1) == -1) return NULL;
        comp->nsub1++;
      }
      comp->paren_level++;
      if((s = compile2(comp, s)) == NULL) return NULL;
      comp->paren_level--;
      if(add_op_with_size(comp, RE_OP_SAVE_MEO, sub) == -1) return NULL;
      if(comp->paren_level == 0) {
        if(add_op_with_size(comp, RE_OP_SAVE_S1EO, sub1) == -1) return NULL;
      }
      if(*s != '\\' || s[1] != ')') {
        comp->errcode = REG_EPAREN;
        return NULL;
      }
      s += 2;
    } else if(*s == '\\' && *s >= '1' && *s <= '9') {
      if(*s - '1' < comp->nsub1) {
        comp->errcode = REG_ESUBREG;
        return NULL;
      }
      if(add_op_with_char(comp, RE_OP_BACK_REF, *s - '1') == -1) return NULL;
      s += 2;
    } else {
      if((s = compile6(comp, s)) == NULL) return NULL;
    }
  }
  return s;
}

static const char *compile4(struct compiler *comp, const char *s)
{
  ssize_t ip = comp->bytecode_size;
  long m, n;
  if(add_op_with_addr(comp, RE_OP_NOP, 0) == -1) return NULL;
  if((s = compile5(comp, s)) == NULL) return NULL;
  if(*s == '*') {
    comp->bytecode[ip] = RE_OP_B;
    if(add_op_with_addr(comp, RE_OP_B, ip + 5) == -1) return NULL;
    if(comp_set_addr(comp, ip + 1, comp->bytecode_size, ip + 5) == -1) return NULL;
  } else {
    int is_dup = 0;
    if((comp->cflags & REG_EXTENDED) != 0) {
      switch(*s) {
      case '+':
        if(add_op_with_addr(comp, RE_OP_B, ip + 5) == -1) return NULL;
        s++;
        break;
      case '?':
        if(comp_set_addr(comp, ip + 1, comp->bytecode_size, ip + 5) == -1) return NULL;
        s++;
        break;
      case '{':
        is_dup = 1;
        s++;
        break;
      default:
        break;
      }
    } else {
      if(*s == '\\' && s[1] == '{') {
        is_dup = 1;
        s += 2;
      }
    }
    if(is_dup) {
      unsigned char *block;
      size_t block_size;
      long i;
      if((n = parse_long(s, (char **) (&s))) == -1L) {
        comp->errcode = REG_BADBR;
        return NULL;
      }
      if(*s == ',') {
        comp->errcode = REG_BADBR;
        return NULL;
      }
      s++;
      if(isdigit(*s)) {
        if((m = parse_long(s, (char **) (&s))) == -1L) {
          comp->errcode = REG_BADBR;
          return NULL;
        }
      } else {
        m = n;
      }
      if((comp->cflags & REG_EXTENDED) != 0) {
        if(*s != '}') {
          comp->errcode = REG_EBRACE;
          return NULL;
        }
        s++;
      } else {
        if(*s != '\\' || s[1] != '}') {
          comp->errcode = REG_EBRACE;
          return NULL;
        }
        s += 2;
      }
      if(n > m) {
        comp->errcode = REG_BADBR;
        return NULL;
      }
      block = comp->bytecode + ip;
      block_size = comp->bytecode_size - ip;
      for(i = 1; i < n; i++) {
        if(add_block(comp, block, block_size) == -1) return NULL;
      }
      if(n > 0) {
        ip += block_size * n;
        for(i = n; i < m; i++, ip += block_size) {
          comp->bytecode[ip] = RE_OP_BFJ;
          if(comp_set_addr(comp, ip + 1, comp->bytecode_size, ip + 5) == -1) return NULL;
        }
      } else {
        comp->bytecode[ip] = RE_OP_JUMP;
        if(comp_set_addr(comp, ip + 1, comp->bytecode_size, ip + 5) == -1) return NULL;
      }
    }
  }
  return s;
}

static const char *compile3(struct compiler *comp, const char *s)
{
  while(1) {
    if(*s == '$' || ((comp->cflags & REG_EXTENDED) != 0 && *s == '|') || *s == '\0') break;
    if((s = compile4(comp, s)) == NULL) return NULL;
  }
  return s;
}

static const char *compile2(struct compiler *comp, const char *s)
{
  if(*s == '^') {
    if(add_op(comp, RE_OP_SANCHOR) == -1) return NULL;
    s++;
  }
  if((s = compile3(comp, s)) == NULL) return NULL;
  if(*s == '$') {
    if(add_op(comp, RE_OP_EANCHOR) == -1) return NULL;
    s++;
  }
  return s;
}

static const char *compile1(struct compiler *comp, const char *s)
{
  ssize_t ip = comp->bytecode_size;
  while(1) {
    ssize_t tmp_ip = comp->bytecode_size;
    if(add_op_with_addr(comp, RE_OP_NOP, 0) == -1) return NULL;
    if((s = compile2(comp, s)) == NULL) return NULL;
    if(*s == '|') {
      if(add_op_with_addr(comp, RE_OP_JUMP, 0) == -1) return NULL;
    }
    if(comp_set_addr(comp, tmp_ip + 1, comp->bytecode_size, tmp_ip + 5) == -1) return NULL;
    if(*s != '|') break;
    s++;
  }
  while(1) {
    int diff = comp_get_int(comp, ip + 1);
    if(ip + 5 + diff >= comp->bytecode_size) break;
    comp->bytecode[ip] = RE_OP_B;
    ip += 5 + diff;
    if(comp_set_addr(comp, ip - 5, comp->bytecode_size, ip) == -1) return NULL;
  }
  return s;
}

static int compile_end(struct compiler *comp, const char *s)
{
  if(*s != '\0') {
    comp->errcode = REG_BADPAT;
    return -1;
  }
  if(add_op(comp, RE_OP_END) == -1) return -1;
  return 0;
}

static int compile_bre(struct compiler *comp, const char *s)
{
  if((s = compile2(comp, s)) == NULL) return -1;
  if(compile_end(comp, s) == -1) return -1;
  return 0;
}

static int compile_ere(struct compiler *comp, const char *s)
{
  if((s = compile1(comp, s)) == NULL) return -1;
  if(compile_end(comp, s) == -1) return -1;
  return 0;
}

int regcomp(regex_t *reg, const char *pattern, int cflags)
{
  struct compiler comp;
  int r;
  comp.cflags = cflags;
  comp.bytecode = NULL;
  comp.bytecode_size = 0;
  comp.bytecode_capacity = 0;
  comp.paren_level = 0;
  comp.nsub = 0;
  comp.nsub1 = 0;
  comp.errcode = 0;
  if((cflags & REG_EXTENDED) == 0) {
    r = compile_ere(&comp, pattern);
  } else {
    r = compile_bre(&comp, pattern);
  }
  if(r != -1) {
    reg->__bytecode = comp.bytecode;
    reg->re_nsub = comp.nsub;
    reg->__cflags = cflags;
    return 0;
  } else {
    return comp.errcode;
  }
}

struct stack_elem
{
  ssize_t ip;
  regoff_t o;
  int sub;
  int sub1;
};

struct interpreter
{
  short cflags;
  short eflags;
  unsigned char *bytecode;
  ssize_t ip;
  regoff_t o;
  int sub;
  int sub1;
  size_t sp;
  struct stack_elem *stack;
  size_t stack_capacity;
  regmatch_t *matches;
  size_t nmatches;
  regmatch_t subs1[9];
  signed char br_mode;
  signed char is_success;
  signed char is_stopped;
  signed char errcode;
};

static int interp_get_int(struct interpreter *interp, ssize_t i)
{ return (int) ((interp->bytecode[i] << 24) | (interp->bytecode[i + 1] << 16) | (interp->bytecode[i + 2] << 8) | interp->bytecode[i + 3]); }

static int get_char(struct interpreter *interp, const char *s)
{ return (interp->cflags & REG_ICASE) != 0 ? tolower(s[interp->o]) : s[interp->o]; }

static int push(struct interpreter *interp, ssize_t ip)
{
  if(interp->sp + 1 > interp->stack_capacity) {
    struct stack_elem *new_stack;
    interp->stack_capacity = ((interp->sp + 1 + STACK_STEP - 1) / STACK_STEP) * STACK_STEP;
    new_stack = realloc(interp->stack, interp->stack_capacity * sizeof(struct stack_elem));
    if(new_stack == NULL) {
      interp->errcode = REG_ESPACE;
      return -1;
    }
    interp->stack = new_stack;
  }
  interp->stack[interp->sp].ip = ip;
  interp->stack[interp->sp].o = interp->o;
  interp->stack[interp->sp].sub = interp->sub;
  interp->stack[interp->sp].sub1 = interp->sub1;
  interp->sp++;
  return 0;
}

static void match_success(struct interpreter *interp, const char *s)
{
  if(!interp->br_mode) {
    if((interp->cflags & REG_NEWLINE) != 0 && s[interp->o] == '\n') {
      interp->eflags &= ~REG_NOTBOL;
    } else {
      interp->eflags |= REG_NOTBOL;
    }
    interp->o++;
  } else {
    interp->is_success = 1;
  }
}

static void match_failure(struct interpreter *interp)
{
  if(!interp->br_mode) {
    if(interp->sp > 0) {
      size_t old_sub = interp->sub;
      size_t old_sub1 = interp->sub1;
      size_t i, n;
      interp->ip = interp->stack[interp->sp].ip;
      interp->o = interp->stack[interp->sp].o;
      interp->sub = interp->stack[interp->sp].sub;
      interp->sub1 = interp->stack[interp->sp].sub1;
      interp->sp--;
      if(interp->nmatches > 0) {
        n = (old_sub + 1 < interp->nmatches - 1 ? old_sub + 1 : interp->nmatches - 1);
        for(i = interp->sub + 1; i < n; i++) interp->matches[i + 1].re_so = interp->matches[i + 1].re_eo = -1;
      }
      n = (old_sub1 + 1 ? old_sub1 + 1 : 9);
      for(i = interp->sub1 + 1; i < n; i++) interp->subs1[i].re_so = interp->subs1[i].re_eo = -1;
    } else {
      interp->is_stopped = 1;
      interp->errcode = REG_NOTMATCH;
    }
  }
}

static int interpret_bytecode(struct interpreter *interp, const char *s)
{
  while(!interp->is_stopped) {
    ssize_t old_ip = interp->ip;
    int tmp, tmp2;
    switch(interp->bytecode[interp->ip]) {
    case RE_OP_END:
      interp->ip++;
      interp->is_stopped = 1;
      interp->errcode = 0;
      break;
    case RE_OP_ANY:
      interp->ip++;
      if(s[interp->o] != '\0') {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_MATCH:
      interp->ip += 2;
      if((unsigned char) get_char(interp, s) == interp->bytecode[old_ip + 1]) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_SANCHOR:
      interp->ip++;
      if((interp->eflags & REG_NOTBOL) != 0) match_failure(interp);
      break;
    case RE_OP_EANCHOR:
      interp->ip++;
      if((interp->cflags & REG_NEWLINE) != 0 && s[interp->o] == '\n') {
        match_success(interp, s);
      } else if((interp->eflags & REG_NOTEOL) != 0 || s[interp->o] != '\0' ) {
        match_failure(interp);
      }
    case RE_OP_BR:
      interp->ip++;
      interp->br_mode = 1;
      interp->is_success = 0;
      break;
    case RE_OP_BR_NOT:
      interp->ip++;
      interp->br_mode = 2;
      interp->is_success = 0;
      break;
    case RE_OP_BR_END:
      interp->ip++;
      if(interp->br_mode == 1 ? interp->is_success : (!interp->is_success && (interp->cflags & REG_NEWLINE) != 0 && s[interp->o] != '\n')) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      interp->br_mode = 0;
      break;
    case RE_OP_NOP:
      interp->ip += 5;
      break;
    case RE_OP_B:
      interp->ip += 5;
      if(push(interp, interp->ip + interp_get_int(interp, old_ip + 1)) == -1) return -1;
      break;
    case RE_OP_BFJ:
      interp->ip += 5;
      if(push(interp, interp->ip) == -1) return -1;
      interp->ip += interp_get_int(interp, old_ip + 1);
      break;
    case RE_OP_JUMP:
      interp->ip += 5;
      interp->ip += interp_get_int(interp, old_ip + 1);
      break;
    case RE_OP_SAVE_MSO:
      interp->ip += 5;
      tmp = interp_get_int(interp, old_ip + 1);
      if(tmp >= 0 && ((size_t) tmp + 1) < interp->nmatches) interp->matches[tmp].re_so = interp->o;
      interp->sub = (tmp > interp->sub ? tmp : interp->sub);
      break;
    case RE_OP_SAVE_MEO:
      interp->ip += 5;
      tmp = interp_get_int(interp, old_ip + 1);
      if(tmp >= 0 && ((size_t) tmp + 1) < interp->nmatches) interp->matches[tmp].re_eo = interp->o;
      interp->sub = (tmp > interp->sub ? tmp : interp->sub);
      break;
    case RE_OP_SAVE_S1SO:
      interp->ip += 5;
      tmp = interp_get_int(interp, old_ip + 1);
      if(tmp >= 0 && tmp < 9) interp->subs1[tmp].re_so = interp->o;
      interp->sub1 = (tmp > interp->sub1 ? (size_t) tmp : interp->sub1);
      break;
    case RE_OP_SAVE_S1EO:
      interp->ip += 5;
      tmp = interp_get_int(interp, old_ip + 1);
      if(tmp >= 0 && tmp < 9) interp->subs1[tmp].re_eo = interp->o;
      interp->sub1 = (tmp > interp->sub1 ? (size_t) tmp : interp->sub1);
      break;
    case RE_OP_BACK_REF:
      interp->ip += 2;
      tmp = interp->bytecode[old_ip + 1];
      if(tmp >= 0 && tmp < 9 && interp->subs1[tmp].re_so != -1 && interp->subs1[tmp].re_eo != -1) {
        if(strncmp(s + interp->o, s + interp->subs1[tmp].re_so, interp->subs1[tmp].re_eo - interp->subs1[tmp].re_so) == 0) {
            if(!interp->br_mode) {
              regoff_t i;
              for(i = 0; i < interp->subs1[tmp].re_eo - interp->subs1[tmp].re_so; i++, interp->o++) {
                if((interp->cflags & REG_NEWLINE) != 0 && s[interp->o] == '\n') {
                  interp->eflags &= ~REG_NOTBOL;
                } else {
                  interp->eflags |= REG_NOTBOL;
                }
              }
            } else {
              interp->is_success = 1;
            }
        } else {
          match_failure(interp);
        }
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_RANGE:
      interp->ip += 3;
      tmp = interp->bytecode[old_ip + 1];
      tmp2 = interp->bytecode[old_ip + 2];
      if(get_char(interp, s) >= tmp && get_char(interp, s) <= tmp2) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_ALNUM:
      interp->ip++;
      if(isalnum(get_char(interp, s))) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_BLANK:
      interp->ip++;
      if(get_char(interp, s) == ' ' || get_char(interp, s) == '\t') {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_CNTRL:
      interp->ip++;
      if(iscntrl(get_char(interp, s))) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_DIGIT:
      interp->ip++;
      if(isdigit(get_char(interp, s))) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_GRAPH:
      interp->ip++;
      if(isgraph(get_char(interp, s))) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_LOWER:
      interp->ip++;
      if(islower(get_char(interp, s))) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_PRINT:
      interp->ip++;
      if(isprint(get_char(interp, s))) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_PUNCT:
      interp->ip++;
      if(ispunct(get_char(interp, s))) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_SPACE:
      interp->ip++;
      if(isspace(get_char(interp, s))) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_UPPER:
      interp->ip++;
      if(isupper(get_char(interp, s))) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    case RE_OP_XDIGIT:
      interp->ip++;
      if(isxdigit(get_char(interp, s))) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    default:
      interp->ip++;
      if((unsigned char) get_char(interp, s) == interp->bytecode[old_ip]) {
        match_success(interp, s);
      } else {
        match_failure(interp);
      }
      break;
    }
  }
  return 0;
}

int regexec(const regex_t *reg, const char *s, size_t nmatches, regmatch_t matches[], int eflags)
{
  struct interpreter interp;
  regoff_t o = 0;
  size_t i;
  interp.cflags = reg->__cflags;
  interp.stack = NULL;
  interp.stack_capacity = 0;
  if((reg->__cflags & REG_NOSUB) == 0) {
    interp.matches = matches;
    interp.nmatches = nmatches;
  } else {
    interp.matches = NULL;
    interp.nmatches = 0;
  }
  o = 0;
  while(1) {
    interp.ip = 0;
    interp.o = o;
    interp.sub = -1;
    interp.sub1 = -1;
    interp.sp = 0;
    if(interp.nmatches > 0) {
      for(i = 0; i < interp.nmatches; i++) interp.matches[i].re_so = interp.matches[i].re_eo = -1;
    }
    for(i = 0; i < 9; i++) interp.subs1[i].re_so = interp.subs1[i].re_eo = -1;
    interp.br_mode = 0;
    interp.is_success = 0;
    interp.is_stopped = 0;
    interp.errcode = 0;
    if(interpret_bytecode(&interp, s) == -1) break;
    if(interp.errcode == 0) {
      if(interp.nmatches > 0) {
        interp.matches[0].re_so = o;
        interp.matches[0].re_eo = interp.o;
      }
      break;
    }
    if((interp.cflags & REG_NEWLINE) != 0 && s[o] == '\n') {
      eflags &= ~REG_NOTBOL;
    } else {
      eflags |= REG_NOTBOL;
    }
    if(s[i] == '\0') break;
    o++;
  }
  if(interp.stack != NULL) free(interp.stack);
  return interp.errcode;
}

size_t regerror(int errcode, const regex_t *reg, char *errbuf, size_t errbuf_size)
{
  static const char *errlist[16] = {
    "Success",
    "No match",
    "Invalid regular expression",
    "Invalid collation character",
    "Invalid character class name",
    "Trailing backslash",
    "Invalid back reference",
    "Unmatched [ or [^",
    "Unmatched ( or \\(",
    "Unmatched \\{",
    "Invalid content of \\{\\}",
    "Invalid range end",
    "Memory exhausted",
    "Premature end of regular expression",
    "Not implemented function",
    "Unknown error"
  };
  size_t len;
  if(errcode < 0 || errcode > 14) errcode = 15;
  len = strlen(errlist[errcode]);
  if(errbuf_size > 0) {
    size_t n = errbuf_size - 1;
    n = (len < n ? len : n);
    memcpy(errbuf, errlist[errcode], n);
    errbuf[n] = '\0';
  }
  return len + 1;
}

void regfree(regex_t *reg)
{ free(reg->__bytecode); }

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "9cc.h"

void error_tok(Token* token) {
  fprintf(stderr, "予期せぬトークンです: %s\n", token->input);
  exit(1);
}

void error(char *err) {
  fprintf(stderr, "%s", err);
  exit(1);
}

Token *new_token(int ty, char *input) {
  Token* tok = malloc(sizeof(Token));
  tok->ty = ty;
  tok->input = input;
  return tok;
}

Token *new_token_val(int ty, char *input, int val) {
  Token* tok = malloc(sizeof(Token));
  tok->ty = ty;
  tok->input = input;
  tok->val = val;
  return tok;
}

void tokenize(char *p, Vector* tokens) {
  int i = 0;
  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+'
      || *p == '-'
      || *p == '/'
      || *p == '*'
      || *p == '('
      || *p == ')'
      || *p == '='
      || *p == ';'
      || *p == '!'
      || *p == ',') {
      vec_push(tokens, new_token(*p, p));
      i++;
      p++;
      continue;
    }

    if ('a' <= *p && *p <= 'z') {
      vec_push(tokens, new_token(TK_IDENT, p));
      i++;
      p++;
      continue;
    }

    if (isdigit(*p)) {
      int int_val = strtol(p, &p, 10);
      vec_push(tokens, new_token_val(TK_NUM, p, int_val));
      i++;
      continue;
    }

    fprintf(stderr, "トークナイズできません: %s\n", p);
    exit(1);
  }
  
  vec_push(tokens, new_token(TK_EOF, p));
}


#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
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

Token* new_token_ident(char* input, int len) {
  char *ident = malloc(sizeof(char) * len + 1);
  strncpy(ident, input, len);
  Token* tok = malloc(sizeof(Token));
  tok->ty = TK_IDENT;
  tok->input = ident;
  return tok;
}

Token *new_token_val(int ty, char *input, int val) {
  Token* tok = malloc(sizeof(Token));
  tok->ty = ty;
  tok->input = input;
  tok->val = val;
  return tok;
}

bool is_identifier_char(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || '_' == c;
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

    if (is_identifier_char(*p)) {
      int ident_size = 0;
      while (is_identifier_char(*(p + ident_size))) {
        ident_size++;
      }

      Token* ident_tok = new_token_ident(p, ident_size);
      vec_push(tokens, ident_tok);
      i += ident_size;
      p += ident_size;
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


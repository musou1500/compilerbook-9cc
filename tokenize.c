#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "9cc.h"

Vector* tokens;

Token *new_token(int ty, char *input) {
  Token* tok = malloc(sizeof(Token));
  tok->ty = ty;
  tok->input = input;
  return tok;
}

Token* new_token_ident(char* input, String *name) {
  Token* tok = malloc(sizeof(Token));
  tok->ty = TK_IDENT;
  tok->input = input;
  tok->name = name;
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

void tokenize(char *p) {
  tokens = new_vector();

  int i = 0;
  while (*p) {
    if (isspace(*p) || *p == '\n') {
      p++;
      continue;
    }

    if (*p == '+'
      || *p == '-'
      || *p == '/'
      || *p == '*'
      || *p == '('
      || *p == ')'
      || *p == '{'
      || *p == '}'
      || *p == '<'
      || *p == '>'
      || *p == '&'
      || *p == '|'
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
      String *ident = new_string();
      do {
        str_push(ident, *p);
        p++;
        i++;
      } while (is_identifier_char(*p));
      str_push(ident, '\0');

      Token* ident_tok = new_token_ident(p, ident);
      vec_push(tokens, ident_tok);
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


#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "9cc.h"

Vector* tokens;

char *keywords[] = {
  "if",
  "else",
  "while",
  "continue",
  "break",
  NULL
};

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

Token *new_token_keyword(char *input, int val) {
  Token* tok = malloc(sizeof(Token));
  tok->ty = TK_KEYWORD;
  tok->input = input;
  tok->val = val;
  return tok;
}

bool is_identifier_char(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || '_' == c;
}

void tokenize(char *p) {
  tokens = new_vector();

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
      p++;
      continue;
    }

    if ('a' <= *p && *p <= 'z') {
      String *ident = new_string();
      do {
        str_push(ident, *p);
        p++;
      } while (is_identifier_char(*p));
      str_push(ident, '\0');
      
      Token* tok = new_token_ident(p, ident);
      for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(ident->data, keywords[i]) == 0) {
          tok = new_token_keyword(p, i);
          break;
        }
      }

      vec_push(tokens, tok);
      continue;
    }

    if (isdigit(*p)) {
      int int_val = strtol(p, &p, 10);
      vec_push(tokens, new_token_val(TK_NUM, p, int_val));
      continue;
    }

    fprintf(stderr, "トークナイズできません: %s\n", p);
    exit(1);
  }
  
  vec_push(tokens, new_token(TK_EOF, p));
}


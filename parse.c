#include <stdlib.h>
#include <stdbool.h>
#include "9cc.h"

Node* new_node(int op, Node *lhs, Node *rhs) {
  Node *node = malloc(sizeof(Node));
  node->ty = op;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node* new_node_num(int val) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_NUM;
  node->val = val;
  return node;
}

Node* new_node_ident(char name) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_IDENT;
  node->name = name;
  return node;
}

Node *cmp(Parser* parser);
Node *mul(Parser* parser);
Node *expr(Parser* parser);
Node *term(Parser* parser);

bool match_ty(Parser* parser, int ty) {
  Token* cur_token = parser->tokens->data[parser->tok_pos];
  return cur_token->ty == ty;
}

Node *mul(Parser* parser) {
  Node *lhs = term(parser);
  
  Token* cur_token = parser->tokens->data[parser->tok_pos];
  if (cur_token->ty == TK_EOF) {
    return lhs;
  }

  if (cur_token->ty == '*') {
    parser->tok_pos++;
    return new_node('*', lhs, mul(parser));
  }

  if (cur_token->ty == '/') {
    parser->tok_pos++;
    return new_node('/', lhs, mul(parser));
  }

  return lhs;
}


Node* expr(Parser* parser) {
  Node *lhs = mul(parser);

  Token* cur_token = parser->tokens->data[parser->tok_pos];
  if (cur_token->ty == TK_EOF) {
    return lhs;
  }

  if (cur_token->ty == '+') {
    parser->tok_pos++;
    return new_node('+', lhs, expr(parser));
  }

  if (cur_token->ty == '-') {
    parser->tok_pos++;
    return new_node('-', lhs, expr(parser));
  }

  return lhs;
}

Node* cmp(Parser* parser) {
  Node *lhs = expr(parser);

  Token* cur_token = parser->tokens->data[parser->tok_pos];
  if (cur_token->ty == TK_EOF) {
    return lhs;
  }

  if (cur_token->ty == '!') {
    Token* next_token = parser->tokens->data[parser->tok_pos + 1];
    if (next_token->ty == '=') {
      parser->tok_pos += 2;
      return new_node(ND_NEQ, lhs, cmp(parser));
    }
  }

  if (cur_token->ty == '=') {
    Token* next_token = parser->tokens->data[parser->tok_pos + 1];
    if (next_token->ty == '=') {
      parser->tok_pos += 2;
      return new_node(ND_EQ, lhs, cmp(parser));
    }
  }

  return lhs;
}

Node *term(Parser* parser) {
  Token* cur_token = parser->tokens->data[parser->tok_pos];
  if (cur_token->ty == TK_NUM) {
    parser->tok_pos++;
    return new_node_num(cur_token->val);
  }

  if (cur_token->ty == TK_IDENT) {
    parser->tok_pos++;
    return new_node_ident(*cur_token->input);
  }

  if (cur_token->ty == '(') {
    parser->tok_pos++;
    Node *node = cmp(parser);

    cur_token = parser->tokens->data[parser->tok_pos];
    if (cur_token->ty != ')') {
      Token* unexpected_token = parser->tokens->data[parser->tok_pos];
      error_tok(unexpected_token);
    }

    parser->tok_pos++;
    return node;
  }

  Token* unexpected_token = parser->tokens->data[parser->tok_pos];
  error_tok(unexpected_token);
}

Node *assign(Parser *parser) {
  Node* lhs = cmp(parser);
  Token* cur_token = parser->tokens->data[parser->tok_pos];
  if (cur_token->ty == TK_EOF) {
    return lhs;
  }
  
  if (cur_token->ty == '=') {
    parser->tok_pos++;
    return new_node('=', lhs, assign(parser));
  }

  if (cur_token->ty == ';') {
    parser->tok_pos++;
    return lhs;
  }
}

Parser* new_parser(Vector* code, Vector* tokens) {
  Parser* parser = malloc(sizeof(Parser));
  parser->tokens = tokens;
  parser->code = code;
  parser->tok_pos = 0;
  return parser;
}

void program(Parser* parser) {
  vec_push(parser->code, assign(parser));

  Token* cur_token = parser->tokens->data[parser->tok_pos];
  if (cur_token->ty == TK_EOF) {
    return;
  }
  
  program(parser);
}


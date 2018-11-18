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

Node* new_node_ident(char* name) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_IDENT;
  node->name = name;
  return node;
}

Node* new_node_func_call(char* name, Vector* args) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_FUNC_CALL;
  node->name = name;
  node->args = args;
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
  if (match_ty(parser, TK_EOF)) {
    return lhs;
  }

  if (match_ty(parser, '*')) {
    parser->tok_pos++;
    return new_node('*', lhs, mul(parser));
  }

  if (match_ty(parser, '/')) {
    parser->tok_pos++;
    return new_node('/', lhs, mul(parser));
  }

  return lhs;
}


Node* expr(Parser* parser) {
  Node *lhs = mul(parser);

  if (match_ty(parser, TK_EOF)) {
    return lhs;
  }

  if (match_ty(parser, '+')) {
    parser->tok_pos++;
    return new_node('+', lhs, expr(parser));
  }

  if (match_ty(parser, '-')) {
    parser->tok_pos++;
    return new_node('-', lhs, expr(parser));
  }

  return lhs;
}

Node* cmp(Parser* parser) {
  Node *lhs = expr(parser);
  if (match_ty(parser, TK_EOF)) {
    return lhs;
  }

  if (match_ty(parser, '!')) {
    Token* next_token = parser->tokens->data[parser->tok_pos + 1];
    if (next_token->ty == '=') {
      parser->tok_pos += 2;
      return new_node(ND_NEQ, lhs, cmp(parser));
    }
  }

  if (match_ty(parser, '=')) {
    Token* next_token = parser->tokens->data[parser->tok_pos + 1];
    if (next_token->ty == '=') {
      parser->tok_pos += 2;
      return new_node(ND_EQ, lhs, cmp(parser));
    }
  }

  return lhs;
}

Vector* arglist(Parser* parser) {
  Vector* args = new_vector();
  vec_push(args, cmp(parser));
  while(match_ty(parser, ',')) {
    parser->tok_pos++;
    vec_push(args, cmp(parser));
  }
  return args;
}

Node *term(Parser* parser) {
  Token* cur_token = parser->tokens->data[parser->tok_pos];
  if (match_ty(parser, TK_NUM)) {
    parser->tok_pos++;
    return new_node_num(cur_token->val);
  }

  if (match_ty(parser, TK_IDENT)) {
    parser->tok_pos++;

    if (match_ty(parser, '(')) {
      parser->tok_pos++;
      Vector* args = arglist(parser);
      if (!match_ty(parser, ')')) {
        Token* unexpected_token = parser->tokens->data[parser->tok_pos];
        error_tok(unexpected_token);
      }

      parser->tok_pos++;
      return new_node_func_call(cur_token->input, args);
    } else {
      return new_node_ident(cur_token->input);
    }
  }

  if (match_ty(parser, '(')) {
    parser->tok_pos++;
    Node *node = cmp(parser);
    if (!match_ty(parser, ')')) {
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
  if (match_ty(parser, TK_EOF)) {
    return lhs;
  }
  
  if (match_ty(parser, '=')) {
    parser->tok_pos++;
    return new_node('=', lhs, assign(parser));
  }

  if (match_ty(parser, ';')) {
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
  if (match_ty(parser, TK_EOF)) {
    return;
  }
  
  program(parser);
}


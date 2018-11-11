#include <stdlib.h>
#include "9cc.h"

int pos = 0;
int code_pos = 0;
Node *code[100];

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

Node* new_node_ident(int name) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_IDENT;
  node->name = name;
  return node;
}

Node *mul();
Node *expr();
Node *term();

Node *mul() {
  Node *lhs = term();
  if (tokens[pos].ty == TK_EOF) {
    return lhs;
  }

  if (tokens[pos].ty == '*') {
    pos++;
    return new_node('*', lhs, mul());
  }

  if (tokens[pos].ty == '/') {
    pos++;
    return new_node('/', lhs, mul());
  }

  return lhs;
}


Node* expr() {
  Node *lhs = mul();
  if (tokens[pos].ty == TK_EOF) {
    return lhs;
  }

  if (tokens[pos].ty == '+') {
    pos++;
    return new_node('+', lhs, expr());
  }

  if (tokens[pos].ty == '-') {
    pos++;
    return new_node('-', lhs, expr());
  }

  return lhs;
}

Node *term() {
  if (tokens[pos].ty == TK_NUM) {
    return new_node_num(tokens[pos++].val);
  }

  if (tokens[pos].ty == TK_IDENT) {
    return new_node_ident(tokens[pos++].val);
  }

  if (tokens[pos].ty == '(') {
    pos++;
    Node *node = expr();
    if (tokens[pos].ty != ')') {
      error_tok(pos);
    }

    pos++;
    return node;
  }

  error_tok(pos);
}

Node *assign() {
  Node* lhs = expr();
  if (tokens[pos].ty == TK_EOF) {
    return lhs;
  }
  
  if (tokens[pos].ty == '=') {
    pos++;
    return new_node('=', lhs, assign());
  }

  if (tokens[pos].ty == ';') {
    pos++;
    return lhs;
  }
}

void program() {
  code[code_pos++] = assign();
  if (tokens[pos].ty == TK_EOF) {
    return;
  }
  
  program();
}

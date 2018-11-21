#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "9cc.h"

Vector* code;
int pos = 0;

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

void error() {
  Token *token = tokens->data[pos];
  fprintf(stderr, "予期せぬトークンです: %s\n", token->input);
  exit(1);
}

Node *cmp();
Node *mul();
Node *expr();
Node *term();

bool match_ty(int ty) {
  Token* cur_token = tokens->data[pos];
  return cur_token->ty == ty;
}

// mul: term mul'
// mul': * mul | / mul | ε
Node *mul() {
  Node *lhs = term();
  if (match_ty(TK_EOF)) {
    return lhs;
  }

  if (match_ty('*')) {
    pos++;
    return new_node('*', lhs, mul());
  }

  if (match_ty('/')) {
    pos++;
    return new_node('/', lhs, mul());
  }

  return lhs;
}

// expr: mul expr'
// expr': + expr expr' | - expr expr' | ε
Node* expr() {
  Node *lhs = mul();

  if (match_ty(TK_EOF)) {
    return lhs;
  }

  if (match_ty('+')) {
    pos++;
    return new_node('+', lhs, expr());
  }

  if (match_ty('-')) {
    pos++;
    return new_node('-', lhs, expr());
  }

  return lhs;
}


// cmp: expr cmp'
// cmp': != expr cmp' | == expr cmp' | ε
Node* cmp() {
  Node *lhs = expr();
  if (match_ty(TK_EOF)) {
    return lhs;
  }

  if (match_ty('!')) {
    Token* next_token = tokens->data[pos + 1];
    if (next_token->ty == '=') {
      pos += 2;
      return new_node(ND_NEQ, lhs, cmp());
    }
  }

  if (match_ty('=')) {
    Token* next_token = tokens->data[pos + 1];
    if (next_token->ty == '=') {
      pos += 2;
      return new_node(ND_EQ, lhs, cmp());
    }
  }

  return lhs;
}


// arglist: cmp arglist'
// arglist': "," arglist' | ε
Vector* arglist(Vector* args) {
  vec_push(args, cmp());
  if (match_ty(',')) {
    pos++;
    arglist(args);
  }

  return args;
}


// term: num | fncall_or_var | "(" cmp ")"
// fncall_or_var: ident "(" arglist ")" | ident
Node *term() {
  Token* cur_token = tokens->data[pos];
  if (match_ty(TK_NUM)) {
    pos++;
    return new_node_num(cur_token->val);
  }

  if (match_ty(TK_IDENT)) {
    pos++;

    if (match_ty('(')) {
      pos++;
      Vector* args = new_vector();
      arglist(args);
      if (!match_ty(')')) {
        error();
      }

      pos++;
      return new_node_func_call(cur_token->input, args);
    } else {
      return new_node_ident(cur_token->input);
    }
  }

  if (match_ty('(')) {
    pos++;
    Node *node = cmp();
    if (!match_ty(')')) {
      error();
    }

    pos++;
    return node;
  }

  error();
}

// assign: cmp assign' ";"
// assign': ε | "=" cmp assign'
Node *assign() {
  Node* lhs = cmp();
  if (match_ty(TK_EOF)) {
    return lhs;
  }
  
  if (match_ty('=')) {
    pos++;
    return new_node('=', lhs, assign());
  }

  if (match_ty(';')) {
    pos++;
    return lhs;
  }
}



// block_item_list: ε | stmt block_item_list'
void stmt();
void block_item_list() {
  stmt();
  if (!match_ty('}')) {
    block_item_list();
  }
}

// stmt: assign | "{" block_item_list "}"
void stmt() {
  if (match_ty('{')) {
    // compound statement
    pos++;

    block_item_list();
    if (match_ty('}')) {
      pos++;
      return;
    }

    error();
  } else {
    vec_push(code, assign());
  }
}

// program: stmt program'
// program': ε | program program'
void program() {
  stmt();
  if (match_ty(TK_EOF)) {
    return;
  }

  program();
}

void parse() {
  code = new_vector();
  program();
}

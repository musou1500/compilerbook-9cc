#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "9cc.h"

Vector* code;
Scope* global_scope;
int pos = 0;

Scope* new_scope() {
  Scope *scope = malloc(sizeof(Scope));
  scope->vars = new_map();
  scope->var_cnt = 0;
  return scope;
}

int scope_declare_var(Scope* scope, char *name) {
  void *var_idx = map_get(scope->vars, name);
  if (var_idx != NULL) {
    return (int)var_idx;
  }
  
  scope->var_cnt++;
  map_put(scope->vars, name, (void *)scope->var_cnt);
  return scope->var_cnt;
}

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

Node* new_node_if(Node* cond, Vector* stmts, Node* els) {
  Node *if_node = malloc(sizeof(Node));
  if_node->ty = ND_IF;
  if_node->cond = cond;
  if_node->stmts = stmts;
  if_node->els = els;
  return if_node;
}

Node* new_node_else(Vector* stmts) {
  Node *if_node = malloc(sizeof(Node));
  if_node->ty = ND_ELSE;
  if_node->stmts = stmts;
  return if_node;
}

Node* new_node_while(Node* cond, Vector* stmts) {
  Node *while_node = malloc(sizeof(Node));
  while_node->ty = ND_WHILE;
  while_node->cond = cond;
  while_node->stmts = stmts;
  return while_node;
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

bool match_keyword(char* keyword) {
  Token* cur_token = (Token *)tokens->data[pos];
  return match_ty(TK_IDENT) && strcmp(keyword, cur_token->input) == 0;
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
// cmp_ops: "!=" | "==" | "<=" | ">=" | ">" | "<"
// cmp': cmp_ops expr cmp' | ε
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
  
  if (match_ty('>')) {
    Token* next_token = tokens->data[pos + 1];
    if (next_token->ty == '=') {
      pos += 2;
      return new_node(ND_GTE, lhs, cmp());
    } else {
      pos++;
      return new_node(ND_GT, lhs, cmp());
    }
  }
  
  if (match_ty('<')) {
    Token* next_token = tokens->data[pos + 1];
    if (next_token->ty == '=') {
      pos += 2;
      return new_node(ND_LTE, lhs, cmp());
    } else {
      pos++;
      return new_node(ND_LT, lhs, cmp());
    }
  }

  return lhs;
}

// logical_and: cmp logical_and'
// logical_and': ε | "&&" logical
Node *logical();
Node* logical_and() {
  Node* lhs = cmp();
  if (match_ty('&')) {
    Token* next_token = tokens->data[pos + 1];
    if(next_token->ty == '&') {
      pos += 2;
      return new_node(ND_LOGICAL_AND, lhs, logical());
    }
  }

  return lhs;
}

// logical: logical_and logical'
// logical': ε | "||" logical_and
Node *logical() {
  Node* lhs = logical_and();
  if (match_ty('|')) {
    Token* next_token = tokens->data[pos + 1];
    if(next_token->ty == '|') {
      pos += 2;
      return new_node(ND_LOGICAL_OR, lhs, logical_and());
    }
  }

  return lhs;
}


// arglist: logical arglist'
// arglist': "," arglist' | ε
Vector* arglist(Vector* args) {
  vec_push(args, logical());
  if (match_ty(',')) {
    pos++;
    arglist(args);
  }

  return args;
}


// term: num | fncall_or_var | "(" logical ")"
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
    Node *node = logical();
    if (!match_ty(')')) {
      error();
    }

    pos++;
    return node;
  }

  error();
}

// assign: logical assign' ";"
// assign': ε | "=" logical assign'
Node *assign() {
  Node* lhs = logical();
  if (match_ty(TK_EOF)) {
    return lhs;
  }
  
  if (match_ty('=')) {
    pos++;
    scope_declare_var(global_scope, lhs->name);
    return new_node('=', lhs, assign());
  }

  if (match_ty(';')) {
    pos++;
    return lhs;
  }

  error();
}


// block_item_list: ε | stmt block_item_list'
void stmt();
void block_item_list(Vector* stmts) {
  stmt(stmts);
  if (!match_ty('}')) {
    block_item_list(stmts);
  }
}

// stmt: assign | "{" block_item_list "}" | if_stmt | while_stmt
Node* if_stmt();
Node* while_stmt();
void stmt(Vector* stmts) {
  if (match_keyword("if")) {
    pos++;
    vec_push(stmts == NULL ? code : stmts, if_stmt());
  } else if (match_keyword("while")) {
    pos++;
    vec_push(stmts == NULL ? code : stmts, while_stmt());
  } else if (match_ty('{')) {
    // compound statement
    pos++;

    block_item_list(stmts);
    if (match_ty('}')) {
      pos++;
      return;
    }

    error();
  } else {
    vec_push(stmts == NULL ? code : stmts, assign());
  }
}

// if_stmt: "if" "(" logical ")" stmt elif
// elif: ε | "else" if_stmt | "else" stmt
Node* elif();
Node *if_stmt() {
  if (match_ty('(')) {
    pos++;
    Node* cond = logical();
    if (!match_ty(')')) {
      error();
    }

    pos++;
    Vector* stmts = new_vector();
    stmt(stmts);
    if (match_keyword("else")) {
      pos++;
      return new_node_if(cond, stmts, elif());
    } else {
      return new_node_if(cond, stmts, NULL);
    }
  }

  error();
}

Node* elif() {
  if (match_keyword("if")) {
    pos++;
    return if_stmt();
  } else {
    Vector* stmts = new_vector();
    stmt(stmts);
    return new_node_else(stmts);
  }
}

Node* while_stmt() {
  if (match_ty('(')) {
    pos++;
    Node* cond = logical();
    if (!match_ty(')')) {
      error();
    }

    pos++;
    Vector* stmts = new_vector();
    stmt(stmts);
    return new_node_while(cond, stmts);
  }

  error();
}

// program: stmt program'
// program': ε | program program'
void program() {
  stmt(NULL);
  if (match_ty(TK_EOF)) {
    return;
  }

  program();
}

void parse() {
  code = new_vector();
  global_scope = new_scope();
  program();
}

#ifndef _9CC_H
#define _9CC_H
enum {
  TK_NUM = 256,
  TK_IDENT,
  TK_NEGATE,
  TK_EOF,
};

typedef struct {
  int ty;
  int val;
  char *input;
} Token;

enum {
  ND_NUM = 256,     // 整数のノードの型
  ND_IDENT,
};

typedef struct Node {
  int ty;
  struct Node* lhs;
  struct Node* rhs;
  int val;
  char name;
} Node;


extern Token tokens[100];
extern Node *code[100];
extern int code_pos;

void tokenize(char *p);
void error(char *err);
void error_tok(int i);
void gen(Node *node);
void program();

#endif

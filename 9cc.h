#ifndef _9CC_H
#define _9CC_H
enum {
  TK_NUM = 256,
  TK_IDENT,
  TK_EOF,
};

typedef struct {
  int ty;
  int val;
  char *input;
} Token;

enum {
  ND_NUM = 256,     // 整数のノードの型
  ND_EQ,
  ND_NEQ,
  ND_LTE,
  ND_LT,
  ND_GTE,
  ND_GT,
  ND_LOGICAL_AND,
  ND_LOGICAL_OR,
  ND_IDENT,
  ND_FUNC_CALL,
};

typedef struct {
  void **data;
  int capacity;
  int len;
} Vector;

typedef struct {
  Vector *keys;
  Vector *vals;
} Map;

typedef struct Node {
  int ty;
  struct Node* lhs;
  struct Node* rhs;
  int val;
  char *name;
  Vector* args;
} Node;

typedef struct {
  Map* vars;
  int var_cnt;
} Scope;

Vector* tokens;
Vector* code;
Scope* global_scope;

void tokenize(char *p);
void gen(Node *node);
void parse();

Vector *new_vector();
void vec_push(Vector *vec, void* elem);
Map *new_map();
void map_put(Map *map, char *key, void *val);
void *map_get(Map *map, char *key);
void runtest();

#endif

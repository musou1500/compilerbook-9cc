#include <stdio.h>
#include <stdlib.h>
#include "9cc.h"

void gen_lval(Node* node) {
  if (node->ty == ND_IDENT) {
    void *var_idx = map_get(global_scope->vars, node->name);
    if (var_idx == NULL) {
      fprintf(stderr, "%sは定義されていません", node->name);
      exit(1);
    }

    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", ((int)var_idx - 1) * 8);
    printf("  push rax\n");
    return;
  }

  fprintf(stderr, "代入の左辺値が変数ではありません");
  exit(1);
}

void gen(Node *node) {
  static int flag_num = 0;
  if (node->ty == ND_FUNC_CALL) {
    // 引数は整数の場合，%rdi，%rsi，%rdx，%rcx，%r8，%r9に置く．残りはスタックへ．
    char *arg_dests[6] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
    int arg_limit = 6;

    // 今のところ，引数の数を6までとし，それ以上は無視する
    int arg_len = (arg_limit > node->args->len) ? node->args->len : arg_limit;
    for (int i = 0; i < arg_len; i++) {
      Node* arg = node->args->data[i];
      gen(arg);
      printf("  pop %s\n", arg_dests[i]);
    }

    printf("  push r10\n");
    printf("  push r11\n");
    printf("  call %s\n", node->name);
    printf("  pop r10\n");
    printf("  pop r11\n");
    printf("  push rax\n");
    return;
  }

  if (node->ty == ND_NUM) {
    printf("  push %d\n", node->val);
    return;
  }

  if (node->ty == ND_IDENT) {
    gen_lval(node);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;
  }

  if (node->ty == '=') {
    gen_lval(node->lhs);
    gen(node->rhs);
    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi \n");
    printf("  push rdi\n");
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->ty) {
    case '+':
      printf("  add rax, rdi\n");
      break;
    case '-':
      printf("  sub rax, rdi\n");
      break;
    case '*':
      printf("  mul rdi\n");
      break;
    case '/':
      printf("  mov rdx, 0\n");
      printf("  div rdi\n");
      break;
    case ND_EQ:
      printf("  cmp rdi, rax\n");
      printf("  sete al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_NEQ:
      printf("  cmp rdi, rax\n");
      printf("  setne al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LT:
      printf("  cmp rdi, rax\n");
      printf("  setg al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LTE:
      printf("  cmp rdi, rax\n");
      printf("  setge al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_GT:
      printf("  cmp rdi, rax\n");
      printf("  setl al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_GTE:
      printf("  cmp rdi, rax\n");
      printf("  setle al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LOGICAL_AND:
      flag_num++;
      printf("  cmp rax, 0\n");
      printf("  je L%d\n", flag_num);
      printf("  cmp rdi, 0\n");
      printf("  je L%d\n", flag_num);
      printf("  jmp L%d\n", flag_num + 1);

      // if false
      printf("L%d:\n", flag_num++);
      printf("  mov rax, 0\n");
      printf("  jmp L%d\n", flag_num + 1);

      // if true
      printf("L%d:\n", flag_num++);
      printf("  mov rax, 1\n");
      printf("  jmp L%d\n", flag_num);

      // return
      printf("L%d:\n", flag_num++);
      break;
    case ND_LOGICAL_OR:
      flag_num++;
      printf("  cmp rax, 1\n");
      printf("  je L%d\n", flag_num + 1);
      printf("  cmp rdi, 1\n");
      printf("  je L%d\n", flag_num + 1);
      printf("  mov rax, 0\n");
      printf("  jmp L%d\n", flag_num);

      // if false
      printf("L%d:\n", flag_num++);
      printf("  mov rax, 0\n");
      printf("  jmp L%d\n", flag_num + 1);

      // if true
      printf("L%d:\n", flag_num++);
      printf("  mov rax, 1\n");
      printf("  jmp L%d\n", flag_num);

      // return
      printf("L%d:\n", flag_num++);
      break;
  }

  printf("  push rax\n");
}


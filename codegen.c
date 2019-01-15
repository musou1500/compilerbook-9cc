#include <stdio.h>
#include <stdlib.h>
#include "9cc.h"

int stack_depth = 0;
void gen_push(char *reg) {
  printf("push %s\n", reg);
  stack_depth += 8;
}

void gen_pop(char *reg) {
  printf("pop %s\n", reg);
  stack_depth -= 8;
}

int label_num() {
  static int label_num = 0;
  return label_num++;
}

void gen_lval(Node* node) {
  if (node->ty == ND_IDENT) {
    void *var_idx = map_get(global_scope->vars, node->name->data);
    if (var_idx == NULL) {
      fprintf(stderr, "%sは定義されていません", node->name->data);
      exit(1);
    }

    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", ((int)var_idx - 1) * 8);
    gen_push("rax");
    return;
  }

  fprintf(stderr, "代入の左辺値が変数ではありません");
  exit(1);
}

void gen_if(Node* node) {
  switch(node->ty) {
    case ND_ELSE: {
      if (node->els != NULL) {
        fprintf(stderr, "elseの前にifがありません");
        exit(1);
      }

      for (int i = 0; i < node->stmts->len; i++) {
        gen((Node *)node->stmts->data[i]);
      }
      break;
    }
    case ND_IF: {
      int else_label = label_num();
      int end_label = label_num();
      gen(node->cond);
      gen_pop("rax");
      printf("  cmp rax, 0\n");
      printf("  je L%d\n", else_label);
      for (int i = 0; i < node->stmts->len; i++) {
        gen((Node *)node->stmts->data[i]);
      }
      printf("  jmp L%d\n", end_label);

      printf("L%d:\n", else_label);
      if (node->els != NULL) {
        gen_if(node->els);
      }
      printf("L%d:\n", end_label);
      break;
    }
  }
}

void gen_while(Node* node) {
  int start_label = label_num();
  int end_label = label_num();
  printf("L%d:\n", start_label);
  gen(node->cond);
  gen_pop("rax");
  printf("  cmp rax, 0\n");
  printf("  je L%d\n", end_label);
  for (int i = 0; i < node->stmts->len; i++) {
    gen((Node *)node->stmts->data[i]);
  }
  printf("  jmp L%d\n", start_label);
  printf("L%d:\n", end_label);
}

void gen(Node *node) {
  if (node->ty == ND_FUNC_CALL) {
    // 引数は整数の場合，%rdi，%rsi，%rdx，%rcx，%r8，%r9に置く．残りはスタックへ．
    char *arg_dests[6] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
    int arg_limit = 6;

    // 今のところ，引数の数を6までとし，それ以上は無視する
    int arg_len = (arg_limit > node->args->len) ? node->args->len : arg_limit;
    for (int i = arg_len - 1; i >= 0; i--) {
      Node* arg = node->args->data[i];
      gen(arg);
    }

    for (int i = 0; i < arg_len; i++) {
      gen_pop(arg_dests[i]);
    }

    gen_push("r10");
    gen_push("r11");
    printf("  call %s\n", node->name->data);
    gen_pop("r11");
    gen_pop("r10");
    gen_push("rax");
    return;
  }

  if (node->ty == ND_NUM) {
    char buf[64];
    sprintf(buf, "%d", node->val);
    gen_push(buf);
    return;
  }

  if (node->ty == ND_IDENT) {
    gen_lval(node);
    gen_pop("rax");
    printf("  mov rax, [rax]\n");
    gen_push("rax");
    return;
  }

  if (node->ty == '=') {
    gen_lval(node->lhs);
    gen(node->rhs);
    gen_pop("rdi");
    gen_pop("rax");
    printf("  mov [rax], rdi \n");
    gen_push("rdi");
    return;
  }

  if (node->ty == ND_IF) {
    gen_if(node);
    return;
  }

  if (node->ty == ND_WHILE) {
    gen_while(node);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  gen_pop("rdi");
  gen_pop("rax");

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
    case ND_LOGICAL_AND: {
      int false_label = label_num();
      int true_label = label_num();
      int end_label = label_num();

      printf("  cmp rax, 0\n");
      printf("  je L%d\n", false_label);
      printf("  cmp rdi, 0\n");
      printf("  je L%d\n", false_label);
      printf("  jmp L%d\n", true_label);

      // if false
      printf("L%d:\n", false_label);
      printf("  mov rax, 0\n");
      printf("  jmp L%d\n", end_label);

      // if true
      printf("L%d:\n", true_label);
      printf("  mov rax, 1\n");
      printf("  jmp L%d\n", end_label);

      // return
      printf("L%d:\n", end_label);
      break;
    }
    case ND_LOGICAL_OR: {
      int false_label = label_num();
      int true_label = label_num();
      int end_label = label_num();

      printf("  cmp rax, 1\n");
      printf("  je L%d\n", true_label);
      printf("  cmp rdi, 1\n");
      printf("  je L%d\n", true_label);
      printf("  mov rax, 0\n");
      printf("  jmp L%d\n", false_label);

      // if false
      printf("L%d:\n", false_label);
      printf("  mov rax, 0\n");
      printf("  jmp L%d\n", end_label);

      // if true
      printf("L%d:\n", true_label);
      printf("  mov rax, 1\n");
      printf("  jmp L%d\n", end_label);

      // return
      printf("L%d:\n", end_label);
      break;
    }
  }

  gen_push("rax");
}


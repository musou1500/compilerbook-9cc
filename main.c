#include <stdio.h>
#include <string.h>
#include "9cc.h"

int main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "引数の個数が正しくありません\n");
    return 1;
  }

  if (strcmp("-test", argv[1]) == 0) {
    runtest();
    return 0;
  }

  tokenize(argv[1]);

  parse();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // プロローグ
  // ローカル変数確保
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, %d\n", 8 * global_scope->var_cnt);

  for (int i = 0; i < code->len; i++) {
    gen(code->data[i]);
    printf("  pop rax\n");
  }
  
  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");
  printf("  ret\n");
  return 0;
}

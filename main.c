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
  program();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // プロローグ
  // 変数26個分の領域を確保する
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, 208\n");
  
  for (int i = 0; i < code_pos; i++) {
    gen(code[i]);
    printf("  pop rax\n");
  }
  
  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");
  printf("  ret\n");
  return 0;
}

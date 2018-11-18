#include <stdio.h>
#include <stdlib.h>
#include "./lib.h"

int add_int(int x, int y) {
  return x + y;
}

Data *new_data(int x, int y) {
  Data* d = malloc(sizeof(Data));
  d->x = x;
  d->y = y;
  return d;
}

int sum_data(Data* d) {
  return d->x + d->y;
}

#include "./9cc.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

Vector *new_vector() {
  Vector *vec = malloc(sizeof(Vector));
  vec->data = malloc(sizeof(void *) * 16);
  vec->capacity = 16;
  vec->len = 0;
  return vec;
}

void vec_push(Vector *vec, void* elem) {
  if (vec->capacity == vec->len) {
    vec->capacity *= 2;
    vec->data = realloc(vec->data, sizeof(void *) * vec->capacity);
  }

  vec->data[vec->len++] = elem;
}

String *new_string() {
  String *str = malloc(sizeof(String));
  str->data = malloc(sizeof(void *) * 16);
  str->capacity = 16;
  str->len = 0;
  return str;
}

void str_push(String *str, char c) {
  if (str->capacity == str->len) {
    str->capacity *= 2;
    str->data = realloc(str->data, sizeof(char) * str->capacity);
  }

  str->data[str->len++] = c;
}

Map *new_map() {
  Map *map = malloc(sizeof(Map));
  map->keys = new_vector();
  map->vals = new_vector();
  return map;
}

void map_put(Map *map, char *key, void *val) {
  vec_push(map->keys, key);
  vec_push(map->vals, val);
}

void* map_get(Map *map, char *key) {
  for (int i = map->keys->len - 1; i >= 0; i--)
    if (strcmp(map->keys->data[i], key) == 0)
      return map->vals->data[i];
  return NULL;
}

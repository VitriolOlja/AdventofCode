#ifndef SPRINGHASH_H
#define SPRINGHASH_H
#include <stdint.h>
#include <stdbool.h>
#include "stringline.h"

typedef struct SpringHashNode {
  StringLine str;
  uint8_t seen : 1;
} SpringHashNode;

typedef struct SpringHash {
  SpringHashNode *data;
  uint32_t capacity;
} SpringHash;

void initialize_hash(SpringHash* hash);

void hash_add(SpringHash* hash, StringLine *str);

bool hash_find(SpringHash* hash, StringLine *str);

void hash_free(SpringHash *hash);





#endif /* SPRINGHASH_H */
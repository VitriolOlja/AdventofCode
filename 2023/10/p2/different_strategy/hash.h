#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct HashNode {
  int32_t x;
  int32_t y;
  uint8_t used;
} HashNode;

typedef struct HashSet {
  uint32_t capacity;
  uint8_t shift;
  HashNode *data;
} HashSet;


void initialize_hashset(HashSet *h, uint8_t cap);

void hashset_add(HashSet *h, int32_t x, int32_t y);

bool hashset_is_in(HashSet* h, int32_t x, int32_t y);


#endif /* HASH_H */
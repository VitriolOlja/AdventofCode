#include "hash.h"

//cap is the power of two size. 
//so entering 0 means size 1, 1 means size 2, 2 means size 4, 3 means size 8, 4 means size 16

static inline uint64_t key(int32_t x, int32_t y) {
  return ((uint64_t)(uint32_t)x << 32) | (uint32_t)y;
}

static inline uint32_t index_for_key(HashSet *h, uint64_t key) {
  //https://probablydance.com/2018/06/16/fibonacci-hashing-the-optimization-that-the-world-forgot-or-a-better-alternative-to-integer-modulo/
    uint64_t hash = key * 11400714819323198485ULL;
    return (uint32_t)((hash >> (64 - (32 - h->shift))) & (h->capacity - 1));
}

void initialize_hashset(HashSet *h, uint8_t cap)
{
  h->data = calloc((1 << cap), sizeof(HashNode));

  h->shift = (32 - cap);
  h->capacity = (1 << cap);
}

void hashset_add(HashSet *h, int32_t x, int32_t y)
{
  uint32_t index = index_for_key(h, key(x, y));

  while(h->data[index].used) {
    if (h->data[index].x == x && h->data[index].y == y) {
      //already added to hashtable so just return. 
      return;
    }
    index = (index + 1) & (h->capacity - 1);  //linear probing. 
  }

  h->data[index].x = x;
  h->data[index].y = y;
  h->data[index].used = 1;
}

bool hashset_is_in(HashSet *h, int32_t x, int32_t y)
{
  uint32_t index = index_for_key(h, key(x, y));

  while(h->data[index].used) {
    if (h->data[index].x == x && h->data[index].y == y) {
      //already added to hashtable so just return. 
      return true;
    }
    index = (index + 1) & (h->capacity - 1);  //linear probing. 
  }

  //did not find it. 
  return false;
}

void free_hashset(HashSet *h) {
  free(h->data);
}

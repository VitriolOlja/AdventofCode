#include "springhash.h"
#include <stdlib.h>
#include <stdio.h>


static uint32_t CAPACITY = 65536;

void initialize_hash(SpringHash *hash)
{
  hash->data = calloc(CAPACITY, sizeof(SpringHashNode));

  if (!hash->data) {
    printf("Error: could not calloc for hashset.");
  }
  hash->capacity = CAPACITY;
}

static uint16_t get_index(StringLine *str) {
  uint16_t sum = 0;
  uint16_t power = 1;
  for (uint32_t i = 0; i < str->current; i++) {
    sum = (uint16_t)(sum + (uint32_t)(uint8_t)str->data[i] * (uint32_t)power);
    power *= 3;
  }

  return sum;
}

void hash_add(SpringHash *hash, StringLine *str)
{
  // multiply all together. Since size of hashtable is 65536 we can just sum it inside a 32 bit var. 
  uint16_t index = get_index(str);

  //linear probing. 
  while(hash->data[index].seen) {
    index = (uint16_t)(index + 1);
  }

  hash->data[index].str = *str;
  hash->data[index].seen = 1;
}

bool hash_find(SpringHash *hash, StringLine *str)
{
  // multiply all together. Since size of hashtable is 65536 we can just sum it inside a 32 bit var. 
  uint16_t index = get_index(str);

  //linear probing. 
  while(hash->data[index].seen) {
    if (str_line_cmp(&hash->data[index].str, str)) {
      return true;
    }
    index = (uint16_t)(index + 1);
  }

  return false;
}

void hash_free(SpringHash *hash)
{
  free(hash->data);
}

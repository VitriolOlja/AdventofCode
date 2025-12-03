#ifndef NUMARRAY_H
#define NUMARRAY_H


#include "stdint.h"



#define NUM_ARRAY_TYPE uint32_t
typedef struct NumArray {
  NUM_ARRAY_TYPE *data;
  uint32_t capacity;
  uint32_t current;
} NumArray;



#endif /*NUMARRAY_H*/
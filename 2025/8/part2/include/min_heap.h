#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <stdint.h>
#include <stdlib.h>

/**
 * a max heap of max size k elements. 
 * if we addd more elements -> it pops the top (the max) and adds a new
 */

typedef struct MinHeapNode {
  uint64_t distance;
  size_t index_a, index_b;
} MinHeapNode;

typedef struct MinHeap {
  MinHeapNode* data;
  size_t num_elements;
  size_t capacity;
} MinHeap;


void initialize_min_heap(MinHeap *heap, size_t initial_size);


uint8_t min_heap_empty(MinHeap *heap);

//empty check this first :)
MinHeapNode min_heap_pop(MinHeap *heap);

//parent = (k - 1) / 2
//left child = k * 2 + 1
// right child = k * 2 + 2
void add_to_min_heap(MinHeap *heap, uint64_t distance, size_t index_a, size_t index_b);

void free_min_heap(MinHeap *heap);


#endif /*MINHEAP_H*/
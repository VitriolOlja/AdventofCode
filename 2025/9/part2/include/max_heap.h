#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include <stdint.h>
#include <stdlib.h>

/**
 * a max heap of max size k elements. 
 * if we addd more elements -> it pops the top (the max) and adds a new
 */

typedef struct MaxHeapNode {
  uint64_t area; 
  size_t index_a, index_b;
} MaxHeapNode;

typedef struct MaxHeap {
  MaxHeapNode* data;
  size_t num_elements;
  size_t capacity;
} MaxHeap;


void initialize_max_heap(MaxHeap *heap, size_t initial_size);

//parent = (k - 1) / 2
//left child = k * 2 + 1
// right child = k * 2 + 2
void add_to_max_heap(MaxHeap *heap, uint64_t area, size_t index_a, size_t index_b);

void free_max_heap(MaxHeap *heap);

uint8_t is_empty(MaxHeap *heap);

MaxHeapNode max_heap_pop(MaxHeap *heap);


#endif /*MAXHEAP_H*/
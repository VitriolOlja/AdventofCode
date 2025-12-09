#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <stdint.h>
#include <stdlib.h>

/**
 * a max heap of max size k elements. 
 * if we addd more elements -> it pops the top (the max) and adds a new
 */

typedef struct MinHeapNode {
  size_t component_size; 
} MinHeapNode;

typedef struct MinHeap {
  MinHeapNode* data;
  size_t num_elements;
  size_t capacity;
} MinHeap;


void initialize_min_heap(MinHeap *heap, size_t initial_size);

//parent = (k - 1) / 2
//left child = k * 2 + 1
// right child = k * 2 + 2
void add_to_min_heap(MinHeap *heap, size_t component_size);

void free_min_heap(MinHeap *heap);


#endif /*MINHEAP_H*/
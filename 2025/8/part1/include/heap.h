#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stdlib.h>

/**
 * a max heap of max size k elements. 
 * if we addd more elements -> it pops the top (the max) and adds a new
 */

typedef struct HeapNode {
  uint64_t distance;
  size_t index_a, index_b;
} HeapNode;

typedef struct Heap {
  HeapNode* data;
  size_t num_elements;
  size_t capacity;
} Heap;


void initialize_heap(Heap *heap, size_t initial_size);

void node_switch(HeapNode* a, HeapNode *b);

//parent = (k - 1) / 2
//left child = k * 2 + 1
// right child = k * 2 + 2
void add_to_heap(Heap *heap, uint64_t distance, size_t index_a, size_t index_b);

void free_heap(Heap *heap);


 #endif /*HEAP_H*/
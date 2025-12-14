#include "heap.h"


void initialize_heap(Heap *heap, size_t initial_size) {
  heap->data = malloc(initial_size * sizeof(HeapNode));

  heap->capacity = initial_size;
  heap->num_elements = 0;
}

void node_switch(HeapNode* a, HeapNode *b) {
  uint64_t temp = a->distance;
  size_t temp_index_a = a->index_a;
  size_t temp_index_b = a->index_b;
  
  a->distance = b->distance;
  a->index_a = b->index_a;
  a->index_b = b->index_b;
  b->distance = temp;
  b->index_a = temp_index_a;
  b->index_b = temp_index_b;
}

//parent = (k - 1) / 2
//left child = k * 2 + 1
// right child = k * 2 + 2
void heap_bubble(Heap *heap, size_t heap_index) {
  while(heap_index > 0) {
    size_t parent = (heap_index - 1) / 2;
    if (heap->data[heap_index].distance > heap->data[parent].distance) {
      //move it up. switch them. 
      node_switch(&heap->data[heap_index], &heap->data[parent]);
      heap_index = parent;
    }
    else {break;}
  }
}

void heap_sink(Heap *heap, size_t heap_index) {
  while(heap_index < heap->num_elements) {
    size_t left_child = heap_index * 2 + 1;
    size_t right_child = heap_index * 2 + 2;

    //find the largest of the two to switch out with and continue sinking. 
    size_t largest_index = heap_index; // Start by assuming the current node is the largest

    // 1. Check if the LEFT child exists and is larger than the current largest
    if (left_child < heap->num_elements && 
        heap->data[left_child].distance > heap->data[largest_index].distance) {
        largest_index = left_child;
    }
    if (right_child < heap->num_elements && 
            heap->data[right_child].distance > heap->data[largest_index].distance) {
            largest_index = right_child;
        }


    //if the largest is greater than this, we need to sink further. 
    if (largest_index != heap_index) {
      node_switch(&heap->data[largest_index], &heap->data[heap_index]);
      heap_index = largest_index;
    }
    else {
      break;
    }
  }
}

void add_to_heap(Heap *heap, uint64_t distance, size_t index_a, size_t index_b) {
  if (heap->num_elements < heap->capacity) {
    //add as a leaf -> bubble up
    heap->data[heap->num_elements].distance = distance;
    heap->data[heap->num_elements].index_a = index_a;
    heap->data[heap->num_elements].index_b = index_b;
    heap_bubble(heap, heap->num_elements);
    heap->num_elements++;
  }
  else {
    //remove top, can i put the new element on top and sink it? 
    //or do i have to remove top, put leaf there and sink and bubble both respectively? 
    // i think it should be fine to do it this way. 
    heap->data[0].distance = distance;
    heap->data[0].index_a = index_a;
    heap->data[0].index_b = index_b;
    heap_sink(heap, 0);
  }
}

void free_heap(Heap *heap) {
  free(heap->data);
  heap->capacity = 0;
}


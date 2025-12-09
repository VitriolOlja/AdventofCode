#include "min_heap.h"

void initialize_min_heap(MinHeap *heap, size_t initial_size) {
  heap->data = malloc(initial_size * sizeof(MinHeapNode));

  heap->capacity = initial_size;
  heap->num_elements = 0;
}

void min_node_switch(MinHeapNode* a, MinHeapNode *b) {
  uint64_t temp = a->component_size;
  
  a->component_size = b->component_size;
  b->component_size = temp;
}

//parent = (k - 1) / 2
//left child = k * 2 + 1
// right child = k * 2 + 2
static void heap_bubble(MinHeap *heap, size_t heap_index) {
  while(heap_index > 0) {
    size_t parent = (heap_index - 1) / 2;
    if (heap->data[heap_index].component_size < heap->data[parent].component_size) {
      //move it up. switch them. 
      min_node_switch(&heap->data[heap_index], &heap->data[parent]);
      heap_index = parent;
    }
    else {break;}
  }
}

static void heap_sink(MinHeap *heap, size_t heap_index) {
  while(heap_index < heap->num_elements) {
    size_t left_child = heap_index * 2 + 1;
    size_t right_child = heap_index * 2 + 2;

    //find the smallest of the two to switch out with and continue sinking. 
    size_t smallest_index = heap_index; // Start by assuming the current node is the largest

    // 1. Check if the LEFT child exists and is smaller than the current largest
    if (left_child < heap->num_elements && 
        heap->data[left_child].component_size < heap->data[smallest_index].component_size) {
        smallest_index = left_child;
    }
    if (right_child < heap->num_elements && 
            heap->data[right_child].component_size < heap->data[smallest_index].component_size) {
            smallest_index = right_child;
        }


    //if the largest is greater than this, we need to sink further. 
    if (smallest_index != heap_index) {
      min_node_switch(&heap->data[smallest_index], &heap->data[heap_index]);
      heap_index = smallest_index;
    }
    else {
      break;
    }
  }
}

void add_to_min_heap(MinHeap *heap, size_t component_size) {
  if (heap->num_elements < heap->capacity) {
    //add as a leaf -> bubble up
    heap->data[heap->num_elements].component_size = component_size;
    heap_bubble(heap, heap->num_elements);
    heap->num_elements++;
  }
  else {
    //remove top, can i put the new element on top and sink it? 
    //or do i have to remove top, put leaf there and sink and bubble both respectively? 
    // i think it should be fine to do it this way. 
    heap->data[0].component_size = component_size;
    heap_sink(heap, 0);
  }
}

void free_min_heap(MinHeap *heap) {
  free(heap->data);
  heap->capacity = 0;
}


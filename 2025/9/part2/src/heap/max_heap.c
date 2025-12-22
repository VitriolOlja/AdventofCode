#include "max_heap.h"

void initialize_max_heap(MaxHeap *heap, size_t initial_size) {
  heap->data = malloc(initial_size * sizeof(MaxHeapNode));

  heap->capacity = initial_size;
  heap->num_elements = 0;
}

void max_node_switch(MaxHeapNode* a, MaxHeapNode *b) {
  uint64_t temp = a->area;
  size_t temp_index_a = a->index_a;
  size_t temp_index_b = a->index_b;

  a->area = b->area;
  a->index_a = b->index_a;
  a->index_b = b->index_b;
  b->area = temp;
  b->index_a = temp_index_a;
  b->index_b = temp_index_b;
}

//parent = (k - 1) / 2
//left child = k * 2 + 1
// right child = k * 2 + 2
static void heap_bubble(MaxHeap *heap, size_t heap_index) {
  while(heap_index > 0) {
    size_t parent = (heap_index - 1) / 2;
    if (heap->data[heap_index].area > heap->data[parent].area) {
      //move it up. switch them. 
      max_node_switch(&heap->data[heap_index], &heap->data[parent]);
      heap_index = parent;
    }
    else {break;}
  }
}

static void heap_sink(MaxHeap *heap, size_t heap_index) {
  while(heap_index < heap->num_elements) {
    size_t left_child = heap_index * 2 + 1;
    size_t right_child = heap_index * 2 + 2;

    //find the smallest of the two to switch out with and continue sinking. 
    size_t largest_index = heap_index; // Start by assuming the current node is the largest

    // 1. Check if the LEFT child exists and is smaller than the current largest
    if (left_child < heap->num_elements && 
        heap->data[left_child].area > heap->data[largest_index].area) {
        largest_index = left_child;
    }
    if (right_child < heap->num_elements && 
        heap->data[right_child].area > heap->data[largest_index].area) {
        largest_index = right_child;
    }


    //if the largest is greater than this, we need to sink further. 
    if (largest_index != heap_index) {
      max_node_switch(&heap->data[largest_index], &heap->data[heap_index]);
      heap_index = largest_index;
    }
    else {
      break;
    }
  }
}

void add_to_max_heap(MaxHeap *heap, uint64_t area, size_t index_a, size_t index_b) {
  if (heap->num_elements < heap->capacity) {
    //add as a leaf -> bubble up
    heap->data[heap->num_elements].area = area;
    heap->data[heap->num_elements].index_a = index_a;
    heap->data[heap->num_elements].index_b = index_b;
    
    heap_bubble(heap, heap->num_elements);
    heap->num_elements++;
  }
  else {
    //remove top, can i put the new element on top and sink it? 
    //or do i have to remove top, put leaf there and sink and bubble both respectively? 
    // i think it should be fine to do it this way. 
    heap->data[0].area = area;
    heap->data[0].index_a = index_a;
    heap->data[0].index_b = index_b;
    heap_sink(heap, 0);
  }
}

uint8_t is_empty(MaxHeap *heap) {
  return heap->num_elements == 0;
}

MaxHeapNode max_heap_pop(MaxHeap *heap) {
  MaxHeapNode temp = {
                      .area = heap->data[0].area, 
                      .index_a = heap->data[0].index_a, 
                      .index_b = heap->data[0].index_b
                    };

  max_node_switch(&heap->data[0], &heap->data[heap->num_elements - 1]);
  heap_sink(heap, 0);
  heap->num_elements--;
  return temp;
}

void free_max_heap(MaxHeap *heap) {
  free(heap->data);
  heap->capacity = 0;
}


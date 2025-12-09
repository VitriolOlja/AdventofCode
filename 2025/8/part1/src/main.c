#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "vertex.h"
#include "heap.h"
#include "min_heap.h"
// can i sort based on x, 
// and sort based on y, and sort based on z? 
// that way i can find the closest on x y and z individually in log speed
// with a binary search. 

// dijkstra/MST? could it help us? 
// can we construct a distance matrix ? in 3d? 
//closest point is then a linear search. 

// okay we need a kd-tree. 

//it seems we can construct such a tree that is balanced using a sorted lists on x and y and z
// as seen here https://arxiv.org/html/1410.5420v41
// we need to calculate the median for each subdivision to split it as efficiently as possible. 


//or maybe well just try a bruteforc.e 

//printf("alignment of uint32_t: %zu\n", __alignof__(uint32_t)); = 4
//printf("alignment of Vertex:   %zu\n", __alignof__(Vertex)); = 4


uint64_t squared_distance(Vertex *a, Vertex *b) {
  int64_t diff_x = ((int64_t)a->x - (int64_t)b->x);
  int64_t diff_y = ((int64_t)a->y - (int64_t)b->y);
  int64_t diff_z = ((int64_t)a->z - (int64_t)b->z);

  return (uint64_t) diff_x * diff_x + diff_y *diff_y + diff_z * diff_z;
}

typedef struct MemoryArena {
  uint8_t *base;
  size_t size;
  size_t used;
} MemoryArena;

MemoryArena arena_create(size_t size) {
  MemoryArena a;
  a.base = malloc(size);
  a.size = size;
  a.used = 0;
  return a;
}

void *arena_alloc(MemoryArena *a, size_t amount, size_t alignment) {
  size_t mask = alignment - 1;
  size_t current = a->used;
  size_t aligned = (current + mask) & ~mask;

  if (aligned + amount > a->size) {
    return NULL;
  }

  void *ptr = a->base + aligned;
  a->used = aligned + amount;

  return ptr;
}

void arena_reset(MemoryArena *a) {
    a->used = 0;
  }

void arena_free(MemoryArena *a) {
  free(a->base);
  a->size = 0;
}


size_t find_root(size_t* parent_array, size_t i) {
  //if element is its own parent its the root. 
  if (parent_array[i] == i) {
        return i;
  }
  //else we find recursively the root on the parent. 
  parent_array[i] = find_root(parent_array, parent_array[i]);
    
  return parent_array[i];
}

void unite_sets(size_t *parent_array, size_t *size_array, size_t a, size_t b) {
  

  size_t root_a = find_root(parent_array, a);
  size_t root_b = find_root(parent_array, b);
  if (root_a == root_b) {
        return;
  }
  if (size_array[root_a] < size_array[root_b]) {
      // Swap to ensure root_a is the larger/new root (or equal)
      size_t temp_root = root_a;
      root_a = root_b;
      root_b = temp_root;
  }

  parent_array[root_b] = root_a;
  size_array[root_a] += size_array[root_b];
}


int main() {
  FILE* fptr = fopen("data/input.txt", "r");

  //lets read how many rows there are
  int ch, last_ch;
  size_t row_count = 0;
  while((ch = fgetc(fptr)) != EOF) {
    if (ch == '\n') {
      row_count++;
    }
    last_ch = ch;
  }

  if (last_ch != '\n'){
    row_count++;  
  }
  //printf("row: %zu", row_count);

  size_t required_memory = sizeof(Vertex) * row_count + //initial list. 
    sizeof(uint64_t*) * row_count + sizeof(uint64_t) * row_count * row_count //distance matrix. 
    + row_count * sizeof(size_t) //componentsizes (union find)
    + row_count * sizeof(size_t); // union find. 
  MemoryArena arena = arena_create(required_memory);

  Vertex* v = arena_alloc(&arena, sizeof(Vertex) * row_count, 4);

  fseek(fptr, 0, SEEK_SET);

  uint32_t current_number = 0;
  uint32_t current_row = 0;
  uint8_t current_column = 0;

  while((ch = fgetc(fptr)) != EOF) {
    if (ch == '\n') {
      //save number
      v[current_row].z = current_number;
      current_row++;
      current_number = 0;
      current_column = 0;
    }
    else if (ch == ',') {
      //save number
      if (current_column == 0) {
        v[current_row].x = current_number;
        current_number = 0;
        current_column++;
      }
      else{
        v[current_row].y = current_number;
        current_number = 0;
        current_column++;
      }
    }
    else if ('0' <= ch && ch <= '9') {
      current_number = current_number * 10 + (ch - '0');
    }
    last_ch = ch;
  }
  if (last_ch != '\n'){
     v[current_row].z = current_number;
  }
  

  //for(size_t i = 0; i < row_count; i++) {
  //  printf("%u - %u - %u\n", v[i].x, v[i].y, v[i].z);
  //}

  //maybe i make a heap - of size 10 where I keep the distances. and the nodes. 
  // a max heap -> and if it is bigger than the root we pop the top and add our new
  // and fix the heap? 



  //create a distance matrix.
  //uint64_t** distance_matrix = arena_alloc(&arena, sizeof(uint64_t*) * row_count, 4);
  //for(size_t i = 0; i < row_count; i++) {
  //  distance_matrix[i] = arena_alloc(&arena, sizeof(uint64_t) * row_count, 4);
  //}
  
  size_t num_connections = 1000;
  size_t count = 0;
  Heap heap;
  initialize_heap(&heap, num_connections);
  for(size_t i = 0; i < row_count; i++) {
    for(size_t j = 0; j < row_count; j++) {
      if (i < j) {
        if (count < num_connections) {
          uint64_t distance = squared_distance(&v[i], &v[j]);
          add_to_heap(&heap, distance, i, j);
          count++;
        }
        else {
          uint64_t distance = squared_distance(&v[i], &v[j]);
          if (distance < heap.data[0].distance) {
            //it is less than the greatest ellement in the heap.
            //so add it
            add_to_heap(&heap, distance, i, j);
          }
        }
      }
    }
  }

  //now we should have the 10 (or k smallest distances. )

  //we make connections by a union find mechanism, 
  //we make a list as long as the elements, and in that we keep the index of the 
  // element that is our parent. 
  // when connecing one to another - we connect it to one and if that one is connected to another,
  // we make a while loop. until we find the root of the connection. 

  size_t * union_find = arena_alloc(&arena, row_count * sizeof(size_t), 4);
  size_t * component_size = arena_alloc(&arena, row_count * sizeof(size_t), 4);
  if (!union_find || !component_size) {
    printf("failed union find alloc");
    arena_free(&arena);
    return 1;
  }
  for (size_t i = 0; i < row_count; i++) {
    union_find[i] = i; 
  }
  for (size_t i = 0; i < row_count; i++) {
    component_size[i] = 1; 
  }

  //no point in any order really -> we can just loop over the heap and add them all. 
  //to our union find. 
  for (size_t i = 0; i < num_connections; i++) { // Loop descending if not reversed/sorted
    HeapNode edge = heap.data[i];
    unite_sets(union_find, component_size, edge.index_a, edge.index_b); 
  }


  size_t component_sizes_list[row_count];
  size_t unique_count = 0;

  count = 0;
  size_t max_components = 3;
  MinHeap min_heap;
  initialize_min_heap(&min_heap, 3);
  for (size_t i = 0; i < row_count; i++) {
    // A component's true size is stored only at its root. 
    // If union_find_parent[i] == i, then 'i' is a root.
    if (find_root(union_find, i) == i) {
        uint64_t current_size = (uint64_t)component_size[i];
        
        if (count < max_components) {
            // Initial fill: Add the component size to the heap
            add_to_min_heap(&min_heap, current_size); 
            count++;
        }
        else {
            // Replacement: Check against the root (smallest of the current 3 largest)
            uint64_t smallest_of_top_k = min_heap.data[0].component_size;
            if (current_size > smallest_of_top_k) {
                add_to_min_heap(&min_heap, current_size);
            }
        }
    }
  }
  
  size_t product = 1;

  for (size_t i = 0; i < max_components; i++) {
    product *= min_heap.data[i].component_size;
  }

  printf("product: %zu\n", product);

  




  


  

  free_heap(&heap);
  arena_free(&arena);
  fclose(fptr);
  return 0;
}
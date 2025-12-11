#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>

//maybe a Linked list of neigbours.
//okay i opted for a adjacency matrix for now instead. 
//typedef struct LinkedList {
//  uint32_t neighbour;
//  LinkedList* next;
//} LinkedList;


uint32_t dfs(uint32_t current_index, uint8_t *bitset, uint32_t count, uint32_t out) {

  if (current_index == out) {
    return 1;
  }
  uint32_t sum_of_paths = 0;

  for (uint32_t next = 0; next < count; next++) {
    size_t bit_index = (size_t)current_index * count + next;
    size_t byte_index = bit_index >> 3;
    uint8_t bit_offset = bit_index & 7;

    if (bitset[byte_index] & (1 << bit_offset)) {
        sum_of_paths += dfs(next, bitset, count, out);
    }
  }

  return sum_of_paths;
}

int main() {

  FILE *fptr = fopen("input.txt", "r");


  int ch, last_ch; 

  size_t row_count = 0;

  // 26^3 = 17576 like 70 kB memory for a direct access table of the chars. not terrible. 
  // it might be acceptable. if we want a matrix though it might be a bit much. 
  // so maybe we make a direct access table -> and inside it we put a index to each 
  //not sure if it is fine but well try to get it on the stack. A uint16 or uint8 might have worked here aswell. 
  //not really that many entries. 
  uint32_t indices[17576] = {0};

  //uint32_t out = ('o' - 'a') * 26 * 26 + ('u' - 'a') * 26 + ('t' - 'a');
  //printf("out: %u\n", out);
  //gives 10003

  //uint32_t you = ('y' - 'a') * 26 * 26 + ('o' - 'a') * 26 + ('u' - 'a');
  //printf("you: %u\n", you); //gives 16608

  const uint32_t out = 10003;
  const uint32_t you = 16608;
  
  
  printf("reading size\n");
  uint32_t current_index = 0;
  uint32_t unique_count = 0;
  while((ch = fgetc(fptr)) != EOF) {
    if ('a' <= ch && ch <= 'z') {
      current_index = current_index * 26 + (ch - 'a');
    }
    else if (ch == ':') {
      indices[current_index] = unique_count;
      unique_count++;
    }
    else if (ch == '\n') {
      current_index = 0;
    }
  }

  //printf("unique: %u\n", unique_count);
  //okay now we need a proximity matrix of size unique_count * unique count
  //this should really be a bitset -> because it is just telling us wether they are connected or not. 
  size_t bit_count  = (size_t)unique_count * unique_count;
  size_t byte_count = (bit_count + 7) / 8;

  uint8_t *bitset = malloc(byte_count);
  if (!bitset) {
    perror("malloc failed");
    return 1;
  }
  // bit_index = r * unique_count + c
  //reading a bit (bitset[bit_index >> 3] >> (bit_index & 7)) & 1;

  memset(bitset, 0, byte_count);  // clear all bits

  //printf("begin reading bitset \n");


  fseek(fptr, 0, SEEK_SET);
  current_index = 0;
  uint32_t current_parent_index = 0;
  while((ch = fgetc(fptr)) != EOF) {
    if ('a' <= ch && ch <= 'z') {
      current_index = current_index * 26 + (ch - 'a');
    }
    else if (ch == ':') {
      //get the index in the lookuptable;
      current_parent_index = indices[current_index];
      current_index = 0;
      fgetc(fptr); //throw an extra space. 
    }
    else if (ch == ' ') {
      uint32_t mapped_child = indices[current_index];
      uint32_t bit_index = current_parent_index * unique_count + mapped_child;  

      bitset[bit_index >> 3] |= 1 << (bit_index & 7); //set a bit
      current_index = 0;
    }
    else if (ch == '\n') {
      uint32_t mapped_child = indices[current_index];
      uint32_t bit_index = current_parent_index * unique_count + mapped_child;  
      bitset[bit_index >> 3] |= 1 << (bit_index & 7); //set a bit
      current_index = 0;
    }
  }
  //printf("reading bitset completed\n");



  //we neeed a dfs or bfs, does not really matter we need to iterate the entire graph. 
  //not sure if there are loops. 

  //lets start on "you"
  uint32_t num_paths = dfs(indices[you], bitset, unique_count, indices[out]);
 
  printf("num paths: %u", num_paths);


  free(bitset);
  fclose(fptr);
  return 0;
}

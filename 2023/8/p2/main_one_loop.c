#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

#define three_letter_to_code(s) ( ((s[0] - 'A') << (5 * 2)) | ((s[1] - 'A') << 5) | (s[2] - 'A') )
#define three_letter_to_index(s) ( ((s[0] - 'A') * 676) + ((s[1]- 'A') * 26) + (s[2] - 'A') )
#define code_to_index(code) ( (((code >> (5 * 2)) & 0b11111) * 676) + (((code >> (5)) & 0b11111) * 26) + ((code) & 0b11111) ) 

#define FILE_NAME "input.txt"

void index_to_string(size_t index, char *out) {
    out[0] = 'A' + (index / 676);
    out[1] = 'A' + ((index / 26) % 26);
    out[2] = 'A' + (index % 26);
    out[3] = '\0';
}

int main(void){
  FILE* fptr = fopen(FILE_NAME, "r");

  if (!fptr){
    printf("Could not open file (%s)", FILE_NAME);
    return 1;
  }
  char ch;
  size_t instruction_count = 0;
  while((ch = fgetc(fptr)) != EOF && (ch == 'R' || ch == 'L')) {
    instruction_count++;
  }
  uint8_t* instructions = malloc(instruction_count * sizeof(uint8_t));

  if (!instructions) {
    printf("Failed to allocate memory for instructions.");
    fclose(fptr);
  }

  rewind(fptr);

  size_t current = 0;
  while((ch = fgetc(fptr)) != EOF && (ch == 'R' || ch == 'L')) {
    if (ch == 'L') {
      instructions[current++] = 1;
    }
    else {
      instructions[current++] = 0;
    }
  }

  while((ch = fgetc(fptr)) != EOF && (ch < 'A' || ch > 'Z') && (ch < '0' && ch > '9'));

  fseek(fptr, -1, SEEK_CUR);

  uint32_t * directions = malloc(17576 * sizeof(uint32_t));

  if (!directions) {
    printf("Failed to allocate memory for direct access table");
    free(instructions);
    fclose(fptr);
    return 1;
  }

  size_t starting_indexes_cap = 1000;
  size_t start_indexes_current = 0;
  size_t *starting_indexes = malloc(starting_indexes_cap * sizeof(size_t));

  char in[4];
  char left[4];
  char right[4];
  while(fscanf(fptr, "%3s = (%3s, %3s)", in, left, right) == 3) {
    size_t index = three_letter_to_index(in);
    directions[index] = (three_letter_to_code(left) << 16) | (three_letter_to_code(right));
    //we want to check if last letter of in is A, then its a starting index. 
    if (in[2] == 'A') {
      if (start_indexes_current >= starting_indexes_cap){
        void *tmp = realloc(starting_indexes, starting_indexes_cap * sizeof(size_t) * 2);

        if (!tmp) {
          printf("Malloc failed when reallocating for starting indexes.");
          free(starting_indexes);
          free(instructions);
          free(directions);
          fclose(fptr);
          return 1;
        }

        starting_indexes = tmp;
        starting_indexes_cap *= 2;
      }
      starting_indexes[start_indexes_current++] = index;
    }
  }

  printf("Number of starting indexes: %d \n", start_indexes_current);

  size_t counter;
  uint16_t mod_count;

  size_t seen_bitfield_length = (instruction_count + 7 ) / 8; // how many bytes it takes to store a seen. 
  uint8_t *seen = malloc(17576 * seen_bitfield_length * sizeof(uint8_t));

  if (!seen) {
    printf("Failed malloc for seen. ");
    free(starting_indexes);
    free(instructions);
    free(directions);
    fclose(fptr);
    return 1;
  }

  uint8_t ends_with_Z[17576];
  for (int i = 0; i < 17576; i++) {
      char name[4];
      index_to_string(i, name);
      ends_with_Z[i] = ( name[2] == 'Z');
  }


  size_t test_index = three_letter_to_index("AAZ");
  uint8_t test_if = ends_with_Z[test_index];
  printf("checked all Z. %" PRIu8 " \n", test_if);

  if (!seen) {
    printf("Failed to malloc seen hash (%d bytes)", 17576 * seen_bitfield_length * sizeof(uint8_t));
    free(directions);
    free(starting_indexes);
    free(instructions);
    fclose(fptr);
    return 1;
  }
  for (int i = 0; i < start_indexes_current; i++){
    counter = 0;
    mod_count = 0;
    for (int byte = 0; byte < 17576 * seen_bitfield_length; byte++) {
      seen[byte] = 0;
    }

    size_t current_index = starting_indexes[i];

    size_t z_count = 0;
    while(1) {
      //we are storing which index -> and which instruction -> this is given by mod_count and index. 
      //we divide mod_count by 8. 
      size_t index = (current_index * seen_bitfield_length) + (mod_count >> 3);
      uint8_t mask = 1 << (mod_count & 7); 

      if (ends_with_Z[current_index]) {
        z_count++;
      }

      if (!(seen[index] & mask)) {
          seen[index] |= mask;
      } else {
          break;
      }
      
      current_index = code_to_index((directions[current_index] >> (instructions[mod_count] << 4)));

      counter++;
      mod_count++;
      if (mod_count >= instruction_count) {
        mod_count = 0; 
      }
    }
    
    char start_node_name[4];
    char end_node_name[4];
    index_to_string(starting_indexes[i], start_node_name);
    index_to_string(current_index, end_node_name);
    printf("Start: %s, End: %s, Cycle length: %zu, Z count: %zu \n", start_node_name, end_node_name, counter, z_count);

  }

  printf("Instructions: %d", counter);


  free(starting_indexes);
  free(directions);
  free(instructions);
  fclose(fptr);
  return 0;
}
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>


/**
 * The questions is, can i encode three letter sets in a good hashset? 
 * We have 26 symbols -> 
 * meaning we can encode one letter in 5 bits
 * Meaning we need 15 bits to hash a three letter symbol. 
 * which means we can work with uint16_t for each three letter combo.
 * 
 * we need a 26^3 = 17576 long list to store these with a direct access table.
 * We can store the result for each hashed input in a uint32_t (16 bits for each direction.)
 * This is a total of 70kb -> which is not super much for a modern computer to handle -> however
 * 
 * the full input is only around 740 entries long. So we will waste a lot of space. Is it worth it to write
 * a hashset -> the downside of course being we will need to handle any collisions? 
 * 
 * I am going to go with a direct access table again. 
 * 
 * for part 2:
 * We pretty much got it all I think we need to to find each starting position by checking when reading if they end with A. 
 * and in that case saving their index -> then we make a list as large as the starting positions. probably dynamic. 
 * Now we are up to three mallocs it would probably be best to make an arena instead and only malloc once -> but we are gonna skip that
 * and then for each instruction we loop through all of our current indexes and take left or right of them. 
 * 
 * for the ending indexes we can check index modulus 26 == 25 which will be all that end with Z. 
 * 
 * Okay. That was not enough. That was slow as fuck. 
 * 
 * I think what we need to do is calculate some least common multiple. 
 * 
 * What is the first time t such that each individiual path is at a node ending in z at time t
 * 
 * For each then we need to find each offset 
 * 
 * WHen it is att ZZZ -> it does not matter -> it will always be at ZZZ
 * 
 * So how long until it gets into a loop -> and how long is the loop? 
 * 
 * to find a loop -> we need to run it for as long is needs until it finds itself on the same state again -> 
 * i.e. it finds itself on the same node with the same direction instructions. 
 * 
 * where counter modulus instruction count is the same and where the node is the same. 
 * 
 */

//each letter is encoded in 5 bits. 
//For a total of 15 bits. 
#define three_letter_to_code(s) ( ((s[0] - 'A') << (5 * 2)) | ((s[1] - 'A') << 5) | (s[2] - 'A') )
#define three_letter_to_index(s) ( ((s[0] - 'A') * 676) + ((s[1]- 'A') * 26) + (s[2] - 'A') )
#define code_to_index(code) ( (((code >> (5 * 2)) & 0b11111) * 676) + (((code >> (5))& 0b11111) * 26) + ((code) & 0b11111) ) 
//we need code to index? 

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
  //while it is a direction R or L, count number of instructions. 
  size_t instruction_count = 0;
  while((ch = fgetc(fptr)) != EOF && (ch == 'R' || ch == 'L')) {
    instruction_count++;
  }

  //printf("%d  \n", instruction_count);
  
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

  //printf("Last char: %c\n", ch);
  fseek(fptr, -1, SEEK_CUR);

  //for (int i = 0; i < instruction_count; i++ ){
  //  puts(instructions[i] ? "L" : "R");
  //}

  // allocate memory for direct access table. 

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
  uint16_t mod_count; // keeps track of which instructions. 

  size_t seen_bitfield_length = (instruction_count + 7 ) / 8; // how many bits we need to store. 
  uint8_t *seen = malloc(17576 * seen_bitfield_length * sizeof(uint8_t));

  if (!seen) {
    printf("Failed malloc for seen. ");
    free(starting_indexes);
    free(instructions);
    free(directions);
    fclose(fptr);
    return 1;
  }


  // we should of course only calculate these for those that we care about -> i.e those in 
  // our saved indices -> but we dont really have that. 
  uint8_t ends_with_Z[17576];
  for (int i = 0; i < 17576; i++) {
      char name[4];
      index_to_string(i, name);
      //also tried name[2] == "Z" ||
      ends_with_Z[i] = ( name[2] == 'Z'); // only need to look at last char
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
    //check all to see if they end with Z. by mod 26 == 25. 

    //clear the seen. 
    for (int byte = 0; byte < 17576 * seen_bitfield_length; byte++) {
      seen[byte] = 0;
    }

    //size_t mod = counter % instruction_count; // can i calculate these for each in instruction before? 
    //is it cheaper to do a if check maybe. 
    //and just loop over them maybe to save time? 
    size_t current_index = starting_indexes[i];
    //we want to hash those that we have already seen somehow. 
    // allocating an additional 70kb for a direct access table for those that we have seen is probably a bit much. 
    // is there any data left at that 70kb direct acces table? 
    // as we know directions is a uint32. 
    // uint32_t * directions = malloc(17576 * sizeof(uint32_t));
    //we have two bits left that we could potentially use. 
    //problem is we would need to know that the mod_count is the same -> 
    //which would require us to save some additional data. 
    //We could use the additional bit as a flag that it as direction data store -> 
    // and then use some probing technique
    // So say for seen -> we hash the direction -> then check the direction to the right until that flag is not high
    // Since the data is so sparse -> we utilize some of that data for other storing. we could then light the second high
    // to indicate that it is the type we are interested in. 
    //is this faster than regular hashing? Most likely right? 

    char node_name[4];

    index_to_string(starting_indexes[i], node_name);
    printf("%s, ", node_name);
    //  For each node we run until we find a end z node -> that is the offset 
    //  then we loop until we have a seen -> that is the cycle length, 
    while(1) {
      if (ends_with_Z[current_index] == 1) {
        //we found an index then ends with Z.
        //Why the fuck does this not break ever? 
        break;
      }

      //change to next index based on instruction. 
      //instructions[mod_count] is either 0 or 1 (R, L)
      // so the right hand is 0 or 16 (R, L) 
      current_index = code_to_index( directions[current_index] >> (instructions[mod_count] << 4) );

      //index_to_string(current_index, node_name);
      //printf("%s, ", node_name);

      counter++;
      mod_count++;
      if (mod_count >= instruction_count) {
        // I could also just increment and when using 
        // I could do modulus -> but it is probably slower.
        mod_count = 0; 
      }
    }
    size_t offset_to_end = counter;
    //printf("Found first offset: %zu\n ", offset_to_end);
    counter = 0;
    while(1) {
      //add to seen? 
      size_t index = current_index * seen_bitfield_length + (mod_count >> 3);
      uint8_t mask = 1 << (mod_count & 7);
      
      if (!(seen[index] & mask)) {
          seen[index] |= mask;
      } else {
          // already seen, its a loop. 
          break;
      }
      
      //change to next index based on instruction. 
      current_index = code_to_index(directions[current_index] >> (instructions[mod_count] << 4));

      //index_to_string(current_index, node_name);
      //printf("%s, ", node_name);

      //then increment counter.
      counter++;
      mod_count++;
      if (mod_count >= instruction_count) {
        // I could also just increment and when using 
        // I could do modulus -> but it is probably slower.
        mod_count = 0; 
      }
    }
    
    printf("\n");
    char start_node_name[4];
    char end_node_name[4]; //end node is the last unique node before periodicity starts. 
    index_to_string(starting_indexes[i], start_node_name);
    index_to_string(current_index, end_node_name);
    printf("Start: %s, End: %s, Offset: %zu Cycle length: %zu\n", start_node_name, end_node_name, offset_to_end, counter);

  }

  printf("Instructions: %d", counter);


  free(starting_indexes);
  free(directions);
  free(instructions);
  fclose(fptr);
  return 0;
}
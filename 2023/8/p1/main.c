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
 */

//each letter is encoded in 5 bits. 
//For a total of 15 bits. 
#define three_letter_to_code(s) ( ((s[0] - 'A') << (5 * 2)) | ((s[1] - 'A') << 5) | (s[2] - 'A') )
#define three_letter_to_index(s) ( ((s[0] - 'A') * 676) + ((s[1]- 'A') * 26) + (s[2] - 'A') )
#define code_to_index(code) ( (((code >> (5 * 2)) & 0b11111) * 676) + (((code >> (5))& 0b11111) * 26) + ((code) & 0b11111) ) 
//we need code to index? 

#define end 

#define FILE_NAME "input.txt"

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
    free(instructions);
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

  while((ch = fgetc(fptr)) != EOF && (ch < 'A' || ch > 'Z'));

  printf("Last char: %c\n", ch);
  fseek(fptr, -1, SEEK_CUR);

  //for (int i = 0; i < instruction_count; i++ ){
  //  puts(instructions[i] ? "L" : "R");
  //}

  // allocate memory for direct access table. 

  uint32_t * directions = malloc(17576 * sizeof(uint32_t));

  if (!directions) {
    printf("Failed to allocate memory for direct access table");
    fclose(fptr);
    return 1;
  }

  char in[4];
  char left[4];
  char right[4];
  while(fscanf(fptr, "%3s = (%3s, %3s)", &in, &left, &right) == 3) {
    //size_t index = ((in[0] - 'A') * 676) + ((in[1]- 'A') * 26) + (in[2] - 'A');
    size_t index = three_letter_to_index(in);
    directions[index] = (three_letter_to_code(left) << 16) | three_letter_to_code(right);
  }

  size_t counter = 0;
  size_t current_index = 0;
  const size_t final_index = three_letter_to_index("ZZZ");
  while(current_index != final_index) {
    //0 -> left -> 1 right
    current_index = code_to_index(directions[current_index] >> (16 * instructions[counter % instruction_count]));

    counter++;
  }


  printf("Instructions: %d", counter);





  free(directions);
  free(instructions);
  fclose(fptr);
  return 0;
}
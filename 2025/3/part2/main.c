#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>


/**
 * okay for twelve numbers, we can no longer simply check if we got the last number
 * we need to check if there are atleast twelwe numbers left, etc. and that might be time consuming. 
 * 
 * I think probably we need to refere to actually reading the full line as a string. 
 * The longest string is 100 characters long. 
 */

int main() {

  FILE *fptr = fopen("input.txt", "r");
  int ch;
  uint64_t sum = 0;
  uint8_t numbers_to_find = 12;
  int row[110];
  uint8_t current_index = 0;

  //423423423427 what i got 
  //434234234278 what it should be
  
  while((ch = fgetc(fptr)) != EOF) {
    if ('0' <= ch && ch <= '9') {
      row[current_index] = ch - '0';
      current_index++;
    }
    else if (ch == '\n') {
      //for (int i = 0; i < current_index; i++) {
      //  //printf("%d", row[i]);
      //}
      //printf("\n");
      //handle it
      uint8_t last_index = -1; //this keeps track of where the last largest was selected from. 
      uint64_t number = 0;
      for (uint8_t n = 0; n < numbers_to_find; n++) {
        //find the largest number a from 0..(current_index-12)
        uint8_t largest = 0;
        uint8_t largest_index;
        for (uint8_t i = last_index + 1; i < current_index - (numbers_to_find - n) + 1; i++) {
          if (row[i] > largest) {
            largest = row[i];
            largest_index = i;
          }
        }
        //now we found the greatest
        //printf("selected %d at (%d)\n", largest, largest_index);
        last_index = largest_index;
        number *= 10;
        number += largest;
      }
      //printf("Number: %llu\n", number);
      sum += number;
      
      current_index = 0;
    }
  }
  //printf("first: %d, second: %d\n", first_number, second_number);

  printf("sum: %llu", sum);

  fclose(fptr);
  return 0;
}
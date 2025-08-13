// I am thinking maybe i only need to save those last 2 rows to calculate the next.
//  althought saving a few extra might not be too terrible.
// it is essentially a graph.
// do we need to save each line?
// or is it enough to save the most recent line to calculate next -> and checking if its sum is 0?
// then for each iteration we check it and the next number -> and calculate a new index here.

// What can we cache? Lets just code it up in the manner in it expressed first.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

#define FILE_NAME "input.txt"

int main(void)
{

  FILE *fptr = fopen(FILE_NAME, "r");

  if (!fptr)
  {
    printf("Failed to open " FILE_NAME);
  }

  int64_t num;
  int ch;
  size_t num_count = 0;
  while (fscanf(fptr, "%" SCNd64 , &num) == 1)
  {
    //printf("%" PRId64 " ", num);

    num_count++;
    ch = fgetc(fptr);
    if (ch == '\n' || ch == EOF)
      break; // stop after first line
  }

  //size_t rows = 2;

  //while((ch = fgetc(fptr)) != EOF) {
  //  if (ch == '\n') {
  //    rows++;
  //  }
  //}

  //printf("%llu %llu", num_count, rows); //that is the length of what we need.
  //to save our numbers? 
  //i guess we need one extra for the result

    //we are just calculating a sum. so we do it in place. 

  //somehow for each row. 

  rewind(fptr);

  int64_t numbers[num_count];

  uint32_t current = 0;
  int64_t total_oasis = 0;
  while (fscanf(fptr, "%" SCNd64, &num) == 1)
  {
    numbers[current] = num;
    current++;

    ch = fgetc(fptr);
    if (ch == '\n' || ch == EOF){
      //here we have all numbers. 
      int64_t sum = 1;
      int right = (int) current - 1; //moving the right to the left because we save the extra numbers above them later. 
      while (sum != 0 && right > 0) {

        //debug print 
        for (uint32_t i = 0; i < current; i++) {
          printf("%" PRId64 " ", numbers[i]);
          if ((int) i == right) {
            printf(" . ");
          }
        }
        printf("\n");

        sum = 0;
        for(int i = 0; i < right; i++) {
          numbers[i] = numbers[i + 1] - numbers[i]; //take the difference. 
          sum += numbers[i] < 0 ? -numbers[i] : numbers[i];
        }
        right--;
      }
      //debug print 
      for (uint32_t i = 0; i < current; i++) {
        printf("%" PRId64 " ", numbers[i]);
        if ((int) i == right) {
          printf(" . ");
        }
      }
      printf("\n\n");
      //calculate the new rows and the sum of them. 
      //printf("Numbers[right]: %lld, Current: %d \n", numbers[right + 1], current);
      int64_t last_number = 0;
      printf("last_numbers: ");
      for (int i = right + 1; i < (int) current; i++) {
        printf("%" PRId64 " ", last_number);
        last_number += numbers[i];
      }
      printf("%" PRId64 " \n\n", last_number);
      total_oasis += last_number;
      current = 0;
    }
  }

  printf("Total: %" PRId64 "\n", total_oasis);

  fclose(fptr);
  return 0;
}
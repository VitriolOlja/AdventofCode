#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>


int main() {

  FILE *fptr = fopen("input.txt", "r");
  int ch;

  uint8_t first_number = 0;
  uint8_t second_number = 0;
  uint32_t sum = 0;
  while((ch = fgetc(fptr)) != EOF) {
    if (ch == '\n') {
      //printf("first: %d, second: %d\n", first_number, second_number);
      sum += first_number * 10 + second_number;


      //first_index = -1;
      first_number = 0;
      //second_index = -1;
      second_number = 0;
      //current_index = 0;
      continue;
    }
    if (ch >= '0' && ch <= '9') {
      uint8_t value = ch - '0';
      //check if greater than first, than if greater than second etc. 
      if (value > first_number) {
        //need to check that it is not last number.
        int next = fgetc(fptr);
        if (next != '\n'){
          first_number = value;
          second_number = 0;
        }
        else {
          second_number = value;
        }
        fseek(fptr, -1, SEEK_CUR); //unget
      }
      else if (value > second_number) {
        second_number = value;
      }

    }

  }
  //printf("first: %d, second: %d\n", first_number, second_number);
  sum += first_number * 10 + second_number;

  printf("sum: %d", sum);

  fclose(fptr);
  return 0;
}
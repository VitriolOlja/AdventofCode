#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

//idea maybe sort the ranges into a binary search tree? 
//could a range be inside another range? 
//then we have to check for each entry 

// Another idea - just sort them based on the first number and 
// then on each element also save how many 'after' it there are,
// or just the larger number aswell.
// if it is larger than the larger
// we continue 
//if smaller than smallest we end check. 

//nevermind lets just bruteforce it :)

int main() {

  //lets read to see how many there are first. 
  FILE* fptr = fopen("input.txt", "r");
  int ch;
  uint8_t is_range = 0;
  uint32_t num_ranges = 0;
  while ((ch = fgetc(fptr)) != EOF) {
    if (ch == '-') {
      is_range = 1;
    }
    if (ch == '\n') {
      if (is_range) {
        num_ranges++;
        is_range=0;
      }
      else {
        break;
      }
    }
  }
  //printf("num ranges: %d", num_ranges); 
  uint64_t (*map)[2] = malloc(sizeof(uint64_t) * 2 * num_ranges);

  fseek(fptr, 0, SEEK_SET);
  uint32_t current_range = 0;
  uint64_t current_number = 0;
  while ((ch = fgetc(fptr)) != EOF) {
    if ('0' <= ch && ch <= '9'){
      current_number *= 10;
      current_number += ch - '0';
    }
    if (ch == '-') {
      is_range = 1;
      map[current_range][0] = current_number;
      current_number = 0;
    }
    if (ch == '\n') {
      if (is_range) {
        
        map[current_range][1] = current_number;
        current_number = 0;
        current_range++;
        is_range=0;
      }
      else {
        break;
      }
    }
  }

  //for (int i = 0; i < num_ranges; i++) {
  //  printf("%llu - %llu\n", map[i][0], map[i][1]);
  //}
  uint64_t fresh = 0;
  current_number = 0;
  while ((ch = fgetc(fptr))!= EOF) {
    if ('0' <= ch && ch <= '9') {
      current_number *= 10;
      current_number += ch - '0';
    }
    else if (ch == '\n') {
      //search for current number in list to see if fresh. 

      //uint8_t in_list = 0;
      for (uint32_t i = 0; i < num_ranges; i++) {
        if (map[i][0] <= current_number && current_number <= map[i][1]) {
          fresh++;
          break;
        }
      }
      current_number = 0;
    }
  }

  printf("fresh: %llu\n", fresh);


  free(map);
  fclose(fptr);
  return 0;
}
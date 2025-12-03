#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main()  {
  
  int16_t dial = 50;
  int16_t prev_dial;

  FILE *fptr = fopen("input.txt", "r");
  char buffer[10];
  uint16_t count = 0;
  while (fgets(buffer, sizeof(buffer), fptr)) {
    //printf("dial: %d, count: %d, instr: %s", dial, count, buffer);
    //printf("%s", buffer); 
    int num = atoi(&buffer[1]);
    count += num / 100;
    num %= 100;
    int prev_dial = dial;
    if (buffer[0] == 'L') {
      dial -= num;
    }
    else {
      dial += num;
    }
    
    if (dial < 0) {
      dial += 100;
      if (prev_dial != 0){
        count++;
      }
    }
    else if (dial >= 100) {
      dial -= 100;
      if (prev_dial != 0){
        count++;
      }
    }
    else if (dial == 0) { 
      count++;
    }
  }
  
  //printf("\ndial: %d, count: %d, instr: %s\n", dial, count, buffer);

  printf("Count: %d\n", count);

  fclose(fptr);
  return 0;
}
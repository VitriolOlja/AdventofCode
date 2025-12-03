#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main()  {
  
  int16_t dial = 50;

  FILE *fptr = fopen("input.txt", "r");
  char buffer[10];
  uint16_t count = 0;
  while (fgets(buffer, sizeof(buffer), fptr)) {
    //printf("%s", buffer); 
    int num = atoi(&buffer[1]);
    if (buffer[0] == 'L') {
      dial -= num;
    }
    else {
      dial += num;
    }
    dial = ((dial % 100) + 100) % 100;
    if (dial == 0) {
      count++;
    }
  }

  printf("Count: %d\n", count);
 
  
  fclose(fptr);
  return 0;
}
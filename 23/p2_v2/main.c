#include <stdio.h> 
#include <inttypes.h>

#define LETTERS 26 // ('z' - 'a' + 1)
#define ROWS 3380


// i want a structure with graph. 
// it needs to be 
// log2 (26^4) ~ 18.8
// so i need 19 bits atleast to save a connection. 

/**
 * 4 letters = 26^4 = 456976
 * if above is true i can hash each connection to a uint32_t
 * 
 */

int main(void) {
  uint16_t *connections[ROWS] = {};

  FILE *fptr = fopen("input.txt", "r");
  char ch;
  uint8_t chars;
  
  uint32_t current_connection;
  for(int i = 0; i < ROWS; i++) {
    chars = 0;
    uint8_t combo = 0;
    while (combo < 2) {
      while (1) {
        ch = fgetc(fptr);
        
        if (ch == '-') {
          combo += 1;
          continue;
        }
        else if (ch == '\n') {
          combo += 1;
          break;
        }
        
        current_connection |= (ch << (chars * 8));
        chars++;
      }
    }
      
    connections[i] = (uint16_t*)current_connection; 
  }

  //print 10
  for(int i = 0; i < 10; i++) {
    printf("at %d, value: ", i);

    uint8_t *char_ptr = ( uint8_t*) &connections[i];
    for(int j = 0; j < 4; j++) {
      printf("%c", char_ptr[j]);
    }
    printf("\n");
  }
}
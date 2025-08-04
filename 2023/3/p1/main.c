#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//  lets employ a three pass strategy. 
//      1. Read the file, read number of rows and columns,
//      2. Save the file to a dynamically allocated char array. 
//      3. Read the allocated char map, interpreting the numbers and if they have any symbols around them. 

// I definitely think this will be the clearest solution. 

#define FILE_NAME "input.txt"

int main(void)
{
  FILE *fptr = fopen(FILE_NAME, "r");

  // int **array;
  // size_t array_height = 0;
  // size_t array_width = 0;

  if (!fptr)
  {
    printf("Could not open file. \n");
    return 1;
  }


  uint16_t columns = 0;
  char ch;
  while ((ch = fgetc(fptr)) != EOF && ch != '\n'){
    columns++;
  }
  uint16_t rows = 2;  
  //there might not be a newline at the end of the file. 
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      rows++;
    }
  }

  /**
   * We could optimize below to one malloc aswell. 
   */
  char **map = (char**)malloc(rows * sizeof(char*));

  if (!map) {
    printf("first malloc failed");
    return 1;
  }

  char* tmp = malloc(rows * columns * sizeof(char));

  if (!tmp) {
    printf("second malloc failed");
    free(map);
    return 1;
  }

  for(int i = 0; i < rows; ++i) {
      map[i] = &tmp[i * columns];
  }

  //2. now time for reading file again and storing

  rewind(fptr);

  uint16_t current_row = 0;
  uint16_t current_column = 0;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      current_row++;
      current_column = 0;
      continue;
    }

    map[current_row][current_column] = ch;

    current_column++;
  }

  // sanity check
  // for(int i = 0; i < rows; i++) {
  //   for (int j = 0; j < columns; j++) {
  //     printf("%c", map[i][j]);
  //   }
  //   printf("\n");
  // }

  uint32_t sum = 0;

  uint16_t current_number = 0;
  uint8_t current_number_width = 0;

  //here i can also take several approaches. 
  // I could read each number - check if there is a symbol anywhere around it 
  // (something that is not a number or a dot)
  // and if so put a boolean to true. 
  // Or i can parse a number. save the width of the number 
  // and then calculate all the positions that are around the number 
  // I think the second way will produce less overall character checks 
  // and will therefore be faster but its hard to tell. 

  for(int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if ('0' <= map[i][j] && map[i][j] <= '9') {
        //its a number begin saving it. 
        current_number *= 10;
        current_number += map[i][j] - '0';
        current_number_width++;
      }
      else {
        //its either a dot or a symbol. 
        //if we have a number > 0 we need to check if there are any proximal 
        // symbols.

        //could also check current_number_width in case we have a number like 0300.
        if (current_number > 0) {
          //check all positions around it. 

          //check if there is a row above it. 
          uint8_t above = 0; 
          uint8_t below = 0;
          uint8_t found_symbol = 0;
          if (i > 0) {
            //check the row above
            above = 1; //there is a row above

            for (int w = 0; w < current_number_width; w++) {
              // check if it is not a number, and that it is not just a dot. 

              char to_check = map[i - 1][ w + j - current_number_width];

              if ((to_check < '0' || to_check > '9') && to_check != '.') {
                found_symbol = 1;
                break;
              }
            }
          }

          if (!found_symbol && i < rows - 1) {
            //there is a row below
            below = 1;

            for (int w = 0; w < current_number_width; w++) {
              // check if it is not a number, and that it is not just a dot. 

              char to_check = map[i + 1][w + j - current_number_width];
              if ((to_check < '0' || to_check > '9') && to_check != '.') {
                found_symbol = 1;
                break;
              }
            }
          }

          //check left
          //j probably not greater than zero. 
          if (!found_symbol && j > 0) {

            // i is the current row we are at. 
            // if we need to check above it, we iterate from one less. (- above)
            // if we need to check below it, we iterate to one more (+ below)
            for (int h = i - above; h <= i + below; h++) {
              //the j is currently on position right of the number. 
              //if we want to check x for example in the following '.x79p.'
              //we are at p which is either a dot or symbol
              char to_check = map[h][j - current_number_width - 1];
              
              if ((to_check < '0' || to_check > '9') && to_check != '.') {
                found_symbol = 1;
                break;
              }
            }
          }

          //check right
          //check left
          if (!found_symbol && j < columns) {
            // i is the current row we are at. 
            // if we need to check above it, we iterate from one less. (- above)
            // if we need to check below it, we iterate to one more (+ below)
            for (int h = i - above; h <= i + below; h++) {
              //the j is currently on position right of the number. 
              //which we want to check.
              char to_check = map[h][j];
              
              if ((to_check < '0' || to_check > '9') && to_check != '.') {
                found_symbol = 1;
                break;
              }
            }
          }


          if (found_symbol) {
            //printf("Adding number: %d \n", current_number);
            sum += current_number;
          }

          current_number = 0;
          current_number_width = 0;
        }
      }
    }
  }


  printf("sum %d", sum);



  fclose(fptr);
  free(map);
  free(tmp);
}

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//  lets employ a three pass strategy. 
//      1. Read the file, read number of rows and columns,
//      2. Save the file to a dynamically allocated char array. 
//      3. Read the allocated char map, interpreting the numbers and if they have any symbols around them. 

// For part 2. 
// 1 and 2 should be the same. 
// I wonder what the best way to check this is. 
// I think I have to keep the same logic. 
// but then also check around the symbol. 
// to not check a symbol multiple times, we might employ a hashtable to keep track
// of the seen symbols. OR even better we just remove the symbol and put a dot there. 
// what shall be done about the gears that have multiple surrounding numbers? Are there
// even such numbers? 
// Apparently not. 
// Coutign the symbols, and printing anytime i have a symbols_found > 1, actually produced no output. 
// so for every number there is at most 1 symbol. 


/**
 * This functions expect to receive a position for a symbol
 * It then calculates the gear ratio by multiplying all numbers around it. 
 * we calculate the gear ratio then remove the symbol.
 */
uint32_t calc_gear_ratio(char** map, uint16_t x_pos, uint16_t y_pos, uint16_t map_width, uint16_t map_height) {

  if (map[y_pos][x_pos] == '.' || ('0' <= map[y_pos][x_pos] && map[y_pos][x_pos] <= '9')) {
    //it is a number or a dot and as such not a symbol.
    printf("undefined behavior, gear ratio received a none symbol character \n");
    return 0;
  }
  // start with top line. 
  uint8_t has_left = x_pos > 0;
  uint8_t has_right = x_pos < map_width - 1;

  uint64_t gear_ratio = 1;
  uint8_t adjacent_number_count = 0;

  //check above
  if (y_pos > 0) {
    for (int i = x_pos - has_left; i <= x_pos + has_right; i++) {
      char read_char = map[y_pos - 1][i];
      if ('0' <= read_char && read_char <= '9') {
        //its a number. 
        uint16_t start = i;

        while(start > 0 && '0' <= map[y_pos - 1][start - 1] && map[y_pos - 1][start - 1] <= '9') {
          start--;
        }

        //get the number, then move i (the loop variable) to where we end up. 
        uint16_t number = 0;
        while(start < map_width && '0' <= map[y_pos - 1][start] && map[y_pos - 1][start] <= '9') {
          number *= 10;
          number += map[y_pos - 1][start] - '0';
          start++;
        }

        printf(" %d * ", number);

        gear_ratio *= number > 0 ? number : 1;
        adjacent_number_count++;

        i = start; //start is now end, maybe bad name of variable.
      }
    }
  }

  //check below
  if (y_pos < map_height - 1) {

    for (int i = x_pos - has_left; i <= x_pos + has_right; i++) {
      char read_char = map[y_pos + 1][i];

      if ('0' <= read_char && read_char <= '9') {
        //its a number. 
        uint16_t start = i;

        while(start > 0 && '0' <= map[y_pos + 1][start - 1] && map[y_pos + 1][start - 1] <= '9') {
          start--;
        }

        //get the number, then move i (the loop variable) to where we end up. 
        uint16_t number = 0;
        while(start < map_width && '0' <= map[y_pos + 1][start] && map[y_pos + 1][start] <= '9') {
          number *= 10;
          number += map[y_pos + 1][start] - '0';
          start++;
        }

        printf(" %d * ", number);

        gear_ratio *= number > 0 ? number : 1;
        adjacent_number_count++;

        i = start; //start is now end, maybe bad name of variable.
      }
    }
  }

  if (has_left) {
    char to_left = map[y_pos][x_pos - 1];
    if ('0' <= to_left && to_left <= '9') {
      uint16_t start = x_pos - 1;

      //loop to the left until beginning of the number
      while (start > 0 && '0' <= map[y_pos][start - 1] && map[y_pos][start - 1] <= '9') {
        start--;
      }

      uint16_t number = 0;
      while (start < map_width && '0' <= map[y_pos][start] && map[y_pos][start] <= '9') {
        number *= 10;
        number += map[y_pos][start] -'0';
        start++;
      }

      printf(" %d * ", number);

      gear_ratio *= number > 0 ? number : 1;
      adjacent_number_count++;
    }
  }

  if (has_right) {
    char to_right = map[y_pos][x_pos + 1];
    if ('0' <= to_right && to_right <= '9') {
      uint16_t start = x_pos + 1;

      uint16_t number = 0;
      while (start < map_width && '0' <= map[y_pos][start] && map[y_pos][start] <= '9') {
        number *= 10;
        number += map[y_pos][start] -'0';
        start++;
      }

      printf(" %d * ", number);

      gear_ratio *= number > 0 ? number : 1; //here ? 
      adjacent_number_count++;
    }
  }

  printf("Gear ratio: %d \n", adjacent_number_count > 1 ? gear_ratio : 0);

  //remove the symbol
  map[y_pos][x_pos] = '.';

  return adjacent_number_count > 1 ? gear_ratio : 0;
}


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

  uint64_t sum = 0;

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

  for(uint16_t i = 0; i < rows; i++) {
    for (uint16_t j = 0; j < columns; j++) {
      //if it is not a dot and it is not a number
      //then it is a symbol.
      if (map[i][j] != '.' && !('0' <= map[i][j] && map[i][j] <= '9') ) {
        sum += calc_gear_ratio(map, j, i, columns, rows);
      }
    }
  }
  printf("sum %d", sum);


  free(map);
  free(tmp);
}

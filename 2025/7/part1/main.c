#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "hashset/hashset.h"

//moves down until it is out of the map, 
//or until it hits a splitter - in that case it returns. 
uint32_t tachyon_dfs(uint32_t start_x, uint32_t start_y, char** map, uint8_t* seen, uint32_t map_width,  uint32_t map_height) {
  // size_t bit_index = start_y * map_width + start_x;
  // size_t byte_index = bit_index >> 3; // div by 8.
  // uint8_t mask = 1u << (bit_index && 7);
  
  uint32_t num_splitters = 0;

  uint32_t y = start_y;
  while(y < map_height) {
    size_t bit_index = y * map_width + start_x;
    size_t byte_index = bit_index >> 3; // div by 8.
    uint8_t mask = 1u << (bit_index & 7);
    if ((seen[byte_index] & mask) != 0) {
      return 0;
    }
    seen[byte_index] |= mask;
    if (map[y][start_x] == '^') {
      num_splitters += 1;
      if (start_x > 0) {
        num_splitters += tachyon_dfs(start_x - 1, y, map, seen, map_width, map_height);
      }
      if (start_x < map_width - 1) {
        num_splitters += tachyon_dfs(start_x + 1, y, map, seen, map_width, map_height);
      }
      break;
    }
    else {
      map[y][start_x] = '|';
    }

    y++;
  }

  return num_splitters;
}

int main()
{
  FILE *fptr = fopen("input.txt", "r");

  if (!fptr)
  {
    perror("failed to open file.");
    return 1;
  }

  int ch;
  uint32_t num_columns = 0;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      break;
    }
    num_columns++;
  }
  uint32_t num_rows = 1;
  int prev = -1;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      num_rows++;
    }
    prev = ch;
  }
  if (prev != '\n')
  {
    /* Last character before EOF was newline */
    num_rows++;
  }

  printf("num rows: %lu, num col: %lu\n", num_rows, num_columns);

  void* memory_arena = malloc(sizeof(char*) * num_rows + sizeof(char) * num_columns * num_rows);

  if (!memory_arena) {
    perror("Failed memory alloc.");
    fclose(fptr);
    return 1;
  }
  char** map = (char**) memory_arena;
  char *data_start = (char *)(map + num_rows);
  for (size_t i = 0; i < num_rows; i++) {
    map[i] = data_start + num_columns * i;
  }


  fseek(fptr, 0, SEEK_SET);
  uint32_t current_row = 0;
  uint32_t current_column = 0;
  uint32_t x_start;
  uint32_t y_start;
  puts("Loading map\n");
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      current_row++;
      current_column = 0;
      continue;
    }
    else if (ch == 'S') {
      x_start = current_column;
      y_start = current_row;
      map[current_row][current_column] = ch;
    }
    else {
      map[current_row][current_column] = ch;
    }

    current_column++;
  }
  printf("loaded map\n");

  uint8_t* seen = calloc(((num_columns * num_rows + 7) >> 3), sizeof(uint8_t));

  if (!seen) {
    perror("failed alloc for seen");
    free(memory_arena);
    return 1;
  }
  
  printf("running tachyon\n");
  uint32_t result = tachyon_dfs(x_start, y_start, map, seen, num_columns, num_rows);
  printf("after tachyon\n");


  //for (uint32_t i = 0; i < num_rows; i++) {
  //  for (uint32_t j = 0; j < num_columns; j++) {
  //    printf("%c", map[i][j]);
  //  }
  //  puts("\n");
  //}
  
  //9


  
  //uint16_t sum = 0;
  //for (uint32_t i = 0; i < num_columns; i++) {
  //  if (map[num_rows - 1][i] == '|') {
  //    sum++;
  //  }
  //}
  
  printf("num splits: %lu\n", result);



  
  free(memory_arena);
  fclose(fptr);
  return 0;
}
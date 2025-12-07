#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


//for part2. 
// we can no longer use the seen hash the same way
// 
// lets actually change perspecive completely. 
// lets go level by level -> we initialize a list of type uint32 for example, wiht as many elements as 
// the map is wide -> then we go level by level -> if we see a splitter '^' we add to the column left and right of it. 

//moves down until it is out of the map, 
//or until it hits a splitter - in that case it returns. 


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
      map[current_row][current_column] = ch;
    }
    else {
      map[current_row][current_column] = ch;
    }

    current_column++;
  }
  printf("loaded map\n");


  uint64_t *list_of_timelines = calloc(num_columns, sizeof(uint64_t));
  uint64_t *list = calloc(num_columns, sizeof(uint64_t));

  list_of_timelines[x_start] = 1;

  for (uint32_t row = 0; row < num_rows; row++) {
    //zero list each iteration. 
    for (uint32_t i = 0; i < num_columns; i++) {
      list[i] = 0;
    }
    for(uint32_t col = 0; col < num_columns; col++) {
      if (map[row][col] == '^') {
        if (col > 0) {
          list[col - 1] += list_of_timelines[col];
        }
        if (col < num_columns - 1) {
          list[col + 1] += list_of_timelines[col];
        }
      }
      else {
        // if the cell is not a splitter, timeline just continues straight down
        list[col] += list_of_timelines[col];
      }

    }
    for (uint32_t i = 0; i < num_columns; i++) {
      list_of_timelines[i] = list[i];
    }
    //for (uint32_t i = 0; i < num_columns - 1; i++) {
    //  if (map[row][i] == '^'){
    //    printf("^|");
    //  }
    //  else {
    //    printf("%llu|", list[i]);
    //  }
    //}
    //printf("%llu", list[num_columns - 1]);
    //puts("\n");
  }

  uint64_t sum = 0;
  for(uint32_t col = 0; col < num_columns - 1; col++) {
    //printf("%lu, ", list_of_timelines[col]);

    sum += list_of_timelines[col];
    if (sum < list_of_timelines[col]) {
      printf("overflow!!\n");
    }
  }
  printf("%" PRIu64 "\n", list_of_timelines[num_columns - 1]);
  sum += list_of_timelines[num_columns - 1];

  printf("sum: %" PRIu64 "\n", sum);

  free(list_of_timelines);
  free(list);
  free(memory_arena);
  fclose(fptr);
  return 0;
}
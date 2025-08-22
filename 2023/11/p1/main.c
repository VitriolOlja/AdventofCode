#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define UINT64_ABS_DIFF(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))

typedef struct Galaxy {
  uint32_t x;
  uint32_t y;
} Galaxy;

typedef struct GalaxyList {
  Galaxy* data;
  uint64_t current; 
  uint64_t capacity;
} GalaxyList;


bool initialize_galaxies(GalaxyList * galaxies, uint64_t initial_cap) {
  galaxies->data = malloc(initial_cap * sizeof(Galaxy));

  if(!galaxies->data) {
    printf("Failed malloc for galaxies");
    return false;
  }

  galaxies->current = 0;
  galaxies->capacity = initial_cap;
  return true;
}

bool add_galaxy(GalaxyList *galaxies, Galaxy *galaxy) {
  if (galaxies->current >= galaxies->capacity) {
    void * tmp = realloc(galaxies->data, galaxies->capacity * 2 * sizeof(Galaxy));

    if (!tmp ){
      printf("Failed realloc for galaxies");

      return false;
    }

    galaxies->capacity *= 2;
    galaxies->data = tmp;
  }

  galaxies->data[galaxies->current++] = *galaxy;
  return true;
}

int main(int argc, char *argv[])
{

  if (argc < 2)
  {
    printf("Usage: %s filename\n", argv[0]);
    return 1;
  }

  const char *file_name = argv[1];

  FILE *fptr = fopen(file_name, "r");
  if (!fptr)
  {
    printf("Failed to open file %s", file_name);
    return 1;
  }

  int ch;
  uint32_t column_count = 0;
  while ((ch = fgetc(fptr)) != EOF && ch != '\n')
  {
    column_count++;
  }
  uint32_t row_count = 2;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      row_count++;
    }
  }

  rewind(fptr);

  void *memory_arena = malloc(sizeof(char *) * row_count +
                              sizeof(char) * row_count * column_count);

  if (!memory_arena)
  {
    printf("Failed map malloc");
    fclose(fptr);
  }
  char **map = (char **)memory_arena;

  for (uint32_t i = 0; i < row_count; i++)
  {
    map[i] = (char *)memory_arena + sizeof(char *) * row_count + i * column_count;
  }

  // now lets read the file.
  uint32_t row = 0;
  uint32_t column = 0;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      row++;
      column = 0;
      continue;
    }
    map[row][column] = (char)ch;

    column++;
  }


  // count empty rows and columns
  uint8_t empty_rows[row_count];
  uint32_t sum_of_empty_rows = 0;
  for (uint32_t i = 0; i < row_count; i++)
  {
    bool row_empty = 1;
    for (uint32_t j = 0; j < column_count; j++)
    {
      if (map[i][j] == '#')
      {
        row_empty = 0;
        break;
      }
    }
    if (row_empty)
    {
      sum_of_empty_rows++;
    }
    empty_rows[i] = row_empty;
  }

  uint8_t empty_columns[column_count];
  uint32_t sum_of_empty_columns = 0;
  for (uint32_t i = 0; i < column_count; i++)
  {
    bool column_empty = 1;
    for (uint32_t j = 0; j < row_count; j++)
    {
      if (map[j][i] == '#')
      {
        column_empty = 0;
        break;
      }
    }
    if (column_empty)
    {
      sum_of_empty_columns++;
    }
    empty_columns[i] = column_empty;
  }

  uint32_t new_column_count = column_count + sum_of_empty_columns;
  uint32_t new_row_count = row_count + sum_of_empty_rows;

  void *new_memory_arena = malloc(sizeof(char *) * new_row_count +
                                  sizeof(char) * (new_row_count) * (new_column_count));

  if (!new_memory_arena)
  {
    printf("Failed map malloc");
    fclose(fptr);
    free(memory_arena);
  }
  char **new_map = (char **)new_memory_arena;

  for (uint32_t i = 0; i < new_row_count; i++)
  {
    new_map[i] = (char *)new_memory_arena + sizeof(char *) * new_row_count + i * new_column_count;
  }

  //create the new map with the expanded space. 
  uint32_t empty_skip_row = 0;
  for(uint32_t i = 0; i < row_count; i++) {
    uint32_t empty_skip_column = 0;
    for (uint32_t j = 0; j < column_count; j++) {
      new_map[i + empty_skip_row][j + empty_skip_column] = map[i][j];
      if (empty_columns[j]) {
        new_map[i + empty_skip_row][j + empty_skip_column + 1] = map[i][j]; //copy the current to the next location. 
        empty_skip_column++;
      }
    }
    //can i just memcpy the row to the next here if its an empty? 
    if (empty_rows[i]) {
      memcpy(new_map[i + empty_skip_row + 1], new_map[i+ empty_skip_row], new_column_count);
      empty_skip_row++;
    }
  }

  //attempt a print
  //for (uint32_t i = 0; i < new_row_count; i++) {
  //  for (uint32_t j = 0; j < new_column_count; j++) {
  //    printf("%c", new_map[i][j]);
  //  }
  //  puts("\n");
  //}

  
  GalaxyList galaxies;
  initialize_galaxies(&galaxies, 200);


  for (uint32_t i = 0; i < new_row_count; i++) {
    for (uint32_t j = 0; j < new_column_count; j++) {
      
      if (new_map[i][j] == '#'){
        //Found galaxy
        add_galaxy(&galaxies, &(Galaxy){.x = j, .y = i});
      }
    }
  }

  
  //now we need to loop over each pair of galaxies.


  uint64_t sum = 0;
  for (uint64_t first = 0; first < galaxies.current; first++) {
    for (uint64_t second = 0; second < galaxies.current; second++) {
      if (first < second) {
        uint64_t dx = UINT64_ABS_DIFF(galaxies.data[first].x, galaxies.data[second].x);
        uint64_t dy = UINT64_ABS_DIFF(galaxies.data[first].y, galaxies.data[second].y);

        sum += dx + dy;
      }
    }
  }

  printf("Sum of distances: %" PRIu64" \n", sum);




  fclose(fptr);
  free(memory_arena);
  free(galaxies.data);
  free(new_memory_arena);
}
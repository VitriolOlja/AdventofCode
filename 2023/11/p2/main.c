#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//hmm
//for part 2 we definitely cannot copy the map. 
//i think we got what we need anyways. 
//we can perhaps just see how many empty_columns and empty rows are between the two galaxies. 
//basically sum up them between the x and y of our galaxies and multiply it by our expansion constant.


#define UINT64_ABS_DIFF(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))


#define UINT64_MAX_COMP(a, b) (((a) > (b)) ? ((a)) : ((b)))
#define UINT64_MIN_COMP(a, b) (((a) <= (b)) ? ((a)) : ((b)))

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


//for p1 its 2, for p2  its 10 times, 100 times or 1000000
#define EXPANSION_CONSTANT 1000000

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

  
  GalaxyList galaxies;
  initialize_galaxies(&galaxies, 200);


  for (uint32_t i = 0; i < row_count; i++) {
    for (uint32_t j = 0; j < column_count; j++) {
      
      if (map[i][j] == '#'){
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

        //first the distance without any expansion.
        uint64_t dx = UINT64_ABS_DIFF(galaxies.data[first].x, galaxies.data[second].x);
        uint64_t dy = UINT64_ABS_DIFF(galaxies.data[first].y, galaxies.data[second].y);


        //sum up all empty space that is expanding in x
        uint64_t x_space_expanding = 0;
        for (uint64_t space = UINT64_MIN_COMP(galaxies.data[first].x, galaxies.data[second].x); space < UINT64_MAX_COMP(galaxies.data[first].x, galaxies.data[second].x); space++) {
          x_space_expanding += empty_columns[space]; //if true its 1 else 0. 
        }

        uint64_t y_space_expanding = 0;
        for (uint64_t space = UINT64_MIN_COMP(galaxies.data[first].y, galaxies.data[second].y); space < UINT64_MAX_COMP(galaxies.data[first].y, galaxies.data[second].y); space++) {
          y_space_expanding += empty_rows[space]; //if true its 1 else 0. 
        }



        
        sum += dx + dy + (x_space_expanding + y_space_expanding) * (EXPANSION_CONSTANT - 1);
      }
    }
  }

  printf("Sum of distances: %" PRIu64" \n", sum);




  fclose(fptr);
  free(memory_arena);
  free(galaxies.data);
}
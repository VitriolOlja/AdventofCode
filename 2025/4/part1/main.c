#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>

int main()
{
  FILE *fptr = fopen("input.txt", "r");

  int ch;

  uint16_t column_counter = 0;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      break;
    }
    column_counter++;
  }
  uint16_t row_counter = 2; // we are assuming it is atleast one row.
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      row_counter++;
      continue;
    }
  }
  //printf("row_count: %d, column count: %d\n", row_counter, column_counter);

  void *memory_arena = malloc(row_counter * sizeof(char *) + row_counter * column_counter * sizeof(char));

  if (!memory_arena)
  {
    printf("Failed map malloc");
    fclose(fptr);
    return 1;
  }

  char **map = (char **)memory_arena;
  for (uint32_t i = 0; i < row_counter; i++)
  {
    map[i] = (char *)memory_arena + sizeof(char *) * row_counter + i * column_counter;
  }

  fseek(fptr, 0, SEEK_SET);
  uint16_t column = 0;
  uint16_t row = 0;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      column = 0;
      row++;
      continue;
    }
    else
    {
      map[row][column] = (char)ch;
    }
    column++;
  }

  uint16_t sum = 0;
  for (uint16_t i = 0; i < row_counter; i++)
  {
    for (uint16_t j = 0; j < column_counter; j++)
    {
      // just look at the 8 positions around me.
      if (map[i][j] == '@')
      {
        uint8_t around = 0;

        if (i > 0)
        {
          // check position above
          if (map[i - 1][j] == '@')
          {
            around++;
          }

          if (j > 0)
          {
            if (map[i - 1][j - 1] == '@')
            {
              around++;
            }
          }
          if (j < column_counter - 1)
          {
            if (map[i - 1][j + 1] == '@')
            {
              around++;
            }
          }
        }
        if (i < row_counter - 1)
        {
          // check positions below
          if (map[i + 1][j] == '@')
          {
            around++;
          }

          if (j > 0)
          {
            if (map[i + 1][j - 1] == '@')
            {
              around++;
            }
          }
          if (j < column_counter - 1)
          {
            if (map[i + 1][j + 1] == '@')
            {
              around++;
            }
          }
        }
        if (j > 0)
        {
          if (map[i][j - 1] == '@')
          {
            around++;
          }
        }
        if (j < column_counter - 1)
        {
          if (map[i][j + 1] == '@')
          {
            around++;
          }
        }

        if (around < 4 ) {
          sum++;
        }
      }
    }
  }

  printf("sum: %d\n", sum);

  free(memory_arena);
  fclose(fptr);
  return 0;
}
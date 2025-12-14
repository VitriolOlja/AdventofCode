#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#define pow10(n) (pow10_table[n])

int main()
{
  FILE *fptr = fopen("input.txt", "r");

  int ch;
  // lets count what we need.
  uint32_t column_count = 0;
  while ((ch = fgetc(fptr)) != '\n')
  {
    column_count++;
  }
  uint32_t row_count = 1;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      row_count++;
    }
  }

  void *memory_arena = malloc(row_count * sizeof(char *) + column_count * row_count * sizeof(char));
  char **map = (char **)memory_arena;
  char *data_start = (char *)(map + row_count);
  for (uint16_t row = 0; row < row_count; row++)
  {
    map[row] = data_start + row * column_count;
  }

  fseek(fptr, 0, SEEK_SET);
  // printf("rows: %lu, columns: %lu\n", row_count, column_count);

  uint32_t current_column = 0;
  uint32_t current_row = 0;
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

  //for (size_t i = 0; i < row_count; i++)
  //{
  //  for (size_t j = 0; j < column_count; j++)
  //  {
  //    printf("%c", map[i][j]);
  //  }
  //  printf("\n");
  //}
  // we want to read column wise. until we have a empty column

  uint8_t multiplication = 1;
  uint64_t sum = 0;
  uint64_t current_number = 0;
  for (size_t j = 0; j < column_count; j++)
  {
    char last_row = map[row_count - 1][j];
    if (last_row == '*')
    {
      //printf("multiplication\n");
      multiplication = 1;
      current_number = 1;
    }
    else if (last_row == '+')
    {
      //printf("addition\n");
      multiplication = 0;
    }
    uint64_t column_number = 0;
    uint8_t all_space = 1;
    for (size_t i = 0; i < row_count - 1; i++)
    {
      if ('0' <= map[i][j] && map[i][j] <= '9')
      {
        all_space = 0;
        column_number = column_number * 10 + (map[i][j] - '0');
      }
    }
    //printf("column number: %llu\n", column_number);
    if (all_space)
    {
      //printf("current number %llu\n", current_number);
      sum += current_number;
      current_number = 0;
    }
    else
    {
      if (multiplication)
      {
        current_number *= column_number;
      }
      else
      {
        current_number += column_number;
      }
    }
  }

  //printf("current number %llu\n", current_number);
  sum += current_number;

  printf("sum: %llu", sum);

  free(memory_arena);
  return 0;
}
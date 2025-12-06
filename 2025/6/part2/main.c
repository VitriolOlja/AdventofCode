#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>


static const uint64_t pow10_table[] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
};

#define pow10(n) (pow10_table[n])

int main()
{
  FILE *fptr = fopen("input.txt", "r");

  int ch;
  // lets count what we need.
  uint32_t column_count = 0;
  uint8_t first = 1;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if ('0' <= ch && ch <= '9')
    {
      first = 1;
    }
    if (ch == ' ' && first)
    {
      column_count++;
      first = 0;
    }
    if (ch == '\n')
    {
      if (first)
      {
        column_count++;
      }
      break;
    }
  }
  uint32_t row_count = 1;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      row_count++;
    }
  }

  // printf("rows: %d, columns: %d\n", row_count, column_count);

  // not sure which direction is really best here
  //  for access sake - we will access the columns - so maybe its best to invert it,
  // but it is so short?
  void *memory_arena = malloc(column_count * sizeof(uint16_t *) + column_count * row_count * sizeof(uint16_t));
  uint16_t **map = (uint16_t **)memory_arena;
  uint16_t *data_start = (uint16_t *)(map + column_count);
  for (uint16_t col = 0; col < column_count; col++)
  {
    map[col] = data_start + col * row_count;
  }
  fseek(fptr, 0, SEEK_SET);

  uint32_t current_number = 0;
  uint32_t current_row = 0;
  uint32_t current_column = 0;
  first = 1;
  while ((ch = fgetc(fptr)) != EOF)
  {
    // printf("%c", ch);
    if (current_row >= row_count)
    {
      break;
    }
    else if ('0' <= ch && ch <= '9')
    {
      first = 1;
      current_number = current_number * 10 + (ch - '0');
    }
    else if (ch == ' ' && first)
    {
      // printf("saving1 %d to col: %d, row: %d\n", current_number, current_column, current_row);
      map[current_column][current_row] = current_number;
      // puts("saved1\n");
      current_number = 0;
      current_column++;
      first = 0;
    }
    else if (ch == '\n')
    {
      // printf("first: %d\n", first);
      if (first)
      {
        // printf("saving2 %d to col: %d, row: %d\n", current_number, current_column, current_row);
        map[current_column][current_row] = current_number; // crashes here.
        // puts("saved2\n");
        current_number = 0;
        first = 0;
      }
      current_column = 0;
      current_row++;
    }
  }

  //uint64_t biggest = 0;
  //for (int i = 0; i < column_count; i++) {
  //  for (int j = 0; j < row_count; j++) {
  //    if (map[i][j] > biggest) {
  //      biggest = map[i][j];
  //    }
  //  }
  //}
  //printf("biggest: %llu\n", biggest);

  // now we just need the last row of multiplication
  fseek(fptr, -10, SEEK_CUR);
  uint64_t sum = 0;
  current_column = 0;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '*')
    {
      uint64_t prod = 1;
      for (int digit = 0; digit < 5; digit++) {
        uint32_t column_number = 0;
        for (int i = 0; i < row_count; i++)
        { 
          uint16_t this_digit = (map[current_column][i] % pow10(digit + 1)) / pow10(digit);
          if (this_digit > 0) {
            column_number *= 10;
            column_number += this_digit;
          }
        }
        if (column_number > 0) {
          printf("column number: %lu\n", column_number);
          prod *= column_number;
        }
      }
      printf("prod: %llu\n", prod);
      sum += prod;
      current_column++;
    }
    else if (ch == '+')
    {
      uint64_t inside_sum = 0;
      for (int digit = 0; digit < 5; digit++) {
        uint32_t column_number = 0;
        for (int i = 0; i < row_count; i++)
        { 
          uint16_t this_digit = (map[current_column][i] % pow10(digit + 1)) / pow10(digit);
          if (this_digit > 0) {
            column_number *= 10;
            column_number += this_digit;
          }
        }
        if (column_number > 0) {
          printf("column number: %lu\n", column_number);
          inside_sum += column_number;
        }
      }
      sum += inside_sum;
      current_column++;
    }
  }

  printf("sum: %llu\n", sum);

  free(memory_arena);
  return 0;
}
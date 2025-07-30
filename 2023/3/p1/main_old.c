#include <stdio.h>
#include <stdint.h>

//I tried to employ a one pass strategy. 
//but somewhere around when I was going to search which numbers have symbols near them. 
// I realised my blunder. 

// iterating through the whole list of numbers, and then also through all symbols - will be a mess. n^2 time. 
// We could maybe utilize hashtables iterate through all numbers and then hash to get if there are any symbols around it. 
// but it would still be quite messy. 

#define FILE_NAME "input.txt"

typedef struct Number
{
  uint16_t value;
  uint16_t x_pos;
  uint16_t y_pos;
  uint8_t width; // we could get number of digits from the value by doing whole number division by 10.
  //- but saving the calculations and just keeping it might be worth it for simplicities sake.
} Number;

typedef struct Symbol
{
  uint16_t x_pos;
  uint16_t y_pos;
} Symbol;

#define MALLOC_INCREASE_WIDTH 100

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

  char ch;

  uint16_t numbers_allocated = MALLOC_INCREASE_WIDTH;
  uint16_t write_index_numbers = 0;
  Number *numbers = (Number *)malloc(numbers_allocated * sizeof(Number));

  if (!numbers)
  {
    printf("malloc numbers failed.");
    return 1;
  }

  uint16_t symbols_allocated = MALLOC_INCREASE_WIDTH;
  uint16_t write_index_symbols = 0;
  Symbol *symbols = (Symbol *)malloc(symbols_allocated * sizeof(Symbol));

  if (!symbols)
  {
    printf("malloc symbols failed.");
    return 1;
  }

  uint16_t number = 0;
  uint8_t number_width = 0;
  uint16_t row = 0;
  uint16_t column = 0;
  while ((ch = fgetc(fptr)) != EOF && ch != EOF)
  {
    if ('0' <= ch && ch <= '9')
    {
      // its a number we should start saving it.

      number *= 10;
      number += number - '0';
      number_width++;
    }
    if ('.' == ch)
    {
      if (number > 0)
      {

        // add to numbers
        if (write_index_numbers >= numbers_allocated)
        {
          uint32_t new_size = numbers_allocated + MALLOC_INCREASE_WIDTH;
          void *tmp = realloc(numbers, new_size);

          if (!tmp)
          {
            // failed alloc
            free(numbers);
            return 1;
          }

          numbers_allocated = new_size;
          numbers = tmp;
        }

        numbers[write_index_numbers].value = number;
        numbers[write_index_numbers].x_pos = column;
        numbers[write_index_numbers].y_pos = row;
        numbers[write_index_numbers].width = number_width;
        write_index_numbers++;
        number = 0;
        number_width = 0;
      }
    }
    else if (ch == '\n')
    {
      row++;
      column = 0;
      continue;
    }
    else
    {
      // add to symbols
      if (write_index_symbols >= symbols_allocated)
      {
        uint32_t new_size = symbols_allocated + MALLOC_INCREASE_WIDTH;
        void *tmp = realloc(symbols, new_size);

        if (!tmp)
        {
          // failed alloc
          free(symbols);
          return 1;
        }

        symbols_allocated = new_size;
        symbols = tmp;
      }

      symbols[write_index_symbols].x_pos = column;
      symbols[write_index_symbols].y_pos = row;
      write_index_symbols++;
      number = 0;
      number_width = 0;
    }
    column++;
  }

  uint32_t sum = 0; 

  free(numbers);
  free(symbols);
}
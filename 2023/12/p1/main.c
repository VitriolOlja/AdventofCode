#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "stringline.h"
#include "numarray.h"
#include "SpringHash.h"

uint32_t find_ways(StringLine *str, NumArray *contigous_damaged)
{
    // find the first '?'
    for (uint32_t i = 0; i < str->current; i++)
    {
        if (str->data[i] == '?')
        {
            str->data[i] = '#';
            uint32_t sum = find_ways(str, contigous_damaged);

            str->data[i] = '.';
            sum += find_ways(str, contigous_damaged);

            str->data[i] = '?'; // restore
            return sum;
        }
    }

    // base case: no '?', check damaged sequences
    uint32_t damaged = 0;
    uint8_t current_sequence = 0;
    uint8_t ptr = 0;

    for (uint32_t i = 0; i < str->current; i++)
    {
        if (str->data[i] == '#') { current_sequence++; damaged++; }
        if (str->data[i] != '#') {
            if (current_sequence > 0) {
                if (current_sequence > contigous_damaged->data[ptr]) return 0;
                current_sequence = 0;
                ptr++;
            }
        }
    }
    if (current_sequence > 0) {
        if (current_sequence > contigous_damaged->data[ptr]) return 0;
        ptr++;
    }

    // total damaged must match
    uint32_t total_damaged = 0;
    for (uint32_t i = 0; i < contigous_damaged->current; i++) total_damaged += contigous_damaged->data[i];
    if (damaged != total_damaged || ptr != contigous_damaged->current) return 0;

    return 1; // valid configuration
}


uint32_t longest_string = 0;
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

  // int ch;
  // uint32_t column_count = 0;
  // uint32_t row_count = 0;

  uint64_t arrangements = 0;

  // not the prettiest loop.
  while (!feof(fptr))
  {
    long line_start = ftell(fptr);
    int ch;
    uint32_t column_count = 0;

    // Step 1: Count characters in the string (up to space)
    while ((ch = fgetc(fptr)) != EOF && ch != ' ' && ch != '\n')
    {
      column_count++;
    }

    if (ch != ' ')
    {
      // Either empty line or malformed line; skip
      if (ch == '\n')
        continue;
      break; // EOF
    }

    // Step 2: Allocate string
    StringLine str;
    str.data = (char *)malloc(column_count);
    if (!str.data)
    {
      fclose(fptr);
      fprintf(stderr, "Failed malloc for str.data\n");
      return 1;
    }
    str.length = column_count;
    str.current = 0;

    // Step 3: Rewind to start of string and read it
    fseek(fptr, line_start, SEEK_SET);
    for (uint32_t i = 0; i < column_count; i++)
    {
      str.data[str.current++] = (char)fgetc(fptr);
    }

    if (longest_string < str.length)
    {
      longest_string = str.length;
    }

    // Step 4: Skip the space
    fgetc(fptr);

    // Step 5: Count numbers
    long numbers_start = ftell(fptr);
    uint32_t num_count = 1; // at least one number
    while ((ch = fgetc(fptr)) != EOF && ch != '\n')
    {
      if (ch == ',')
        num_count++;
    }

    // Step 6: Allocate number array
    NumArray numArray;
    numArray.data = (uint32_t *)malloc(num_count * sizeof(uint32_t)); // or NUM_ARRAY_TYPE // explicit casting should be fine in C, but debugger complained
    if (!numArray.data)
    {
      free(str.data);
      fclose(fptr);
      fprintf(stderr, "Failed malloc for numArray.data\n");
      return 1;
    }
    numArray.capacity = num_count;
    numArray.current = 0;

    // Step 7: Rewind and parse numbers
    fseek(fptr, numbers_start, SEEK_SET);
    uint32_t current_number = 0;
    while ((ch = fgetc(fptr)) != EOF && ch != '\n')
    {
      if (ch == ',')
      {
        numArray.data[numArray.current++] = current_number;
        current_number = 0;
      }
      else
      {
        current_number = current_number * 10 + ((uint32_t)ch - '0');
      }
    }
    numArray.data[numArray.current++] = current_number;

    SpringHash seen;
    initialize_hash(&seen);
    // Step 8: Call your processing function
    uint32_t arrangement = find_ways(&str, &numArray);
    printf("Arrangement: %" PRIu32 "\n", arrangement);

    arrangements += arrangement;

    // Step 9: Free memory
    hash_free(&seen);
    free(str.data);
    free(numArray.data);
  }

  // printf("Longest string: %" PRIu32, longest_string);
  printf("Arrangements: %" PRIu64 "\n", arrangements);

  rewind(fptr);
}
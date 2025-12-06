#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// idea maybe sort the ranges into a binary search tree?
// could a range be inside another range?
// then we have to check for each entry

// Another idea - just sort them based on the first number and
// then on each element also save how many 'after' it there are,
// or just the larger number aswell.
// if it is larger than the larger
// we continue
// if smaller than smallest we end check.

// nevermind lets just bruteforce it :)

// returns new count of elements.
//  map[i][0] = start
//  map[i][1] = end
//  map is already sorted by start
// 87605819227810
// 55202591019326
// 338345899055156
// 343482865498510
// 2^64 - 1 = 18,446,744,073,709,551,615 
//should not exactly overflow. 

uint64_t sum_fresh(uint64_t (*map)[2], size_t count)
{
  // now check for ranges to be merged.
  //for (size_t i = 0; i < count; i++) {
  //  printf("%llu - %llu\n", map[i][0], map[i][1]);
  //}
  size_t read = 0;
  uint64_t sum_fresh = 0;
  //printf("count: %llu\n", count);

  size_t initial = read;
  size_t max_read = initial;
  while (read < count - 1) {
    //printf("%llu", read);
    initial = read;
    max_read = initial;

    while (read < count - 1 && map[max_read][1] >= map[read + 1][0]) {
      if(map[read + 1][1] > map[max_read][1]) {
        max_read = read + 1;
      }
      read++;
    } 
    //int64_t this = map[max_read][1] - map[initial][0] + 1;
    //printf("this: %" PRId64 "\n", this);
    //since it is sorted - this should be the smallest in the list: map[initial][0]
    //since we checked- this must be the largest: map[max_read][1] 
    // so largest - smallest + 1 is the number of fresh ids. 
    printf("range: %llu - %llu\n", map[initial][0], map[max_read][1]);
    sum_fresh += map[max_read][1] - map[initial][0] + 1;
    
    read++;
  }
  if (map[max_read][0] < map[read][0]) {
    //*printf("read: %llu, initial: %llu\n", read, initial);
    //*printf("last range: %llu - %llu\n", map[count - 1][0], map[count - 1][1]);
    sum_fresh += map[read][1] - map[read][0] + 1;
  }
  
  return sum_fresh;
}

// not sure if it is enough values to actually merit a quicksort.
// lets just implement a n^2 sort. here is bubblesort.
void my_sort(uint64_t (*map)[2], size_t count)
{
  for (size_t i = 0; i < count - 1; i++)
  {
    uint8_t bubbled = 0;
    for (size_t j = 0; j < count - i - 1; j++)
    {
      if (map[j][0] > map[j + 1][0])
      {
        bubbled = 1;
        uint64_t tmp0 = map[j][0];
        uint64_t tmp1 = map[j][1];
        map[j][0] = map[j + 1][0];
        map[j][1] = map[j + 1][1];
        map[j + 1][0] = tmp0;
        map[j + 1][1] = tmp1;
      }
    }
    if (!bubbled)
    {
      break;
    }
  }
}

int main()
{

  // lets read to see how many there are first.
  FILE *fptr = fopen("input.txt", "r");
  int ch;
  uint8_t is_range = 0;
  uint32_t num_ranges = 0;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '-')
    {
      is_range = 1;
    }
    if (ch == '\n')
    {
      if (is_range)
      {
        num_ranges++;
        is_range = 0;
      }
      else
      {
        break;
      }
    }
  }
  // printf("num ranges: %d", num_ranges);
  uint64_t (*map)[2] = malloc(sizeof(uint64_t) * 2 * num_ranges);

  fseek(fptr, 0, SEEK_SET);
  uint32_t current_range = 0;
  uint64_t current_number = 0;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if ('0' <= ch && ch <= '9')
    {
      current_number *= 10;
      current_number += ch - '0';
    }
    if (ch == '-')
    {
      is_range = 1;
      map[current_range][0] = current_number;
      current_number = 0;
    }
    if (ch == '\n')
    {
      if (is_range)
      {

        map[current_range][1] = current_number;
        current_number = 0;
        current_range++;
        is_range = 0;
      }
      else
      {
        break;
      }
    }
  }

  // lets sort the ranges by start.

  my_sort(map, (size_t) num_ranges);

  //for (size_t i = 0; i < num_ranges; i++)
  //{
  //  printf("%llu - %llu\n", map[i][0], map[i][1]);
  //  // fresh += map[i][1] - map[i][0];
  //}
  //puts("\n");

  uint64_t fresh = sum_fresh(map, (size_t) num_ranges);

  printf("sum fresh range: %" PRIu64 "\n", fresh);

  // for (int i = 0; i < num_ranges; i++) {
  //   printf("%llu - %llu\n", map[i][0], map[i][1]);
  // }
  // uint64_t fresh = 0;

  // printf("fresh: %llu\n", fresh);

  free(map);
  fclose(fptr);
  return 0;
}
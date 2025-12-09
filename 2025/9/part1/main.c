#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>


uint64_t abs_diff_u64(uint64_t a, uint64_t b) {
    return (a > b) ? (a - b) : (b - a);
}

typedef struct Point2D {
  uint64_t x, y;
} Point2D;

int main() {
  FILE *fptr = fopen("input.txt", "r");

  int ch, last_ch;
  size_t row_count = 0;
  while((ch = fgetc(fptr)) != EOF) {
    if (ch == '\n') {
      row_count++;
    }
    last_ch = ch;
  }
  if (last_ch != '\n'){
    row_count++;  
  }

  size_t required_memory = sizeof(Point2D) * row_count;
  void* memory_arena = malloc(required_memory);

  Point2D *points = memory_arena;
  fseek(fptr, 0, SEEK_SET);

  size_t current_row = 0;
  uint64_t number = 0;
  while((ch = fgetc(fptr)) != EOF) {
    if (ch == '\n') {
      points[current_row].y = number;
      number = 0;
      current_row++;
    }
    if (ch == ',') {
      points[current_row].x = number;
      number = 0;
    }
    if ('0' <= ch && ch <= '9') {
      number = number * 10 + (ch - '0');
    }
    last_ch = ch;
  }
  if (last_ch != '\n'){
    points[current_row].y = number;
  }

  //for (size_t i = 0; i < row_count; i++) {
  //  printf("(%u, %u)\n", points[i].x, points[i].y);
  //}

  uint64_t largest = 0;
  uint64_t largest_side_x = 0;
  uint64_t largest_side_y = 0;
  for (size_t i = 0; i < row_count; i++) {
    for (size_t j = 0; j < row_count; j++) {
      if (i < j) {
        uint64_t x_side = abs_diff_u64(points[i].x, points[j].x) + 1;
        uint64_t y_side = abs_diff_u64(points[i].y, points[j].y) + 1;
        uint64_t area = x_side * y_side;
        if (area > largest) {
          largest = area;
          largest_side_x = x_side;
          largest_side_y = y_side;
        }
      }
  }
  }

  printf("largest: %llu : sides: %llu, %llu", largest, largest_side_x, largest_side_y);





  free(memory_arena);
  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "max_heap.h"
#include <strings.h>


//(98318 - 1834) * (98476 - 1828) / 8 / 1000000 = 1166 mb = 1.1 Gb of Ram
  // to allocate a bit lookup table for the map. that might be a bit much. 
/**
 * This one is hard. 
 * 
 * I am thinking, 
 * 
 * How does vector graphics work -> is there any way i can represent it more as a function? as lines perhaps?
 * what if i can break it down into lines segment -> where they are all 
 * so for example on row x, it is (4,10) and then from two numbers i know that all are filled between those? 
 * i think in general it will be a big area filled right? 
 * its still around 90k x values though. 
 * 
 * a #   #
 *      c#   #
 *   #       #b    for example between a and b here. 
 * there are values in between the c is lying in between the a and b on the y axis and the x
 * so it is "inside" the other. 
 * If there is a corner "inside" 
 * 
 * However this could be possible
 * 
 * Now today i was thinking back on the problem 2023 -> about an animal hiding in pipes. and we wanted to find the area 
 * surrounded by the pipes. I think i made an algorithm first that utilzedthe odd even rule, 
 * and something like that should be possible here.
 * 
 * I am thinking we for each point on the wall of the rectangle -> we check all the points
 * to the right. (this is a binary search of the sorted array). now we can find all
 * points to the right (or left does not matter) and then for each of those points check the previous
 * and the following node from that, if the y is greater than our (we check the direction up)
 * (we could maybe optimize by going to the closest wall. )
 * and then if there is a line between any points we count, and if there is an uneven amount of
 * "walls" then we are inside, but if its 
 * 
 * We might want to reconsider moving up -> perhaps down will be a better choice. due to how the coordinates
 * and boxes work. 
 */   



 /**
  *    x---x x---x   i aalso need to check these -> which only break the case when 
  *    |   | |   |
  *    |   x-x   |
  *    |         |
  *    x---------x
  */   
uint64_t abs_diff_u64(uint64_t a, uint64_t b) {
    return (a > b) ? (a - b) : (b - a);
}



typedef struct Point2D {
  uint64_t x, y;
} Point2D;

void switch_points(Point2D* a, Point2D* b) {
  uint64_t temp_x = a->x;
  uint64_t temp_y = a->y;
  a->x = b->x;
  a->y = b->y;
  b->x = temp_x;
  b->y = temp_y;
}
void switch_size_t(size_t* a, size_t *b) {
  size_t temp = *a;
  *a = *b;
  *b = temp;
}

uint8_t less_than_x(Point2D *a, Point2D* piv) {
  return a->x < piv->x;
}
uint8_t less_than_y(Point2D *a, Point2D* piv) {
  return a->y < piv->y;
}
uint8_t greater_than_x(Point2D *a, Point2D* piv) {
  return a->x > piv->x;
}
uint8_t greater_than_y(Point2D *a, Point2D* piv) {
  return a->y > piv->y;
}

/**
 * binary searches the list on x position. 
 * and returns the index of the position with the same x. 
 */
size_t binarySearch_x(Point2D *list, size_t length, Point2D p) {

  size_t low = 0;
  size_t high = length - 1; 
  size_t index;

  while(low <= high) {
    index = (low + high) / 2; 
    if (p.x == list[index].x) {
      //now we need to find the greatest with the same x. 
      while(list[index+1].x == p.x) {
        index++;
      }

      return index;
     }
     else if (p.x < list[index].x) {
      // we want the left side 
      high = index - 1;
     }
     else if (p.x > list[index].x) {
      low = index + 1;
     } 
  }
  return SIZE_MAX;
}

void sort_points_rec(Point2D* points, size_t length, size_t low, size_t high, uint8_t (*lt)(Point2D*, Point2D*), uint8_t (*gt)(Point2D*, Point2D*), 
      size_t* argsorted) {
  //pick
  if (high <= low) {
    return;
  }
  Point2D pivot = {points[low].x, points[low].y};

  size_t left = low;
  size_t right = high;

  while(1) {
    while (lt(&points[left], &pivot)) {
      left++;
    }
    while(gt(&points[right], &pivot)) {
      right--;
    }
    if (left >= right) {
      break;
    }

    switch_points(&points[left], &points[right]);
    switch_size_t(&argsorted[left], &argsorted[right]);
    
    left++;
    right--;
  }

  sort_points_rec(points, length, low, right, lt, gt, argsorted);
  sort_points_rec(points, length, right + 1, high, lt, gt, argsorted);
}


void sort_points_x(Point2D* points, size_t length, size_t* argsorted) {
  sort_points_rec(points, length, 0, length-1, &less_than_x, &greater_than_x, argsorted);
}
void sort_points_y(Point2D* points, size_t length, size_t* argsorted) {
  sort_points_rec(points, length, 0, length-1, &less_than_y, &greater_than_y, argsorted);
}

uint64_t get_max(uint64_t a, uint64_t b) {
  if (a > b) {
    return a;
  }
  return b;
}
uint64_t get_min(uint64_t a, uint64_t b) {
  if (a < b) {
    return a;
  }
  return b;
}

/**
 * this checks if point is inside the polygon by checking even odd rule.
 * if it crosses odd number of walls, it means it is a outside point. 
 * Point2D *sorted_x, size_t *argsorted_x 
 */
uint8_t is_point_in_polygon(Point2D p, Point2D *points, size_t length) {
  //size_t res = binarySearch_x(sorted_x, length, p);
  //printf("is point (%llu, %llu):\n", p.x, p.y);
  
  uint8_t inside = 0;
  for (size_t i = 0; i < length; i++) {
    Point2D A = points[i];
    Point2D B = points[(i + 1) % length];

    if (A.y == B.y) {
      continue; //skip horizontal lines
    }

    if (A.x == B.x) { // Vertical Wall
            uint64_t minY = get_min(A.y, B.y);
            uint64_t maxY = get_max(A.y, B.y);
            if (p.y > minY && p.y < maxY) {
                // Ray shoots to the RIGHT. Wall must be at an X greater than p.x
                if (A.x > p.x) {
                    //printf("Border at x %llu, from %llu to %llu)\n", A.x, minY, maxY );
                    inside = !inside;
                }
            }
        }
  }
  return inside;
}

//check on x direction (top, bottom) static y. 
uint8_t is_border_intact_x(Point2D* points, size_t length, uint64_t y, uint64_t min_x, uint64_t max_x ) {
  for (size_t i = 0; i < length; i++) {
    Point2D A = points[i];
    Point2D B = points[(i + 1) % length];

    if (A.x == B.x && A.x < max_x && A.x > min_x) { // vertical wall. lying between max and min x. 
      uint64_t minY = get_min(A.y, B.y);
      uint64_t maxY = get_max(A.y, B.y);
      if ((minY < y) && (maxY > y)) {
        //now i have a line between. 
        return 0;
      }
    }
  }
  return 1;
}
uint8_t is_border_intact_y(Point2D* points, size_t length, uint64_t x, uint64_t min_y, uint64_t max_y ) {
  for (size_t i = 0; i < length; i++) {
    Point2D A = points[i];
    Point2D B = points[(i + 1) % length];

    if (A.y == B.y && A.y < max_y && A.y > min_y) { // horizontal wall. lying between max and min y. 
      uint64_t minX = get_min(A.x, B.x);
      uint64_t maxX = get_max(A.x, B.x);
      if ((minX < x) && (maxX > x)) {
        //now i have a line between. 
        return 0;
      }
    }
  }
  return 1;
}


uint8_t is_rec_empty(Point2D *points, size_t length, uint64_t min_x, uint64_t max_x, uint64_t min_y, uint64_t max_y) {
  for (size_t i = 0; i < length; i++) {
    Point2D p = points[i];
    if (p.x > min_x && p.x < max_x && p.y > min_y && p.y < max_y) {
      return 0;
    }
  }
  return 1;
}



int main() {
  FILE *fptr = fopen("data/input.txt", "r");

  if (!fptr) {
    printf("Failed to open file input.txt\n");
  }

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
  void* memory_arena = malloc(required_memory * 3 + sizeof(size_t) * row_count * 2);

  if (!memory_arena) {
    printf("Failed malloc");
  }

  Point2D *points = memory_arena;
  Point2D *x_sorted = ((Point2D *)memory_arena) + row_count;
  Point2D *y_sorted = ((Point2D *)x_sorted) + row_count;
  size_t *x_argsorted = (size_t *)(y_sorted + row_count);
  size_t *y_argsorted = ((size_t *)x_argsorted) + row_count;



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

  memcpy(x_sorted, points, required_memory);
  memcpy(y_sorted, points, required_memory);


  for (size_t i = 0; i < row_count; i++) {
    x_argsorted[i] = i;
  }
   for (size_t i = 0; i < row_count; i++) {
    y_argsorted[i] = i;
  }



  //use a max heap and then we use it pop until we find a rectangle that is viable.
  //we probably only need around 100 rectangles, if none of those are viable- > then i guess we increase later on. 
  //if it is too slow we can limit it like that later -> but maybe we can just use a max heap.
  MaxHeap heap;
  initialize_max_heap(&heap, row_count * row_count);
  for (size_t i = 0; i < row_count; i++) {
    for (size_t j = 0; j < row_count; j++) {
      if (i < j) {
        uint64_t x_side = abs_diff_u64(points[i].x, points[j].x) + 1;
        uint64_t y_side = abs_diff_u64(points[i].y, points[j].y) + 1;
        uint64_t area = x_side * y_side;
        
        add_to_max_heap(&heap, area, i, j);
      }
    }
  }


  sort_points_x(x_sorted, row_count, x_argsorted);
  sort_points_y(y_sorted, row_count, y_argsorted);

  while(!is_empty(&heap)) {
    MaxHeapNode node = max_heap_pop(&heap);

    //for each point on the rectangle. 
    Point2D point_a = points[node.index_a];
    Point2D point_b = points[node.index_b];

    //Failed: (9, 5) - (2, 3) area: {24} 
    //if (point_a.x != 9 || point_a.y != 5 || point_b.x != 2 || point_b.y != 3) {
    //  continue;
    //}

    //check minimum x and maximum x.
    uint64_t min_x = get_min(point_a.x, point_b.x);
    uint64_t min_y = get_min(point_a.y, point_b.y);

    uint64_t max_x = get_max(point_a.x, point_b.x);
    uint64_t max_y = get_max(point_a.y, point_b.y);

    uint8_t top_left = is_point_in_polygon((Point2D){min_x, min_y}, points, row_count);
    uint8_t bottom_left = is_point_in_polygon((Point2D){min_x, max_y}, points, row_count);
    uint8_t top_right = is_point_in_polygon((Point2D){max_x, min_y}, points, row_count);
    uint8_t bottom_right = is_point_in_polygon((Point2D){max_x, max_y}, points, row_count);
    
    if (top_left &&
        bottom_left &&
        top_right &&
        bottom_right
    ) {
      if (
        is_border_intact_x(points, row_count, min_y, min_x, max_x) &&
        is_border_intact_x(points, row_count, max_y, min_x, max_x) &&
        is_border_intact_y(points, row_count, min_x, min_y, max_y) &&
        is_border_intact_x(points, row_count, max_x, min_y, max_y) 
        ) {
        //lets check that there is no point within the rectangle aswell. 
        uint8_t rec_is_empty = is_rec_empty(points, row_count, min_x, max_x, min_y, max_y);

        if (rec_is_empty) {
          printf("found: (%llu, %llu) - (%llu, %llu) area: {%llu} \n", point_a.x, point_a.y, point_b.x, point_b.y, node.area);
        }
        continue;
      }
    }

  }



  free_max_heap(&heap);
  free(memory_arena);
  fclose(fptr);
  return 0;
}
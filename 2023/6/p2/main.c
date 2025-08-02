/**
 * 
 * distance travelled = time_held * time_in_race > record_dist
 * 
 * total_time = time_held + time_in_race < record_time
 * 
 * time_in_race < record_time - time_held
 * 
 * time_held * (record_time - time_held) > record_dist // the product should be greater than before. 
 * 
 * time_held * record_time - time_held^2 > record_dist
 * 
 * time_held^2 - time_held * record_time + record_dist < 0
 * 
 * time_held = record_time / 2 +- sqrt( ((record_time/2)^2) - record_dist ) 
 */

 /**
  * for part 2 i dont think we should have to change that much except maybe the
  * data types and the way we read the file? 
  */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#define FILE_NAME "input.txt"

#define D_LIST_DATA_TYPE uint16_t
typedef struct d_list {
  size_t capacity;
  D_LIST_DATA_TYPE *data;
  size_t current;
} d_list;

/**
 * Warning this will malloc. remember to free. 
 */
void init_list(d_list* dl, size_t capacity) {
  dl->capacity = capacity;
  dl->data = malloc(sizeof(D_LIST_DATA_TYPE) * dl->capacity);

  if (!dl->data) {
    printf("Failed to allocate memory for list.");
  }
  dl->current = 0;
}

/**
 * Appends new_data to dynamic list, if needed it reallocates memory, 
 * if that realloc is unsuccessfull , then false is returned. 
 */
uint8_t append_to_list(d_list *dl, D_LIST_DATA_TYPE new_data) {
  if (dl->current >= dl->capacity) {
    size_t new_capacity = dl->capacity * 2;
    void *tmp = realloc(dl->data, new_capacity * sizeof(D_LIST_DATA_TYPE));

    if (!tmp) {
      return 0; // realloc failed
    }

    dl->data = tmp;
    dl->capacity = new_capacity; // only update after success
  }

  dl->data[dl->current++] = new_data;
  return 1;
}

void free_list(d_list* dl){
  free(dl->data);
}

int main(void) {
  FILE* fptr = fopen(FILE_NAME, "r");

  if (!fptr) {
    printf("Failed to open file: %s", FILE_NAME);
  }

  d_list times;
  init_list(&times, 10);
  d_list distances;
  init_list(&distances, 10);

  char ch;
  while((ch = fgetc(fptr)) != EOF && ch != ':');
  //read times.
  D_LIST_DATA_TYPE num;
  while (fscanf(fptr, "%" SCNu16, &num) == 1) {
    if (!append_to_list(&times, num)) {
      printf("Memory allocation failed for times!");
    }
  }

  while((ch = fgetc(fptr)) != EOF && ch != ':');

  while (fscanf(fptr, "%"SCNu16, &num) == 1) {
    //printf("Trying to save %" PRIu16 "\n", num);
    if (!append_to_list(&distances, num)) {
      printf("Memory allocation failed for distances!\n");
    }
  }

  // printf("Times: ");
  // for (int i = 0; i < times.current; i++) {
  //   printf("%d ", times.data[i]);
  // }
  // printf("\nDistances: ");
  // 
  // for (int i = 0; i < distances.current; i++) {
  //   printf("%d ", distances.data[i]);
  // }
  // printf("\n");

  //sum up times to one time. 

  uint64_t time = 0;
  uint16_t digit = 0;
  for (int i = 0; i < times.current; i++) {
    uint16_t t = times.data[i];

    //we count the digits of this numnber for each digit we multiply, then add. 

    while (t > 0 ) {
      time *= 10;
      t /= 10;
    }
    time += times.data[i];
  }

  uint64_t distance = 0;
  digit = 0;
  for (int i = 0; i < distances.current; i++) {
    uint16_t d = distances.data[i];

    //we count the digits of this numnber for each digit we multiply, then add. 

    while (d > 0 ) {
      distance *= 10;
      d /= 10;
    }
    distance += distances.data[i];
  }

  //time_held = record_time / 2 +- sqrt( ((record_time/2)^2) - record_dist ) 
  double record_time = time;
  double time_held_2 =  record_time / 2 + sqrt(pow(record_time / 2, 2) - distance);
  double time_held_1 =  record_time / 2 - sqrt(pow(record_time / 2, 2) - distance);

  uint64_t ceiling_1 = ceil(time_held_1);

  uint64_t ceiling_2 = ceil(time_held_2);

  uint64_t ways_of_getting_record = ceiling_2 - ceiling_1;

  if (ceiling_2 = time_held_2 && ceiling_1 == time_held_1){
    ways_of_getting_record -= 1;
  }
  
  printf("Ways: %d \n", ways_of_getting_record);

  free_list(&times);
  free_list(&distances);

  return 0;
}
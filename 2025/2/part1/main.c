#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>


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


// We only need to check those of even length. 
//this should be able to also return early. 
uint64_t is_a_double(uint64_t n) {
  uint8_t digits = 1;
  uint64_t d = n; 
  while (d > 0) {
    d /= 10;
    digits++;
  }
  uint64_t x = pow10((digits / 2));
  return (digits & 1) && ((n % x) == (n / x));
}

/**
 * returns sum.
 */
uint64_t bruteforce(uint64_t start, uint64_t end) {
    uint64_t sum = 0;
    printf("finding for range: %llu - %llu\n", start, end);
    for (uint64_t i = start; i <= end; i++) {
        if (is_a_double(i)) {
            printf("Invalid id: %llu\n", i);
            sum += i;
        }
    }

   
    return sum;
}

int main() {
  FILE *fptr = fopen("input.txt", "r");

  int ch;

  uint8_t is_reading_second_number = 0;
  uint64_t first_number = 0;
  uint64_t second_number = 0;
  uint8_t current_digit = 0;
  uint64_t sum = 0;
  while((ch = fgetc(fptr)) != EOF) {

    if (ch == '-') {
      is_reading_second_number = 1;
      current_digit = 0;
    }
    else if (ch == ',') {
      is_reading_second_number = 0;
      current_digit = 0;
      //logic for calculating biggest range
      uint64_t range = second_number - first_number;
      //if (range > biggest_range) {
      //  biggest_range = range;
      //}

      sum += bruteforce(first_number, second_number);
      
      
      first_number = 0;
      second_number = 0;
    }
    else if (ch == '\n') {
      continue;
    }
    else if (ch >= '0' && ch <= '9')  {
      if (!is_reading_second_number) { //reading first number
        first_number *= 10;
        first_number += (ch - '0');
        current_digit++;
      } else { //reading second number
        second_number *= 10;
        second_number += (ch - '0');
        current_digit++;
      }
    }
  }
  sum += bruteforce(first_number, second_number);
  //printf("sum: %llu\n", sum);
  printf("sum: %" PRIu64 "\n", sum);
  //printf("biggest range: %d\n", biggest_range);

  fclose(fptr);
  return 0;
}
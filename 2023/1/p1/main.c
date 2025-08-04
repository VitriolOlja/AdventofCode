#include <stdio.h>
#include <stdint.h>

int main(void)
{
  FILE *fptr = fopen("input.txt", "r");
  char ch = 'a'; //just something that is not accidentally EOF.

  uint8_t first_number = 0;
  uint8_t second_number = 0;
  uint16_t sum = 0;
  uint8_t numbers = 0;
  while (ch != EOF)
  {
    while ((ch = fgetc(fptr)) != EOF)
    {
      // check if number
      if (ch >= '0' && ch <= '9')
      {
        first_number = (uint8_t)(ch - '0');
        second_number = first_number;
        break;
      }
    }
    while ((ch = fgetc(fptr)) != EOF)
    {
      // check if number
      if (ch >= '0' && ch <= '9')
      {
        second_number = (uint8_t)(ch - '0');
      }
      if (ch == '\n')
        break;
    }

    uint16_t this_number = first_number * 10 + second_number;

    printf("number %d is equal to : %d \n", numbers, this_number);
    sum += this_number;
    numbers++;
  }

  printf("sum: %d", sum);
  fclose(fptr);
}
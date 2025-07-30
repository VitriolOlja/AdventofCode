#include <stdio.h>
#include <stdint.h>

#define MAX_RED 12
#define MAX_BLUE 14
#define MAX_GREEN 13
#define FILE_NAME "input.txt"

enum COLOR
{
  RED,
  GREEN,
  BLUE,
  UNKNOWN_COLOR
};

// i realise fscanf might have been better.
uint8_t read_number(FILE *fptr)
{
  char ch;
  uint8_t number = 0;
  while ((ch = fgetc(fptr)) != EOF && ch != ' ')
  {
    if (ch >= '0' && ch <= '9')
    {
      number = number * 10 + (ch - '0');
    }
  }
  return number;
}

enum COLOR get_color(FILE *fptr)
{
  char ch;
  // while its not a letter just discard everything.
  while (((ch = fgetc(fptr)) != EOF) && (ch < 'a' || ch > 'z'))
    ;

  //printf("character is: '%c' \n", ch);
  switch (ch)
  {
  case ('r'):
    return RED;
    break;
  case ('g'):
    return GREEN;
    break;
  case ('b'):
    return BLUE;
    break;

  default:
    // should never happen..
    printf("What should never have happened happened. \n");
    return UNKNOWN_COLOR;
  }
}

int main(void)
{
  FILE *fptr = fopen(FILE_NAME, "r");
  char ch = 'a';

  uint16_t game = 1;
  uint32_t id_sum = 0;

  if (!fptr)
  {
    printf("Could not read file %s\n", FILE_NAME);
    return 1;
  }

  
  while (ch != EOF)
  {
    // do for each line
    uint8_t reds = 0;
    uint8_t blues = 0;
    uint8_t greens = 0;


    while (((ch = fgetc(fptr)) != EOF) && ch != ':')
      ; // Discard everything up to ':'

    while (ch != EOF && ch != '\n' )
    {

      fseek(fptr, 1, SEEK_CUR); // discard the space

      // read up until first letter and save each number

      uint8_t number = read_number(fptr);
      //printf("Read number: '%d'\n", number);

      enum COLOR color = get_color(fptr);

      switch (color)
      {
      case (RED):
        if (number > reds){
          reds = number;
        }
        break;

      case (BLUE):
        if (number > blues){
          blues = number;
        }
        break;

      case (GREEN):
        if (number > greens){
          greens = number;
        }
        break;
      }

      while (((ch = fgetc(fptr)) != EOF) && ch != ';' && ch != ',' && ch != '\n')
        ; // Discard everything up to ',' or ';'
      // check if ch is
    }

    //printf("Game %d: Sum reds:  %d, Sum greens:  %d, Sum blues:  %d \n", game, reds, greens, blues);

    if (reds <= MAX_RED && greens <= MAX_GREEN && blues <= MAX_BLUE) {
      id_sum += game;
    }
    game++;
  }

  printf("sum: %d", id_sum);
  return 0;
}
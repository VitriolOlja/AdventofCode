#include <stdint.h>
#include <stdio.h>

//im thinking i can make a bitmap and just and them together. 
//it needs to be 100 bits long. so i will need a uint128
// Its not certain such a big integer exists on many systems.
// but on msys2 ucrt64 gcc it does. 
// and should on most linux systems. 


#define FILE_NAME "input.txt"

/**
 * Don't think we need to count actually. 
 */
void count_rows_and_columns(FILE *fptr, uint16_t *columns, uint16_t *rows) {
  long pos = ftell(fptr);

  rewind(fptr);
  *columns = 0;

  char ch;
  while ((ch = fgetc(fptr)) != EOF && ch != '\n'){
    (*columns)++;
  }
  *rows = 2;  
  //there might not be a newline at the end of the file. 
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      (*rows)++;
    }
  }
  fseek(fptr, pos, SEEK_SET);
}

int main(void) {

  FILE *fptr = fopen(FILE_NAME, "r");

  if (!fptr)
  {
    printf("Could not open file. \n");
    return 1;
  }

  uint16_t columns, rows;
  count_rows_and_columns(fptr, &columns, &rows);


  //dynamic list of 128 bits. 
  //__uint128_t * winning_numbers = malloc(sizeof(__uint128_t) * rows);

  
  uint32_t counts[rows]; //will not work with all c compilers. 

  char ch = 'a';

  uint16_t current_row = 0;
  while (ch != EOF) {
    

    while ((ch = fgetc(fptr)) != EOF && ch != ':'); //just skip til :

    int num;

    __uint128_t numbers = 0;
   
    // Read numbers until the '|' 
    while (fscanf(fptr, "%d", &num) == 1) {
      ch = fgetc(fptr);
      if (ch == '|') {
          break;
      }
      ungetc(ch, fptr); // put it back if not '|'

      //winning numbers
      numbers |= ((__uint128_t)1 << (num - 1));
    }

    //printf("%c", fgetc(fptr));

    uint16_t count = 0;
    
    fgetc(fptr);
    while (fscanf(fptr, "%d", &num) == 1) {
      if (numbers & ((__uint128_t)1 << (num - 1)) ) {
        count++; // multiply by 2.
        //printf("%d  ", num);
      }

      ch = fgetc(fptr);
      if (ch == '\n') {
        break;
      }
      
      ungetc(ch, fptr); // put it back if not '\n'
    }

    //printf("\n Count: %d \n", count);

    counts[current_row] = count;
    current_row++;
  }

  uint32_t multipliers[rows];
  
  for(int i = 0; i < rows; i++ ) {
    multipliers[i] = 1;
  }

  //i loop through each counts of crrectly guessed cards. 
  for(int i = 0; i < rows; i++ ) {
    // for each card - i check how many matches (counts[i])
    // and for c = 0, 1, ..., counts[i] 
    // we check for i + c + 1 which is counts[i] cards after the original. 
    // and for all those we add however many cards we have on the original position
    for (int c = 0; c < counts[i]; c++) {
      multipliers[i + c + 1] += multipliers[i]; 
    }
  }

  uint32_t sum = 0;
  for(int i = 0; i < rows; i++) {
    sum += multipliers[i];
    //printf("%d \n", multipliers[i]);
  }

  printf("sum: %d \n", sum);
  
  return 0;
}
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
}

int main(void) {

  FILE *fptr = fopen(FILE_NAME, "r");

  // int **array;
  // size_t array_height = 0;
  // size_t array_width = 0;

  if (!fptr)
  {
    printf("Could not open file. \n");
    return 1;
  }

  //count_rows_and_columns(fptr, &columns, &rows);

  //dynamic list of 128 bits. 
  //__uint128_t * winning_numbers = malloc(sizeof(__uint128_t) * rows);

  
  uint32_t sum = 0;
  char ch = 'a';
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

    uint16_t count = 1;
    
    fgetc(fptr);
    while (fscanf(fptr, "%d", &num) == 1) {
      if (numbers & ((__uint128_t)1 << (num - 1)) ) {
        count <<= 1; // multiply by 2.
        //printf("%d  ", num);
      }

      ch = fgetc(fptr);
      if (ch == '\n') {
        break;
      }
      
      ungetc(ch, fptr); // put it back if not '\n'
    }
    count >>= 1; //divide by 2. 

    //printf("\n Count: %d \n", count);

    sum += count;
  }
  
  printf("sum: %d ", sum);
  fclose(fptr);
  return 0;
}
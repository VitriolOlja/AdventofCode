#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>


//could it be that xor itself just undoes itself. 
//so either we press a button or we dont press a button. 
//There is not double press a button. 
//this way we can search for the minimal combination of button presses. 

void printBinary(uint32_t n) {
    for (int i = 16; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
}

uint32_t countSetBits(uint32_t num)
{
  uint32_t count = 0;
  while (num) {
      count += num & 1; // Add the least significant bit
      num >>= 1; // Right shift the number
  }
  return count;
}

uint32_t reverseBits(uint32_t input, size_t numBits) {
  uint32_t result = 0;

  for (size_t bit = 0; bit < numBits && bit < 32; bit++) {
    result <<= 1;
    result += (input & 1);
    input >>= 1;
  }

  return result;
}

//buttons 
//hopefully 20 buttons is max. 
#define MAXIMUM_BUTTONS 20

/** returns the fewest amout of buttons presses.  */
uint32_t handle_machine(uint32_t light_diagram, const uint32_t buttons[MAXIMUM_BUTTONS], size_t buttons_length) {
  //printf("\n");
  //printf("[");
  //printBinary(light_diagram);
  //printf("]");
  //for (size_t i = 0; i < buttons_length; i++) {
  //  printf(" (");
  //  printBinary(buttons[i]);
  //  printf(" )");
  //}
  //printf("\n");

  //printf("%lu", buttons_length);

  uint32_t fewest_presses = UINT32_MAX;

  for (uint32_t i = 0; i < (1 << buttons_length); i++) {
    uint32_t result = 0; 
    
    uint32_t t = i;
    size_t current_button = 0;
    while(t > 0) {
      if (t & 1) {
        result ^= buttons[current_button];
      }
      t >>= 1;
      current_button++;
    }
    
    if (result == light_diagram) {
      //we found a potential solution. 
      //count bits
      uint32_t presses = countSetBits(i);
      if (fewest_presses > presses) {
        fewest_presses = presses;
      }
    }

  }

  return fewest_presses;
}

int main() {
  FILE *fptr = fopen("input.txt", "r");

  int ch;
  int last_ch;
  size_t row_count = 0;

  uint32_t light_diagram = 0; //uint16 would be enough but whatever.
  uint32_t num_lightbulbs = 0;

 
  uint32_t sum_of_presses = 0;
  uint32_t buttons[MAXIMUM_BUTTONS] = {0};
  size_t current_button = 0;
  size_t current_digit = 0;

  while((ch = fgetc(fptr)) != EOF) {
    if (ch == '#') {
      //# means on
      light_diagram = (light_diagram << 1) + 1;
      num_lightbulbs++;
    }
    else if (ch == '.') {
      //. means off
      light_diagram = (light_diagram << 1);
      num_lightbulbs++;
    }
    else if(ch == ']') {
      light_diagram = reverseBits(light_diagram, num_lightbulbs);
    }
    else if (ch == ')') {
      buttons[current_button] |= (1 << current_digit);
      current_button++;
      current_digit = 0;
    }
    else if('0' <= ch && ch <= '9') {
      current_digit = current_digit * 10 + (ch - '0');
    }
    else if(ch == ',') {
      buttons[current_button] |= (1 << current_digit);
      current_digit = 0;
    }

    
    else if (ch == '\n') {
      if (current_button >= MAXIMUM_BUTTONS ) {
        printf("Major error, we misjudged the required amount of buttons please recalibrate.");
        fclose(fptr);
        return 1;
      }
      //this is where we send the information to handle machine function. 
      uint32_t presses = handle_machine(light_diagram, buttons, current_button);
      //printf("press: %u\n", presses);
      sum_of_presses += presses;

      for (size_t i = 0; i < MAXIMUM_BUTTONS; i++) {
        buttons[i] = 0;
      }
      current_button = 0;
      current_digit = 0;
      light_diagram = 0;
      num_lightbulbs = 0;


      // and reset the values for next iteration. 

    }
  }

  printf("sum of presses: %u\n", sum_of_presses);

  fclose(fptr);
  return 0;
}
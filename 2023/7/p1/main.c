#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

// we can just keep one large list -> but that seems messy
// Hands are primarily sorted on type, so it seems best separate types in their own structures.
// perhaps we have one dynamic list for each type.
//  I think this is the best i can come up for for part 1.

#define CARDS_ON_HAND 5
#define CARD_TYPES 13
/**
 * A = 12 = 0b1100
 * K = 11 = 0b1011
 * Q = 10 = 0b1010
 * J = 9  = 0b1001
 * T = 8  = 0b1000
 * 9 = 7  = 0b0111
 * 8 = 6  = 0b0110
 * 7 = 5  = 0b0101
 * 6 = 4  = 0b0100
 * 5 = 3  = 0b0011
 * 4 = 2  = 0b0010
 * 3 = 1  = 0b0001
 * 2 = 0  = 0b0000
 *
 * Types are (7 types) only needs 3 bits.
 *
 * Five of a kind     = 0b110
 * Four of a kind     = 0b101
 * Full house         = 0b100
 * Three of a kind    = 0b011
 * Two Pair           = 0b010
 * One Pair           = 0b001
 * High Card          = 0b000
 */
#define GET_TYPE(x) (((x) >> 61) & 0b111)
#define FIVE_OF_A_KIND 0b110
#define FOUR_OF_A_KIND 0b101
#define FULL_HOUSE 0b100
#define THREE_OF_A_KIND 0b011
#define TWO_PAIR 0b010
#define ONE_PAIR 0b001
#define HIGH_CARD 0b000

static inline uint8_t char_to_card(char c)
{
  if (c >= '2' && c <= '9')
  {
    return c - '2';
  }
  switch (c)
  {
  case 'T':
    return 8;
  case 'J':
    return 9;
  case 'Q':
    return 10;
  case 'K':
    return 11;
  case 'A':
    return 12;
  default:
    return 0xFF; // not valid.
  }
}

static inline char card_to_char(uint8_t c)
{
  if (c < 8)
  {
    return '2' + c;
  }
  switch (c)
  {
  case 8:
    return 'T';
  case 9:
    return 'J';
  case 10:
    return 'Q';
  case 11:
    return 'K';
  case 12:
    return 'A';
  default:
    return 'X';
  }
}

typedef struct STACK_NODE
{
  size_t low;
  size_t high;
} STACK_NODE;

typedef struct STACK
{
  STACK_NODE *base;
  size_t current;
  size_t capacity;
} STACK;

uint8_t initalize_stack(STACK *stack, size_t initial_capacity)
{
  stack->current = 0;

  stack->base = malloc(initial_capacity * sizeof(STACK_NODE));

  if (!stack->base)
  {
    printf("Malloc failed when initializing stack.");
    return 0;
  }

  stack->capacity = initial_capacity;

  return 1;
}

void stack_push(STACK *stack, STACK_NODE *node)
{
  if (stack->capacity <= stack->current)
  {
    // realloc
    void *tmp = realloc(stack->base, stack->capacity * 2 * sizeof(STACK_NODE));

    if (!tmp)
    {
      printf("Failed realloc when pushing to stack. ");
      return;
    }

    stack->base = tmp;
    stack->capacity *= 2;
  }
  //stack->base[stack->current].high = node->high;
  //stack->base[stack->current].low = node->low;
  stack->base[stack->current] = *node;
  stack->current++;
}

STACK_NODE stack_pop(STACK *stack)
{
  stack->current--;
  return stack->base[stack->current];
}

void free_stack(STACK *stack)
{
  free(stack->base);
}

void swap(uint64_t *a, uint64_t *b)
{
  uint64_t tmp = *a;
  *a = *b;
  *b = tmp;
}

void quicksort(uint64_t *list, size_t low, size_t high)
{
  // initialize a stack list.
  STACK stack;
  initalize_stack(&stack, 200);

  // node on this stack for pushing to stack.
  STACK_NODE pushNode; 
  pushNode.high = high;
  pushNode.low = low;
  stack_push(&stack, &pushNode); //push the initial subarray on to the stack.

  while (stack.current > 0)
  {
    STACK_NODE currentNode = stack_pop(&stack);

    size_t pivot_index = (currentNode.high - currentNode.low) / 2 + currentNode.low;
    uint64_t pivot_value = list[pivot_index];
    //printf("Pivot index: %zu, Pivot value: %" PRIu64 "\n", pivot_index, pivot_value); // Debug print

    // now for inplace sorting.
    // we use a left and right ptr
    int left = (int)currentNode.low;
    int right = (int)currentNode.high;

    // move left to the right until we find a value that should be on the right.
    while (left <= right)
    {
      while (left <= right && list[left] < pivot_value) {
        left++;
      }
      while (left <= right && list[right] > pivot_value) {
        right--;
      }

      if (left <= right)
      {
        //printf("Swapping elements at indices %zu and %zu\n", left, right);
        //printf("Left element: %" PRIu64 ", Right element: %" PRIu64 "\n", list[left], list[right]);
        swap(&list[left], &list[right]);
        right--;
        left++;
        //printf("Swapped elements at indices %zu and %zu\n", left, right); // Debug print
      }
    }
    
    //printf("Partitioning result: left=%zu, right=%zu\n", left, right);

    if ((int)currentNode.low < right)
    {
      pushNode.low = currentNode.low;
      pushNode.high = (size_t)right;
      stack_push(&stack, &pushNode);
      //printf("Pushed node: low=%zu, high=%zu\n", pushNode.low, pushNode.high); // Debug print
    }
    if (left < (int)currentNode.high)
    {
      pushNode.low = (size_t)left;
      pushNode.high = currentNode.high;
      stack_push(&stack, &pushNode);
      //printf("Pushed node: low=%zu, high=%zu\n", pushNode.low, pushNode.high); // Debug print
    }
  }

  free_stack(&stack);
}

void printf_binary64(uint64_t value)
{
  int boundaries[] = {3, 4, 4, 4, 4, 4, 10}; // type, card1..5, bid
  int bit_index = 64; // Start from MSB side

  for (int i = 0; i < sizeof(boundaries) / sizeof(boundaries[0]); ++i) {
    bit_index -= boundaries[i];
    for (int b = boundaries[i] - 1; b >= 0; --b) {
      putchar((value >> (bit_index + b)) & 1 ? '1' : '0');
    }
    putchar(' ');
  }

  // Print any remaining bits (from bit_index - 1 to 0)
  for (int b = bit_index - 1; b >= 0; --b) {
    putchar((value >> b) & 1 ? '1' : '0');
  }
}


#define FILE_NAME "input.txt"
int main(void)
{

  FILE *fptr = fopen(FILE_NAME, "r");

  if (!fptr)
  {
    printf("Failed to open file %s", FILE_NAME);
    return 1;
  }

  size_t row_count = 0;
  char ch;
  while (EOF != (ch = fgetc(fptr)))
  {
    if (ch == '\n')
    {
      row_count++;
    }
  }
  row_count += 1;

  // printf("Row count %" PRIu16 " \n", row_count);
  //  I can store each card in 4 bits.
  //  5 cards -> thats 20 bits. so i need a uint32bit.
  //  that leaves 12 bits, which should be enough to store each bid.
  //  there seems to be no number over three digits. (perhaps one thousand. )
  //  so 1023 = 10 bits should be enough.
  //  that leaves 2 bits. that only leaves room for 4 types.
  //  I calculated wrong first -> and thought:
  //  that is enough to also store the type if we would like to.
  //  perhaps we just push them all while reading second time in a list.
  //  then we sort with quicksort?
  //  since one list will be contigous in memory-> this will probably speed up memory access a bit.

  // okay. damn we will just use 64 bits then.

  // lets sort it by order of what how it should be sorted so we push the type first as it is most significant. s
  // type:3, card:4, card:4, card:4, card:4, card:4, bid:10 = 33 bits.
  // 63, 62, 61, 41, 31

  uint64_t *list = malloc(sizeof(uint64_t) * row_count);
  size_t current = 0;

  if (!list)
  {
    printf("Malloc failed for list. ");
    return 1;
  }
  rewind(fptr);

  char cards[CARDS_ON_HAND + 1]; // for nullterm
  uint16_t bid;
  uint8_t c;

  while (fscanf(fptr, "%5s %" SCNu16, cards, &bid) == 2)
  {
    // printf("card %s , num %" PRIu16 "\n", cards, num);

    // check the type -> we have to have a static list of counters for each card
    // there are 13 different cards

    uint8_t card_counters[CARD_TYPES] = {};
    uint8_t highest = 0;
    list[current] = 0;
    for (int i = 0; i < CARDS_ON_HAND; i++)
    {
      c = char_to_card(cards[i]);
      list[current] |= ((uint64_t)(c & 0xF) << (57 - i * 4)); // add the cards. to the first 20 bits.
      card_counters[c] += 1;
      if (card_counters[c] > card_counters[highest])
      {
        highest = c;
      }
    }
    if (card_counters[highest] == 5)
    {
      // five of a kind
      list[current] |= ((uint64_t)FIVE_OF_A_KIND << 61);
    }
    else if (card_counters[highest] == 4)
    {
      list[current] |= ((uint64_t)FOUR_OF_A_KIND << 61);
    }
    else if (card_counters[highest] == 3)
    {
      // check if we have three pair or house
      uint8_t house = 0;
      for (int i = 0; i < CARD_TYPES; i++)
      {
        if (card_counters[i] == 2)
        {
          house = 1;
          break;
        }
      }
      if (house)
      {
        list[current] |= ((uint64_t)FULL_HOUSE << 61);
      }
      else
      { // three of a kind.
        list[current] |= ((uint64_t)THREE_OF_A_KIND << 61);
      }
    }
    else if (card_counters[highest] == 2)
    { //
      uint8_t two_pair = 0;
      for (int i = 0; i < CARD_TYPES; i++)
      {
        if (i == highest)
        {
          continue;
        }

        if (card_counters[i] == 2)
        {
          two_pair = 1;
          break;
        }
      }
      if (two_pair)
      {
        list[current] |= ((uint64_t)TWO_PAIR << 61);
      }
      else
      { // one pair
        list[current] |= ((uint64_t)ONE_PAIR << 61);
      }
    }
    else
    { // high card.
      list[current] |= ((uint64_t)HIGH_CARD << 61);
    }

    // add the bid, and the current++;

    list[current] |= (((uint64_t)bid & 1023) << 31);

    current++;
  }

  // now quicksort.
  quicksort(list, 0, current - 1);

  /*
  for (int i = 0; i < current; i++)
  {

    printf_binary64(list[i]);
    printf("\n");
    switch ((list[i] >> 61) & 0b111)
    {
    case FIVE_OF_A_KIND:
      printf("Five of a kind ");
      break;
    case FOUR_OF_A_KIND:
      printf("Four of a kind ");
      break;
    case FULL_HOUSE:
      printf("Full house ");
      break;
    case THREE_OF_A_KIND:
      printf("Three of a kind ");
      break;
    case TWO_PAIR:
      printf("Two pair ");
      break;
    case ONE_PAIR:
      printf("One pair ");
      break;
    case HIGH_CARD:
      printf("High card ");
      break;
    default:
      printf("Unknown hand ");
      break;
    }

    for (int card = 0; card < CARDS_ON_HAND; card++)
    {
      uint8_t card_val = (list[i] >> (57 - 4 * card)) & 0xF;
      printf("%c", card_to_char(card_val));
    }
    uint16_t bid = (uint16_t)((list[i] >> 31) & 1023);

    printf("  %" PRIu16 " \n", bid);
  }
  */

  // uint16_t highest_bid = 0;
  // for (int i = 0; i < current; i++) {
  //   uint16_t bid = (uint16_t)((list[i] >> 31) & 1023);
  //
  //  if (bid > highest_bid) {
  //    highest_bid = bid;
  //  }
  //}
  // printf("%" PRIu16 "\n", highest_bid);

  uint64_t sum = 0;
  for (uint64_t i = 0; i < current; i++)
  {
    uint64_t bid = ((list[i] >> 31) & 1023);

    sum += bid * (i + 1);
  }

  printf("%d Sum: %" PRIu64 "\n", current, sum);

  free(list);
  fclose(fptr);

  return 0;
}
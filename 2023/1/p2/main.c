#include <stdio.h>
#include <stdint.h>

#include "alpha_number_tree.h"

// maybe make a trie

int main(void)
{
  FILE *fptr = fopen("input.txt", "r");
  char ch = 'a'; // just something that is not accidentally EOF.

  uint8_t first_number = 0;
  uint8_t second_number = 0;
  uint16_t sum = 0;
  uint8_t numbers = 0;

  TrieNode *root = get_alpha_number_trie();

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
      } // ALSO CHECK for letter, then check with trie.
      else if (ch >= 'a' && ch <= 'z')
      {
        // do a trie search. we save the position,
        // perhaps we read threight - then it will attempt to read
        // three but it wont find the last e, and if we start after the word
        // we will reach i, and having missed the first e it wont spell eight.

        uint8_t found = 0;
        long pos = ftell(fptr);
        TrieNode *currentNode = root;
        fseek(fptr, -1, SEEK_CUR);
        while ((ch = fgetc(fptr)) != EOF && ch >= 'a' && ch <= 'z')
        {
          if (!currentNode->children[ch - 'a'])
          {
            // if do not find a word with this prefix. then break;
            // also set currentNode to NULL to tell that it was not found.
            break;
          }
          // else set the currentNode to the child and continue looking
          currentNode = currentNode->children[ch - 'a'];

          if (currentNode->is_terminal)
          {
            // we found the first number.
            // get the value.
            first_number = currentNode->value;
            found = 1;
            break;
          }
        }

        if (found) break;
        // else change back position
        fseek(fptr, pos, SEEK_SET);
      }
    }

    while ((ch = fgetc(fptr)) != EOF)
    {
      if (ch == '\n')
        break;
      // check if number
      if (ch >= '0' && ch <= '9')
      {
        second_number = (uint8_t)(ch - '0');
      }
      else if (ch >= 'a' && ch <= 'z')
      {
        // do a trie search. we save the position,
        // perhaps we read threight - then it will attempt to read
        // three but it wont find the last e, and if we start after the word
        // we will reach i, and having missed the first e it wont spell eight.

        uint8_t found = 0;
        long pos = ftell(fptr);
        TrieNode *currentNode = root;
        fseek(fptr, -1, SEEK_CUR);
        while ((ch = fgetc(fptr)) != EOF && ch >= 'a' && ch <= 'z')
        {
          if (!currentNode->children[ch - 'a'])
          {
            // if do not find a word with this prefix. then break;
            // also set currentNode to NULL to tell that it was not found.
            break;
          }
          // else set the currentNode to the child and continue looking
          currentNode = currentNode->children[ch - 'a'];

          if (currentNode->is_terminal)
          {
            // we found the first number.
            // get the value.
            second_number = currentNode->value;
            found = 1;
            break;
          }
        }

        //if (found) break;
        // else change back position
        fseek(fptr, pos, SEEK_SET);
      }
    }

    uint16_t this_number = first_number * 10 + second_number;

    //printf("number %d is equal to : %d \n", numbers, this_number);
    sum += this_number;
    numbers++;
  }

  printf("sum: %d", sum);
  fclose(fptr);
  return 0;
}
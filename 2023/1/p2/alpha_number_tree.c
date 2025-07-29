#include "alpha_number_tree.h"

/**
 * With c++ 20 i think it should be possible to do these
 * preconfigured numbers in a trie that is fully constexpr.
 * That would be cool. Maybe a future project.
 */

#define TOTAL_NODES 37
TrieNode node_arena[TOTAL_NODES] = {0}; // it should be enough with 37 nodes.
size_t num_nodes_in_arena = 0;

char *alpha_numbers[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
unsigned int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

void insert(TrieNode *root, char *word, unsigned int value)
{
  TrieNode *currentNode = root;
  TrieNode *nextNode = NULL;

  // loop over the word

  for (char *c = word; *c != '\0'; c++)
  {
    // check if the current letter exists in the trie.
    nextNode = currentNode->children[*c - 'a'];
    if (!nextNode)
    {
      // we have our node arena
      num_nodes_in_arena++;

      if (num_nodes_in_arena > TOTAL_NODES)
      {
        // arena overflow. so quit program after printf
        printf("Arena out of memory, increase arena size.");
        exit(1);
      }
      currentNode->children[*c - 'a'] = &node_arena[num_nodes_in_arena];
      currentNode = currentNode->children[*c - 'a'];
    }
    else
    {
      // it exists so move the currentNode and continue.
      currentNode = nextNode;
    }
  }

  currentNode->is_terminal = 1;
  currentNode->value = value;
}

TrieNode *get_alpha_number_trie()
{
  TrieNode *root = node_arena;

  // insert all numbers in the trie
  for (int i = 0; i < ALPHA_NUMBER_COUNT; i++)
  {
    insert(root, alpha_numbers[i], values[i]);
  }
  return root;
}

uint8_t search_trie(TrieNode *root, char *word)
{

  TrieNode *currentNode = root;
  for (char *c = word; *c != '\0'; c++)
  {
    // check the current Node if the letter is there.

    if (currentNode->children[*c - 'a'])
    {
      currentNode = currentNode->children[*c - 'a'];
    }
    else
    {
      currentNode = NULL;
      break;
    }
  }
  if (currentNode)
  {
    return (uint8_t)currentNode->is_terminal;
  }
  return 0;
}

// int main() {
//
//   TrieNode *root = get_alpha_number_trie();
//
//   char *test1 = "zer";
//   char *test2 = "three";
//
//   uint8_t test_result1 = search_trie(root, test1);
//   uint8_t test_result2 = search_trie(root, test2);
//
//   printf("%s in trie: %s, %s in trie: %s \n", test1, test_result1 ? "true" : "false", test2, test_result2 ? "true" : "false");
//
//   return 0;
// }

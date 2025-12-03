#ifndef ALPHA_NUMBER_TRIE_H
#define ALPHA_NUMBER_TRIE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#define ALPHABET_SIZE 26
#define ALPHA_NUMBER_COUNT 10

struct TrieNode {
  struct TrieNode* children[ALPHABET_SIZE];
  unsigned int is_terminal : 1; 
  unsigned int value : 7; //we use the rest of the byte that we probably ned to allocate for the value. 
  //it will only be a proper value for those that are terminal.
  // bitfield, 
  // it will most likely align to 1 byte anyways so no advantage over bool really.
};

typedef struct TrieNode TrieNode;

TrieNode* get_alpha_number_trie();

#endif /*ALPHA_NUMBER_TRIE_H*/
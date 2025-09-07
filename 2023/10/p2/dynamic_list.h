#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct ListNode {
  int x;
  int y;
} ListNode;

typedef struct DynamicList {
  size_t capacity;
  size_t current;
  ListNode* data;
  
} DynamicList;


void list_copy(DynamicList * dst, const DynamicList *src);

void initialize_list(DynamicList *list, size_t initial_capacity);

bool append(DynamicList *list, ListNode *node);
/**
 * Allocate memory for a node and send it and this sets it to the one popped. 
 */
void pop(DynamicList *list, ListNode *popped);

void free_list(DynamicList *list);
#endif /* DYNAMIC_LIST_H */
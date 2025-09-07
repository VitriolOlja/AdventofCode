#include "dynamic_list.h"
#include <string.h>

void initialize_list(DynamicList *list, size_t initial_capacity)
{
  list->data = malloc(initial_capacity * sizeof(ListNode));
  if (!list->data)
  {
    return;
  }
  list->capacity = initial_capacity;
  list->current = 0;
  return;
}

bool append(DynamicList *list, ListNode *node)
{
  if (list->current >= list->capacity)
  {
    // realloc
    void *tmp = realloc(list->data, list->capacity * 2 * sizeof(ListNode));

    if (!tmp)
    {
      // failed malloc
      return false;
    }

    list->capacity *= 2;
    list->data = tmp;
  }

  list->data[list->current++] = *node;
  return true;
}

/**
 * Allocate memory for a node and send it and this sets it to the one popped.
 */
void pop(DynamicList *list, ListNode *popped)
{
  if (list->current == 0)
  {
    popped = NULL;
    return;
  }

  *popped = list->data[list->current--];
}


//https://stackoverflow.com/questions/1401234/differences-between-using-realloc-vs-free-malloc-functions 
//since we dont want to keep data in case of a new malloc. then we do a lossful malloc. 
void list_copy(DynamicList *dst, const DynamicList *src){
 if (src->current > dst->capacity) {
  //realloc 
  free(dst->data);
  void *tmp = malloc(src->capacity * sizeof(ListNode));

  if (!tmp) {
    printf("Failed list copy. ");
    return; //
  }

  dst->data = tmp;
  dst->capacity = src->capacity;
 }
 memcpy(dst->data, src->data, src->current * sizeof(ListNode));
 dst->current = src->current;
 dst->capacity = src->capacity;
}

void free_list(DynamicList *list)
{
  free(list->data);
}
#include "stringline.h"

#include <stdlib.h>

bool str_line_cmp(const StringLine *str1, const StringLine *str2)
{
  if (str1 == NULL || str1->data == NULL || str2 == NULL || str2->data == NULL || str1->current != str2->current) {
    //they are not the same size so cannot be the same string. 
    return false;
  }

  //check that each character is the same. 
  for (uint32_t i = 0; i < str1->current; i++){
    if (str1->data[i] != str2->data[i]) {
      return false;
    }
  }
  return true;
}

void free_str(StringLine *str)
{
  free(str->data);
}

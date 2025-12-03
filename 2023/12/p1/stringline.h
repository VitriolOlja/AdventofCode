#ifndef STRINGLINE_H
#define STRINGLINE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct StringLine {
  char *data;
  uint32_t length;
  uint32_t current; 
} StringLine;


bool str_line_cmp(const StringLine *str1, const StringLine *str2);


void free_str(StringLine *str);

#endif /*STRINGLINE_H*/
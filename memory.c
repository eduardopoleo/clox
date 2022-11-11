#include <stdlib.h>
#include "memory.h"

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }
  // the pointer to the first byte of memory. C will keep some meta
  // to determine the size of the block
  // realloc is like malloc but when data might already be there.
  void* result = realloc(pointer, newSize);
  if (result == NULL) exit(1);
  return result;
}

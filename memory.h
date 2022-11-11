#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

// this quite literally replaces the text GROW_CAPACITY 
// by this 
#define GROW_CAPACITY(capacity) \
  ((capacity) < 8 ? 8 : (capacity) * 2)

// (type*) casts the void pointer in the correct type
// type is a byte in this case which is the minimum addressable memory
#define GROW_ARRAY(type, pointer, oldCount, newCount) \
  (type*)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
  reallocate(pointer, sizeof(type) * oldCount, 0)

// a void pointer points to nowhere
void* reallocate(void* pointer, size_t oldSize, size_t newSize);
#endif
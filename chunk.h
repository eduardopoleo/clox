/*
  Header files are also c code that's shared
  into other files
*/
// I think this ifndef prevents this chunk of code from being inserted
// more than once
#ifndef clox_chunk_h
#define clox_chunk_h
#include "common.h"
#include "memory.h"

// can define enums directly as constants
typedef enum {
  OP_RETURN,
} OpCode; // apprently these are equivalent to uint8_t  
// which makese me think that writeChunk will be used for different types of
// chunnks that are not only OpCodes

// Define properties for the struct
typedef struct {
  int count; // how much of the array is being used
  int capacity; // total capacity
  uint8_t* code; 
  // * indicates that we're saving a pointer? to the beginning of the array
  // This is effectively saving an array. This is how we reference them
} Chunk; // are gonna be the sequence of operations, we still do not know
// what a chunk will but for now we know that will contain a certain number
// of elements in an array.

// init take a pointer to where chunk is (not the actual object)
// function declarations are put in the .h files
void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte);

#endif
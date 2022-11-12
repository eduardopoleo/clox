# include <stdlib.h>
# include "chunk.h"
# include "memory.h"

// function definitions are locate in the .c files
void initChunk(Chunk* chunk) {
  // -> dereference chunk and calls . to it
  // verify if this is the same as *chunk.count = 0
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  // a reference to where chunk constants are stored
  initValueArray(&chunk->constants);
}

// I think it's called byte cuz either 8 bit is a byte
// or cuz it points to the place where a byte is stored.
void writeChunk(Chunk* chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count+1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
  }
  // ok a byte is the smallest addresable unit of memory
  // it's a least a bit but apparently it can be bigger.
  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  // writes the byte into the first non used memory space
  // the count is effectively 
  chunk->count++; // adds 1 to the count 
}

void freeChunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(uint8_t, chunk->lines, chunk->capacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk); // passes the pointer directly
}

int addConstant(Chunk* chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  // this returns a byte a apprently or a uint8_t
  return chunk->constants.count - 1;
}
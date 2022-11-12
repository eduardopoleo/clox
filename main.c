#include "common.h"
#include <stdio.h>
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
  Chunk chunk;
  // &references the object so that we pass the pointer
  initChunk(&chunk);
  // Add the value
  int constant = addConstant(&chunk, 1.2);
  // The code stream for this chunk is gonna have
  // two entries (for now)
  // Operation and the index where the constant is
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant, 123);

  writeChunk(&chunk, OP_RETURN, 123);
  dissassembleChunk(&chunk, "Test chunk");
  freeChunk(&chunk);
  return 0;
}
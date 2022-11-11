#include "common.h"
#include <stdio.h>
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
  Chunk chunk;
  // &references the object so that we pass the pointer
  initChunk(&chunk);
  writeChunk(&chunk, OP_RETURN);
  dissassembleChunk(&chunk, "Test chunk");
  freeChunk(&chunk);
  return 0;
}
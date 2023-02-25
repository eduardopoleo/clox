//
//  compiler.h
//  clox
//
//  Created by Eduardo Poleo on 2023-02-20.
//

#ifndef compiler_h
#define compiler_h

#include <stdbool.h>
#include "vm.h"


bool compile(const char * source, Chunk* chunk);

#endif /* compiler_h */

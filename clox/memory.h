//
//  memory.h
//  clox
//
//  Created by Eduardo Poleo on 2023-02-17.
//

#ifndef memory_h
#define memory_h

// Not sure why this isn't just a method
#define GROW_CAPACITY(capacity) \
    capacity < 8 ? 8 : capacity * 2

/*
    pointer refers to the Array pointer
    This is an interface that wraps the reallocate method
    it allows us to use the method with any type
*/

#define GROW_ARRAY(type, pointer, oldCapacity, newCapacity) \
    (type*)reallocate(pointer, sizeof(type) * oldCapacity, sizeof(type) * newCapacity)

#define FREE_ARRAY(type, pointer, oldCapacity) \
    reallocate(pointer, sizeof(type) * oldCapacity, 0)

void *reallocate(void *pointer, size_t oldSize, size_t newSize);

#endif /* memory_h */

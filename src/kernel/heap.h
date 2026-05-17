#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>

void heap_init(uint8_t* start, size_t size);

void* kmalloc(size_t size);
void* kmalloc_aligned(size_t size, size_t align);

#endif
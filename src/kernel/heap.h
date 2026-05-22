#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdint.h>

void heap_init(uint8_t *start, size_t size);
void heap_reset();

void *kmalloc(size_t size);
void *kmalloc_aligned(size_t size, size_t align);

#endif

#include "heap.h"

static uint8_t* heap_start = 0;
static uint8_t* heap_end   = 0;
static uint8_t* heap_curr  = 0;

static inline size_t align_forward(size_t ptr, size_t align)
{
    size_t p = ptr;
    size_t a = align;

    size_t mod = p & (a - 1);

    if (mod != 0)
        p += (a - mod);

    return p;
}

void heap_init(uint8_t* start, size_t size)
{
    heap_start = start;
    heap_end   = start + size;
    heap_curr  = start;
}

/* malloc simples */
void* kmalloc(size_t size)
{
    return kmalloc_aligned(size, 8);
}

/* malloc com alinhamento */
void* kmalloc_aligned(size_t size, size_t align)
{
    if (heap_curr == 0)
        return 0;

    size_t curr_addr = (size_t)heap_curr;
    size_t aligned   = align_forward(curr_addr, align);

    uint8_t* ptr = (uint8_t*)aligned;

    if (ptr + size > heap_end)
        return 0; // sem memória

    heap_curr = ptr + size;

    return ptr;
}
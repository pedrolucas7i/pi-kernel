#ifndef MMIO_H
#define MMIO_H

#include <stdint.h>

#define MMIO_BASE 0x20000000  // Pi 1 (change for Pi2/3/4)

static inline void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t*)reg = data;
}

static inline uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t*)reg;
}

#endif
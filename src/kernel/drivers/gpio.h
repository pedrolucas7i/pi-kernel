#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "mmio.h"

#define GPIO_BASE (MMIO_BASE + 0x200000)

#define GPFSEL0 (GPIO_BASE + 0x00)
#define GPFSEL1 (GPIO_BASE + 0x04)
#define GPFSEL2 (GPIO_BASE + 0x08)

#define GPSET0  (GPIO_BASE + 0x1C)
#define GPCLR0  (GPIO_BASE + 0x28)
#define GPLEV0  (GPIO_BASE + 0x34)

#define GPREN0  (GPIO_BASE + 0x4C)
#define GPFEN0  (GPIO_BASE + 0x58)

#define GPEDS0  (GPIO_BASE + 0x40)

#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

void gpio_set_function(uint32_t pin, uint32_t function);
void gpio_set(uint32_t pin);
void gpio_clear(uint32_t pin);
uint32_t gpio_read(uint32_t pin);

void gpio_enable_falling_edge(uint32_t pin);
void gpio_clear_event(uint32_t pin);
void gpio_set_input(uint32_t pin);

#endif
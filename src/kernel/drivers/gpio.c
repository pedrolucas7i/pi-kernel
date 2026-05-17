#include "gpio.h"

void gpio_set_function(uint32_t pin, uint32_t function)
{
    uint32_t reg = GPFSEL0 + ((pin / 10) * 4);
    uint32_t shift = (pin % 10) * 3;

    uint32_t value = mmio_read(reg);

    value &= ~(7 << shift);
    value |= (function << shift);

    mmio_write(reg, value);
}

void gpio_set(uint32_t pin)
{
    mmio_write(GPSET0, (1 << pin));
}

void gpio_clear(uint32_t pin)
{
    mmio_write(GPCLR0, (1 << pin));
}

uint32_t gpio_read(uint32_t pin)
{
    return (mmio_read(GPLEV0) & (1 << pin)) ? 1 : 0;
}

void gpio_enable_falling_edge(uint32_t pin)
{
    uint32_t val = mmio_read(GPFEN0);
    val |= (1 << pin);
    mmio_write(GPFEN0, val);
}

void gpio_clear_event(uint32_t pin)
{
    mmio_write(GPEDS0, (1 << pin));
}

void gpio_set_input(uint32_t pin)
{
    uint32_t reg = GPFSEL0 + ((pin / 10) * 4);
    uint32_t shift = (pin % 10) * 3;

    uint32_t value = mmio_read(reg);
    value &= ~(7 << shift);

    mmio_write(reg, value);
}
#include "irq.h"
#include "drivers/mmio.h"

void irq_enable()
{
    asm volatile("cpsie i");
}

void irq_disable()
{
    asm volatile("cpsid i");
}

void irq_init()
{
    mmio_write(IRQ_ENABLE2, (1 << (IRQ_GPIO0 - 32)));
}
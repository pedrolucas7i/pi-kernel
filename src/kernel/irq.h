#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>

#define IRQ_BASE (MMIO_BASE + 0xB200)

#define IRQ_ENABLE2  (IRQ_BASE + 0x214)
#define IRQ_DISABLE2 (IRQ_BASE + 0x220)

#define IRQ_PENDING2 (IRQ_BASE + 0x208)

#define IRQ_GPIO0 49

void irq_enable();
void irq_disable();
void irq_init();

#endif
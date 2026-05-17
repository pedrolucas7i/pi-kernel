#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/* Initialize the timer */
void timer_init(void);

/* Returns ticks (microsseconds from boot) */
uint32_t get_ticks(void);

/* Delay */
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#endif  
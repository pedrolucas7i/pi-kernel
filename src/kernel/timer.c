#include "timer.h"

/* =========================================================
   BCM2835 SYSTEM TIMER
   ========================================================= */

#define TIMER_BASE  0x20003000UL

#define TIMER_CS    ((volatile uint32_t*)(TIMER_BASE + 0x00))
#define TIMER_CLO   ((volatile uint32_t*)(TIMER_BASE + 0x04))
#define TIMER_CHI   ((volatile uint32_t*)(TIMER_BASE + 0x08))
#define TIMER_C0    ((volatile uint32_t*)(TIMER_BASE + 0x0C))
#define TIMER_C1    ((volatile uint32_t*)(TIMER_BASE + 0x10))
#define TIMER_C2    ((volatile uint32_t*)(TIMER_BASE + 0x14))
#define TIMER_C3    ((volatile uint32_t*)(TIMER_BASE + 0x18))

/* =========================================================
   INIT
   ========================================================= */

void timer_init(void)
{
    /* limpa flags do timer */
    *TIMER_CS = 0;
}

/* =========================================================
   GET TICKS (microseconds)
   ========================================================= */

uint32_t get_ticks(void)
{
    return *TIMER_CLO;
}

/* =========================================================
   DELAY MICROSECONDS
   ========================================================= */

void delay_us(uint32_t us)
{
    uint32_t start = get_ticks();

    while ((get_ticks() - start) < us)
    {
        __asm__ volatile("nop");
    }
}

/* =========================================================
   DELAY MILLISECONDS
   ========================================================= */

void delay_ms(uint32_t ms)
{
    while (ms--)
        delay_us(1000);
}
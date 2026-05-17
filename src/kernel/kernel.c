#include "drivers/gpu.h"
#include "drivers/uart.h"

static inline void delay(int32_t count)
{
    while(count--)
        asm volatile("nop");
}

void kernel_main(void)
{
    uart_init();
    framebuffer_init(1024, 768, 32);
    int t=0;
    while (1)
    {
        draw_wave_background(t);
        uart_putc((char)t);
        t++;
    }
}
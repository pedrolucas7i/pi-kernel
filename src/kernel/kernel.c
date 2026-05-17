#include "drivers/gpu.h"
#include "drivers/uart.h"
#include "timer.h"

void kernel_main(void)
{
    uart_init();
    framebuffer_init(1920, 1080, 32);
    int t=0;
    while (1)
    {
        draw_wave_background(t);
        uart_putc(t);
        t++;
    }
}
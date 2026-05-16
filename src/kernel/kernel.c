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

    uart_putc('A');
    uart_putc('\n');

    char msg[] = {'H','I','\n','\0'};
    uart_puts(msg);

    uart_putc('B');
    uart_putc('\n');

    framebuffer_init(1024, 768, 32);

    clear_screen(0x00101010);

    uart_puts((char*)"GPU");

    draw_rect(50, 50, 200, 120, 0x00FF0000);

    while (1)
    {
        delay(1000000);
    }
}
#include "drivers/gpu.h"
#include "drivers/uart.h"
#include "drivers/gpio.h"
#include "timer.h"

#define LED_PIN 16

void kernel_main(void)
{
    uart_init();

    while(!framebuffer_init(1920, 1080, 32));
    
    char msg[] = {'F','B','\n','\0'};
    uart_puts(msg);

    clear_screen(0x000000);
    gpio_set_function(LED_PIN, GPIO_OUTPUT);

    int t = 0;

    while (1)
    {
        if (t % 2)
            gpio_set(LED_PIN);
        else
            gpio_clear(LED_PIN);
        draw_wave_background(t);
        uart_putc('0' + (t % 10));

        uart_putc('\n');
        delay_ms(500);

        t++;
    }
}
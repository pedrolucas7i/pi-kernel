#include "drivers/gpu.h"
#include "drivers/uart.h"
#include "drivers/gpio.h"
#include "timer.h"

#define LED_PIN 16

void kernel_main(void)
{
    uart_init();

    if (!framebuffer_init(1920, 1080, 32))
    {
        uart_putc('F');
        uart_putc('B');
        uart_putc('\n');
        uart_putc('F');
        uart_putc('A');
        uart_putc('I');
        uart_putc('L');
        uart_putc('\n');
    }
    else
    {
        uart_putc('F');
        uart_putc('B');
        uart_putc('\n');
        uart_putc('O');
        uart_putc('K');
    }

    clear_screen(0x000000);

    gpio_set_function(LED_PIN, GPIO_OUTPUT);

    int t = 0;
    int led_state = 0;

    uint32_t milis;
    uint32_t last_milis = get_ticks_ms();

    while (1)
    {
        milis = get_ticks_ms();

        if (milis - last_milis > 500)
        {

            uart_putc('\n');

            led_state = !led_state;

            if (led_state)
            {
                gpio_set(LED_PIN);
                uart_putc('1');
            }
            else
            {
                gpio_clear(LED_PIN);
                uart_putc('0');
            }

            last_milis = milis;
        }

        draw_wave_background(t);

        t++;
    }
}
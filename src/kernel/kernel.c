// src/kernel/kernel.c

#include <stdint.h>
#include "drivers/gpu.h"

static inline void delay(int32_t count)
{
    while(count--)
    {
        asm volatile("nop");
    }
}

void kernel_main(void)
{
    if (!framebuffer_init(1024, 768, 32))
    {
        while (1);
    }

    clear_screen(0x00101010);

    draw_rect(50, 50, 250, 150, 0x00FF0000);

    draw_rect(350, 100, 120, 300, 0x0000FF00);

    draw_circle(700, 250, 120, 0x000000FF);

    draw_triangle(
        700,
        500,
        900,
        650,
        550,
        650,
        0x00FFFF00
    );

    draw_line(
        0,
        0,
        1023,
        767,
        0x00FFFFFF
    );

    draw_line(
        1023,
        0,
        0,
        767,
        0x00FFFFFF
    );

    while (1)
    {
        delay(100000);
    }
}
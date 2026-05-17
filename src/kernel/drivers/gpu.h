#ifndef GPU_H
#define GPU_H

#include <stdint.h>

int framebuffer_init(uint32_t width,
                     uint32_t height,
                     uint32_t depth);

void clear_screen(uint32_t color);

void put_pixel(int x, int y, uint32_t color);
void put_pixel_alpha(int x, int y, uint32_t color, uint8_t alpha);

void draw_rect(int x,
               int y,
               int w,
               int h,
               uint32_t color,
               int filled);

void draw_line(int x0,
               int y0,
               int x1,
               int y1,
               uint32_t color);

void draw_triangle(int x1,
                   int y1,
                   int x2,
                   int y2,
                   int x3,
                   int y3,
                   uint32_t color,
                   int filled);

void draw_circle(int xc,
                 int yc,
                 int radius,
                 uint32_t color,
                 int filled);

void draw_noise(int density);

void draw_wave_background(int t);

void fill_gradient(uint32_t c1,
                   uint32_t c2,
                   int vertical);

void draw_circle_gradient(int xc,
                          int yc,
                          int radius,
                          uint32_t c1,
                          uint32_t c2);

uint32_t blend(uint32_t src,
               uint32_t dst,
               uint8_t alpha);

#endif
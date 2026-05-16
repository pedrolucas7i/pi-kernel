// src/kernel/gpu.h

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

int framebuffer_init(uint32_t width,
                     uint32_t height,
                     uint32_t depth);

void clear_screen(uint32_t color);

void draw_rect(int x,
               int y,
               int w,
               int h,
               uint32_t color,
               int);

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

#endif
// src/kernel/gpu.c

#include "gpu.h"

#define MMIO_BASE       0x20000000UL

#define MAILBOX_BASE    (MMIO_BASE + 0xB880)

#define MAILBOX_READ    ((volatile uint32_t*)(MAILBOX_BASE + 0x00))
#define MAILBOX_STATUS  ((volatile uint32_t*)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE   ((volatile uint32_t*)(MAILBOX_BASE + 0x20))

#define MAILBOX_EMPTY   0x40000000
#define MAILBOX_FULL    0x80000000

#define MAILBOX_PROP    8

static volatile uint32_t __attribute__((aligned(16))) mbox[36];

static uint8_t* framebuffer = 0;

static uint32_t screen_width  = 0;
static uint32_t screen_height = 0;
static uint32_t screen_pitch  = 0;
static uint32_t screen_depth  = 0;

static int mailbox_call(uint8_t channel)
{
    uint32_t addr =
    (((uint32_t)&mbox & ~0xF) |
     (channel & 0xF));

    while (*MAILBOX_STATUS & MAILBOX_FULL);

    *MAILBOX_WRITE = addr;

    while (1)
    {
        while (*MAILBOX_STATUS & MAILBOX_EMPTY);

        uint32_t response = *MAILBOX_READ;

        if (response == addr)
        {
            return mbox[1] == 0x80000000;
        }
    }

    return 0;
}

int framebuffer_init(uint32_t width,
                     uint32_t height,
                     uint32_t depth)
{
    mbox[0] = 35 * 4;
    mbox[1] = 0;

    mbox[2] = 0x48003;
    mbox[3] = 8;
    mbox[4] = 8;
    mbox[5] = width;
    mbox[6] = height;

    mbox[7] = 0x48004;
    mbox[8] = 8;
    mbox[9] = 8;
    mbox[10] = width;
    mbox[11] = height;

    mbox[12] = 0x48009;
    mbox[13] = 8;
    mbox[14] = 8;
    mbox[15] = 0;
    mbox[16] = 0;

    mbox[17] = 0x48005;
    mbox[18] = 4;
    mbox[19] = 4;
    mbox[20] = depth;

    mbox[21] = 0x48006;
    mbox[22] = 4;
    mbox[23] = 4;
    mbox[24] = 1;

    mbox[25] = 0x40001;
    mbox[26] = 8;
    mbox[27] = 8;
    mbox[28] = 16;
    mbox[29] = 0;

    mbox[30] = 0x40008;
    mbox[31] = 4;
    mbox[32] = 4;
    mbox[33] = 0;

    mbox[34] = 0;

    if (!mailbox_call(MAILBOX_PROP))
    {
        return 0;
    }

    framebuffer =
        (uint8_t*)((mbox[28]) & 0x3FFFFFFF);

    screen_width  = width;
    screen_height = height;
    screen_pitch  = mbox[33];
    screen_depth  = depth;

    if (framebuffer == 0)
    {
        return 0;
    }

    return 1;
}

void put_pixel(int x,
                             int y,
                             uint32_t color)
{
    if (x < 0 || y < 0)
        return;

    if (x >= (int)screen_width)
        return;

    if (y >= (int)screen_height)
        return;

    uint32_t offset =
        (y * screen_pitch) +
        (x * (screen_depth >> 3));

    *((volatile uint32_t*)(framebuffer + offset)) = color;
}

void put_pixel_alpha(int x, int y, uint32_t color, uint8_t alpha)
{
    if (x < 0 || y < 0 || x >= (int)screen_width || y >= (int)screen_height)
        return;

    uint32_t offset = (y * screen_pitch) + (x * (screen_depth >> 3));

    uint32_t* px = (uint32_t*)(framebuffer + offset);

    *px = blend(color, *px, alpha);
}

void clear_screen(uint32_t color)
{
    for (uint32_t y = 0; y < screen_height; y++)
    {
        for (uint32_t x = 0; x < screen_width; x++)
        {
            put_pixel(x, y, color);
        }
    }
}

/* =========================
   RECTANGLE
   ========================= */

void draw_rect(int x,
               int y,
               int w,
               int h,
               uint32_t color,
               int filled)
{
    if (filled)
    {
        for (int yy = 0; yy < h; yy++)
        {
            for (int xx = 0; xx < w; xx++)
            {
                put_pixel(x + xx,
                          y + yy,
                          color);
            }
        }
    }
    else
    {
        draw_line(x, y, x + w - 1, y, color);
        draw_line(x, y, x, y + h - 1, color);

        draw_line(x + w - 1,
                  y,
                  x + w - 1,
                  y + h - 1,
                  color);

        draw_line(x,
                  y + h - 1,
                  x + w - 1,
                  y + h - 1,
                  color);
    }
}

/* =========================
   LINE
   ========================= */

void draw_line(int x0,
               int y0,
               int x1,
               int y1,
               uint32_t color)
{
    int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int sx = (x0 < x1) ? 1 : -1;

    int dy = (y1 > y0) ? -(y1 - y0) : -(y0 - y1);
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx + dy;

    while (1)
    {
        put_pixel(x0, y0, color);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = err << 1;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

/* =========================
   TRIANGLE
   ========================= */

static void swap_int(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void draw_triangle(int x1,
                   int y1,
                   int x2,
                   int y2,
                   int x3,
                   int y3,
                   uint32_t color,
                   int filled)
{
    if (!filled)
    {
        draw_line(x1, y1, x2, y2, color);
        draw_line(x2, y2, x3, y3, color);
        draw_line(x3, y3, x1, y1, color);

        return;
    }

    if (y1 > y2)
    {
        swap_int(&y1, &y2);
        swap_int(&x1, &x2);
    }

    if (y2 > y3)
    {
        swap_int(&y2, &y3);
        swap_int(&x2, &x3);
    }

    if (y1 > y2)
    {
        swap_int(&y1, &y2);
        swap_int(&x1, &x2);
    }

    for (int y = y1; y <= y3; y++)
    {
        int xa;
        int xb;

        if (y < y2)
        {
            xa = x1 + (x2 - x1) * (y - y1) / (y2 - y1 + 1);
            xb = x1 + (x3 - x1) * (y - y1) / (y3 - y1 + 1);
        }
        else
        {
            xa = x2 + (x3 - x2) * (y - y2) / (y3 - y2 + 1);
            xb = x1 + (x3 - x1) * (y - y1) / (y3 - y1 + 1);
        }

        if (xa > xb)
            swap_int(&xa, &xb);

        for (int x = xa; x <= xb; x++)
        {
            put_pixel(x, y, color);
        }
    }
}

/* =========================
   CIRCLE
   ========================= */

void draw_circle(int xc,
                 int yc,
                 int radius,
                 uint32_t color,
                 int filled)
{
    int x = radius;
    int y = 0;

    int decision = 1 - x;

    while (y <= x)
    {
        if (filled)
        {
            for (int xx = -x; xx <= x; xx++)
            {
                put_pixel(xc + xx, yc + y, color);
                put_pixel(xc + xx, yc - y, color);
            }

            for (int xx = -y; xx <= y; xx++)
            {
                put_pixel(xc + xx, yc + x, color);
                put_pixel(xc + xx, yc - x, color);
            }
        }
        else
        {
            put_pixel(xc + x, yc + y, color);
            put_pixel(xc - x, yc + y, color);

            put_pixel(xc + x, yc - y, color);
            put_pixel(xc - x, yc - y, color);

            put_pixel(xc + y, yc + x, color);
            put_pixel(xc - y, yc + x, color);

            put_pixel(xc + y, yc - x, color);
            put_pixel(xc - y, yc - x, color);
        }

        y++;

        if (decision <= 0)
        {
            decision += (2 * y) + 1;
        }
        else
        {
            x--;
            decision += (2 * (y - x)) + 1;
        }
    }
}

uint32_t blend(uint32_t src, uint32_t dst, uint8_t alpha)
{
    uint8_t sr = (src >> 16) & 0xFF;
    uint8_t sg = (src >> 8) & 0xFF;
    uint8_t sb = src & 0xFF;

    uint8_t dr = (dst >> 16) & 0xFF;
    uint8_t dg = (dst >> 8) & 0xFF;
    uint8_t db = dst & 0xFF;

    uint8_t r = (sr * alpha + dr * (255 - alpha)) / 255;
    uint8_t g = (sg * alpha + dg * (255 - alpha)) / 255;
    uint8_t b = (sb * alpha + db * (255 - alpha)) / 255;

    return (r << 16) | (g << 8) | b;
}

void fill_gradient(uint32_t c1, uint32_t c2, int vertical)
{
    for (int y = 0; y < (int)screen_height; y++)
    {
        for (int x = 0; x < (int)screen_width; x++)
        {
            uint8_t t;

            if (vertical)
                t = (y * 255) / screen_height;
            else
                t = (x * 255) / screen_width;

            uint8_t r1 = (c1 >> 16) & 0xFF;
            uint8_t g1 = (c1 >> 8) & 0xFF;
            uint8_t b1 = c1 & 0xFF;

            uint8_t r2 = (c2 >> 16) & 0xFF;
            uint8_t g2 = (c2 >> 8) & 0xFF;
            uint8_t b2 = c2 & 0xFF;

            uint8_t r = r1 + ((r2 - r1) * t) / 255;
            uint8_t g = g1 + ((g2 - g1) * t) / 255;
            uint8_t b = b1 + ((b2 - b1) * t) / 255;

            put_pixel(x, y, (r << 16) | (g << 8) | b);
        }
    }
}

void draw_circle_gradient(int xc, int yc, int radius, uint32_t c1, uint32_t c2)
{
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            if (x * x + y * y <= radius * radius)
            {
                int d = (x * x + y * y);
                int max = radius * radius;

                uint8_t t = (d * 255) / max;

                uint8_t r = ((c1 >> 16) & 0xFF) + (((c2 >> 16) & 0xFF - (c1 >> 16) & 0xFF) * t) / 255;
                uint8_t g = ((c1 >> 8) & 0xFF) + (((c2 >> 8) & 0xFF - (c1 >> 8) & 0xFF) * t) / 255;
                uint8_t b = (c1 & 0xFF) + ((c2 & 0xFF - c1 & 0xFF) * t) / 255;

                put_pixel(xc + x, yc + y, (r << 16) | (g << 8) | b);
            }
        }
    }
}

static uint32_t seed2 = 987654;

static inline uint32_t fast_rand()
{
    seed2 = seed2 * 1103515245 + 12345;
    return seed2;
}

void draw_noise(int density)
{
    for (int i = 0; i < density; i++)
    {
        int x = fast_rand() % screen_width;
        int y = fast_rand() % screen_height;

        uint32_t v = fast_rand() & 0xFF;

        uint32_t color = (v << 16) | (v << 8) | v;

        put_pixel(x, y, color);
    }
}

void draw_wave_background(int t)
{
    for (int y = 0; y < (int)screen_height; y++)
    {
        for (int x = 0; x < (int)screen_width; x++)
        {
            uint8_t wave = (uint8_t)(
                (x + t + (y >> 1)) ^ (y + (t >> 1))
            );

            uint32_t color =
                (wave << 16) |
                ((wave / 2) << 8) |
                (255 - wave);

            put_pixel(x, y, color);
        }
    }
}
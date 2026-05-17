#include "memory.h"

/* =========================================================
   MEMSET (critical for kernel, framebuffer, etc)
   ========================================================= */
void *memset(void *dst, int val, size_t n)
{
    uint8_t *p = (uint8_t*)dst;
    uint8_t v = (uint8_t)val;

    for (size_t i = 0; i < n; i++)
        p[i] = v;

    return dst;
}

/* =========================================================
   MEMCPY
   ========================================================= */
void *memcpy(void *dst, const void *src, size_t n)
{
    uint8_t *d = (uint8_t*)dst;
    const uint8_t *s = (const uint8_t*)src;

    for (size_t i = 0; i < n; i++)
        d[i] = s[i];

    return dst;
}

/* =========================================================
   MEMMOVE (safe overlap)
   ========================================================= */
void *memmove(void *dst, const void *src, size_t n)
{
    uint8_t *d = (uint8_t*)dst;
    const uint8_t *s = (const uint8_t*)src;

    if (d < s)
    {
        for (size_t i = 0; i < n; i++)
            d[i] = s[i];
    }
    else
    {
        for (size_t i = n; i > 0; i--)
            d[i - 1] = s[i - 1];
    }

    return dst;
}
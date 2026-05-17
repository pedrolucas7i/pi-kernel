#ifndef KERNEL_MATH_H
#define KERNEL_MATH_H

static inline int abs_int(int x)
{
    return (x < 0) ? -x : x;
}

static inline int min_int(int a, int b)
{
    return (a < b) ? a : b;
}

static inline int max_int(int a, int b)
{
    return (a > b) ? a : b;
}

#endif
#include "uart.h"
#include "../timer.h"
#include <stdint.h>

/*
 * Raspberry Pi 1 PL011 UART0
 */
#define UART0_BASE 0x20201000

#define UART_DR   (*(volatile uint32_t*)(UART0_BASE + 0x00))
#define UART_FR   (*(volatile uint32_t*)(UART0_BASE + 0x18))
#define UART_IBRD (*(volatile uint32_t*)(UART0_BASE + 0x24))
#define UART_FBRD (*(volatile uint32_t*)(UART0_BASE + 0x28))
#define UART_LCRH (*(volatile uint32_t*)(UART0_BASE + 0x2C))
#define UART_CR   (*(volatile uint32_t*)(UART0_BASE + 0x30))
#define UART_ICR  (*(volatile uint32_t*)(UART0_BASE + 0x44))

/* GPIO (Raspberry Pi 1) */
#define GPFSEL1   (*(volatile uint32_t*)(0x20200004))
#define GPPUD     (*(volatile uint32_t*)(0x20200094))
#define GPPUDCLK0 (*(volatile uint32_t*)(0x20200098))

#define FR_TXFF (1 << 5)
#define FR_RXFE (1 << 4)

/* =========================
   GPIO SETUP FOR UART
   ========================= */
static void uart_gpio_init(void)
{
    /* GPIO14 & GPIO15 to ALT0 (UART0) */
    GPFSEL1 &= ~((7 << 12) | (7 << 15));
    GPFSEL1 |=  ((4 << 12) | (4 << 15));

    /* Disable pull-up/down */
    GPPUD = 0;
    delay(150);
    GPPUDCLK0 = (1 << 14) | (1 << 15);
    delay(150);
    GPPUDCLK0 = 0;
}

/* =========================
   UART INIT
   ========================= */
void uart_init(void)
{
    uart_gpio_init();

    UART_CR = 0;           // disable UART
    UART_ICR = 0x7FF;      // clear interrupts

    /* Baud rate (safe for QEMU / Pi1 default clock) */
    UART_IBRD = 1;
    UART_FBRD = 40;

    /* 8N1 + FIFO enable */
    UART_LCRH = (1 << 4) | (3 << 5);

    /* Enable UART, TX, RX */
    UART_CR = (1 << 0) | (1 << 8) | (1 << 9);
}

/* =========================
   PUT CHAR
   ========================= */
void uart_putc(char c)
{
    while (UART_FR & FR_TXFF);

    if (c == '\n')
        UART_DR = '\r';

    while (UART_FR & FR_TXFF);

    UART_DR = (uint32_t)c;
}

/* =========================
   RX
   ========================= */
int uart_rx_ready(void)
{
    return !(UART_FR & FR_RXFE);
}

char uart_getc(void)
{
    while (UART_FR & FR_RXFE);
    return (char)UART_DR;
}
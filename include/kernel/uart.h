#ifndef UART_H
#define UART_H
#include "stdint.h"

void uart_initialize(void);
char uart_getchar(void);
bool uart_is_receive_buffer_empty(void);
void uart_putchar(char a);
void uart_puts(const char* str);

#endif /* ifndef UART_H */

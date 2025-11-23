#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

void console_init(void);
void console_put_char(char character);
void console_write(const char *string);

#endif
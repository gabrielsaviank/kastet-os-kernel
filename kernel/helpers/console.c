// kernel/console.c
#include "console.h"

#include <stdint.h>

#define VGA_MEM    ((uint16_t*)0xB8000)
#define VGA_COLOUR 0x0F
#define VGA_WIDTH  80
#define VGA_HEIGHT 25

static int cursor_x;
static int cursor_y;

static inline uint16_t vga_entry(char c) {
    return ((uint16_t)VGA_COLOUR << 8) | (uint8_t)c;
}

static void console_clear_screen(void) {
    volatile uint16_t* vga = VGA_MEM;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
        vga[i] = vga_entry(' ');
    }
}

static void console_newline(void) {
    volatile uint16_t* vga = VGA_MEM;

    cursor_x = 0;
    cursor_y++;

    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = VGA_HEIGHT - 1;

        for (int y = 1; y < VGA_HEIGHT; ++y) {
            for (int x = 0; x < VGA_WIDTH; ++x) {
                vga[(y - 1) * VGA_WIDTH + x] = vga[y * VGA_WIDTH + x];
            }
        }

        for (int x = 0; x < VGA_WIDTH; ++x) {
            vga[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ');
        }
    }
}

void console_put_char(char character) {
    volatile uint16_t* vga = VGA_MEM;

    if (character == '\n') {
        console_newline();
        return;
    }

    vga[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(character);
    cursor_x++;

    if (cursor_x >= VGA_WIDTH) {
        console_newline();
    }
}

void console_write(const char* string) {
    while (*string) {
        console_put_char(*string++);
    }
}

void console_init(void) {
    cursor_x = 0;
    cursor_y = 0;
    console_clear_screen();
}

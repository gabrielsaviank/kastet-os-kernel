#include <stdint.h>

#define VGA_MEM    ((uint16_t*)0xB8000)
#define VGA_COLOUR 0x0F
#define VGA_WIDTH  80
#define VGA_HEIGHT 25

static int cursor_x;
static int cursor_y;

void kernel_main(void) __attribute__((section(".text.kernel_main")));

static inline uint16_t vga_entry(char c) {
    return ((uint16_t)VGA_COLOUR << 8) | (uint8_t)c;
}

static void console_clear(void) {
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

static void console_put_char(char c) {
    volatile uint16_t* vga = VGA_MEM;

    if (c == '\n') {
        console_newline();
        return;
    }

    vga[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c);
    cursor_x++;

    if (cursor_x >= VGA_WIDTH) {
        console_newline();
    }
}

static void kprint(const char* s) {
    while (*s) {
        console_put_char(*s++);
    }
}

void kernel_main(void) {
    cursor_x = 0;
    cursor_y = 0;

    console_clear();
    kprint("Hallo, this is a 32-bit kernel.\n");
    kprint("We now have a tiny text console. :)\n");

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}

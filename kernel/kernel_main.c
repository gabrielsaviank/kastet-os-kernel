#include <stdint.h>
#include "helpers/console.h"

#define VGA_MEM    ((uint16_t*)0xB8000)
#define VGA_COLOUR 0x0F
#define VGA_WIDTH  80
#define VGA_HEIGHT 25


void kernel_main(void) __attribute__((section(".text.kernel_main")));

void kernel_main(void) {
    console_init();

    console_write("Hallo, this is a 32-bit kernel.\n");
    console_write("We now have a tiny text console. :)\n");

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}

#include <stdint.h>
#include "helpers/console.h"
#include "interrupts.h"

#define VGA_MEM    ((uint16_t*)0xB8000)
#define VGA_COLOUR 0x0F
#define VGA_WIDTH  80
#define VGA_HEIGHT 25


void kernel_main(void) __attribute__((section(".text.kernel_main")));

void kernel_main(void) {
    console_init();

    console_write("Beginnend 32 bit kernel.\n");

    interrupt_descriptor_table_init();

    // TEEEESST
    console_write("IDT initialised. Triggering int 0...\n");

    __asm__ __volatile__("int $0");

    console_write("If you see this, int 0 failed.\n");

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}

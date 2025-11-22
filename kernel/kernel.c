#include <stdint.h>

#define VGA_TEXT_BUFFER ((uint16_t* )0xB8000)
#define VGA_COLOUR 0x0F

void kernel_main(void) {
    const char* msg = "Hallo this is 32 bit kernel, OS kernel has been called";

    volatile  uint16_t* vga = VGA_TEXT_BUFFER;

    for (int i = 0; i < 80 *  25; i++) {
        vga[i] = ((uint16_t)VGA_COLOUR << 8) | ' ';
    }

    for(int i = 0; msg[i] != '\0'; i++){
        vga[i] = ((uint16_t)VGA_COLOUR << 8) | (uint8_t)msg[i];
    }

    for(;;) {
        // same shite as the assembly code to halt
        __asm__ __volatile__("hlt");
    }
}
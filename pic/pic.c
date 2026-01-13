#include <stdint.h>
#include "helpers/io.h"
#include "pic.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW_8086 0x01

void picRemap(int offsetMaster, int offsetSlave) {
    uint8_t a1, a2

    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW_ICW4);

    outb(PIC1_DATA,(uint8_t) offsetMaster);
    outb(PIC2_DATA, (uint8_t) offsetSlave);

    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, a1);
    outb(PIC1_DATA, a2);
}

void picDisable(void) {
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

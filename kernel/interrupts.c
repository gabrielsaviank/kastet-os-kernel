#include <stdint.h>
#include "helpers/console.h"
#include "interrupts.h"


struct interrupt_descriptor_table_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint16_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
}__attribute__((packed));

struct interrupt_descriptor_table_pointer {
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

static struct interrupt_descriptor_table_entry idt[256];
static struct interrupt_descriptor_table_pointer idt_desc;

extern void interrupt_descriptor_table_load(void* interrupt_descriptor_table_pointer);
extern void isr0(void);


// “When interrupt num happens, jump to the function at address base, using the code segment sel,
// with permission/type flags flags.”
static void interrupt_descriptor_table_set_gate(int num, uint32_t base, uint16_t code_segment, uint8_t flags) {
    idt[num].offset_low  = (uint16_t)(base & 0xFFFF);
    idt[num].selector = code_segment;
    idt[num].zero = 0; // Always ZERO otherwise CPU explode haha
    idt[num].type_attr = flags;
    idt[num].offset_high = (uint16_t)((base >> 16) & 0xFFFF);
}

void exception_handler(uint32_t int_number, uint32_t error_code) {
    (void)error_code;

    console_write("Exception: ");

    if (int_number == 0) {
        console_write("Divide-by-zero (int 0)\n");
    } else {
        console_write("Unknown exception\n");
    }

    console_write("SYSTEM HALTED \n");

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}


void interrupt_descriptor_table_init(void) {
    for (int i = 0;i < 256; i++) {
        idt[i].offset_low  = 0;
        idt[i].selector    = 0;
        idt[i].zero        = 0;
        idt[i].type_attr   = 0;
        idt[i].offset_high = 0;
    }

    interrupt_descriptor_table_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_desc.limit = sizeof(idt) - 1;
    idt_desc.base  = (uint32_t)&idt;

    interrupt_descriptor_table_load(&idt_desc);
}
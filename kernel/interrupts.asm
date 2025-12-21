[bits 32]

global interrupt_descriptor_table_load
global isr0

extern exception_handler

interrupt_descriptor_table_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret

isr0:
    pusha
    push dword 0
    push dword 0
    call exception_handler
    add esp, 8
    popa
    iretd
; GLOSSARY Because I'm not fluent in assembly
; lidt refres to Load Interruption descriptor
; ldtr descriptor table register

[bits 32]

global idt_load
global isr0

extern exception_handler

idt_load:
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

[org 0x7C00]
bits 16

KERNEL_LOAD_ADDRESS equ 0x1000
KERNEL_LOAD_sEG equ KERNEL_LOAD_ADDRESS >> 4
KERNEL_SECTORS equ 4

start:
    cli

    mov[boot_drive], dl

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00 ; STACK POINTER, stack grows down from here

    mov si, msg
    call print_string

hang:
    hlt ; HALT CPU PAPIEREN BITTE? Jokes apart, this halt the CPU
    jmp hang

print_string:
    lodsb
    test al, al
    jz .done

    mov ah, 0x0E
    mov bh, 0x00
    mov bl, 0x07
    int 0x10

    jmp print_string
.done:
    ret

msg db "Hallo von meinem bootloader!", 0

times 510-($-$$) db 0   ; 20) Fill with zeros until byte 510
dw 0xAA55               ; 21) Boot signature (0x55AA in memory)

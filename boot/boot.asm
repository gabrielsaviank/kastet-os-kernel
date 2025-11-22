; boot/boot.asm
; 16-bit boot sector:
;  - prints a message using BIOS
;  - loads kernel sectors into memory at 0x1000
;  - switches to 32-bit protected mode
;  - calls kernel_main at 0x1000

[org 0x7C00]
bits 16

KERNEL_LOAD_ADDR   equ 0x1000
KERNEL_LOAD_SEG    equ KERNEL_LOAD_ADDR >> 4
KERNEL_SECTORS     equ 20

start:
    cli

    mov [boot_drive], dl

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov si, msg16
    call print_string

    call load_kernel

    call enter_protected_mode

.hang16:
    hlt
    jmp .hang16

print_string:
    lodsb                   ; AL = [DS:SI], SI++
    test al, al
    jz .done
    mov ah, 0x0E
    mov bh, 0x00
    mov bl, 0x07
    int 0x10
    jmp print_string
.done:
    ret


load_kernel:
    ; ES:BX = KERNEL_LOAD_SEG:0
    mov ax, KERNEL_LOAD_SEG
    mov es, ax
    xor bx, bx

    mov ah, 0x02
    mov al, KERNEL_SECTORS
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    mov dl, [boot_drive]

    int 0x13
    jc disk_error           ; if carry flag set, error

    ret

disk_error:
    mov si, disk_err_msg
    call print_string
    jmp $


enter_protected_mode:
    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or  eax, 0x1
    mov cr0, eax

    jmp 0x08:protected_mode_entry

bits 32

protected_mode_entry:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000

    mov eax, KERNEL_LOAD_ADDR
    call eax

.hang32:
    hlt
    jmp .hang32


align 8
gdt_start:
    dq 0

    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start


bits 16

boot_drive db 0

msg16 db "Bootloader: loading 32-bit kernel...", 0
disk_err_msg db "Disk read error!", 0

times 510-($-$$) db 0
dw 0xAA55

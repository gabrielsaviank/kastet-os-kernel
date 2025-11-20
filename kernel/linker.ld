ENTRY(kernel_main)

SECTIONS
{
    /* address that we've defined in the boot(assembly code)*/
    . = 0x100;

    .text ALIGN(4) : {
        *(.text*)
    }

    .rodata ALIGN(4) : {
        *(.rodata*)
    }

    .data ALIGN(4) : {
        *(.data*)
    }

    .bss ALIGN(4) : {
        *(.bss*)
        *(COMMON)
    }
}
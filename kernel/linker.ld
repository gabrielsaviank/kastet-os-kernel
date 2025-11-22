ENTRY(kernel_main)

SECTIONS
{
    /* Load address where the bootloader will place us */
    . = 0x1000;

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

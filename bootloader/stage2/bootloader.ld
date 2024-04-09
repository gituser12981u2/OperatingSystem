ENTRY(protected_mode)

SECTIONS {
    . = 0x10000;

    /* Place the assembly code section first */
    .boot : {
        *(.boot)
    }

    /* Place the C code sections after the assembly code */
    .text : {
        *(.text .text.*)
    }
    .rodata : {
        *(.rodata .rodata.*)
    }
    .data : {
        *(.data .data.*)
    }
    .bss : {
        *(.bss .bss.*)
    }
}
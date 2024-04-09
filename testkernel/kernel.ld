ENTRY(start)

SECTIONS {
    . = 0x100000;

    /* Place the assembly code section first */
    .boot : {
        *(.start)
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
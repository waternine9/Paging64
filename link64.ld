OUTPUT_FORMAT(binary)
SECTIONS {
    .main64 : 
    {
        *(.main64)
        *(.text)
    }
    .rodata : 
    {
        *(.rodata)
    }
    .data : 
    {
        *(.data)
    }
    . = 0x1A01000;
    .bss : 
    {
        *(.bss)
    }
}

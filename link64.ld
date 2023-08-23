OUTPUT_FORMAT(binary)
SECTIONS {
    . = 0xFFFFFFFFD0000000;
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
    .bss : 
    {
        *(.bss)
    }
}

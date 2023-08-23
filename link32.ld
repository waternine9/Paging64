OUTPUT_FORMAT(binary)
SECTIONS {
    .main32 : 
    {
        *(.main32)
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
    . = 0x1A00000;
    .bss : 
    {
        *(.bss)
    }
}

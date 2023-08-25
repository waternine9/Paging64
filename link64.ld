OUTPUT_FORMAT(binary)
SECTIONS {
    . = 0xFFFFFFFFC0000000;
    .main64 :
    {
        *(.main64)
        *(.text)
        *(.handlers)
    }
    .rodata : 
    {
        *(.rodata)
    }
    .data : 
    {
        *(.data)
    }
    . = 0xFFFFFFFFC1200000;
    .bss : 
    {
        *(.bss)
    }
}

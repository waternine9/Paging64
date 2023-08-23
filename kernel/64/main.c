#include "../include.h"

const char* Str = "Long Mode Started";

volatile void __attribute__((section(".main64"))) main64()
{
    const char* Str2 = Str;
    int i = 0;
    while (*Str2) *((volatile uint16_t*)0xb8000 + i++) = 0x0F00 | (*Str2++);
    
    
    while (1);
}
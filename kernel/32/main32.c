#include "../include.h"
#include "vmalloc32.h"

volatile void __attribute__((section(".main32"))) main32()
{
    Init();
    if (!IdMap(0x0, 0x70000000))
    {
        asm volatile ("cli\nhlt\n");
    }
}
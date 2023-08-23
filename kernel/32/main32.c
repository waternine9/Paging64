#include "../include.h"
#include "vmalloc32.h"

volatile void __attribute__((section(".main32"))) main32()
{
    Init();
    if (!IdMap(0x0, 0x100000))
    {
        *(uint16_t*)0xb8002 = 0x0F00 | '0';
        asm volatile ("cli\nhlt\n");
    }
    if (!IdMap(0x2000000, 0x20000000))
    {
        *(uint16_t*)0xb8002 = 0x0F00 | '0';
        asm volatile ("cli\nhlt\n");
    }
    if (!IdMap(0x60000000, 0x10001000))
    {
        *(uint16_t*)0xb8002 = 0x0F00 | '1';
        asm volatile ("cli\nhlt\n");
    }
    if (!MapHigher(0xD0000000, 0x100000, 0x100000))
    {
        *(uint16_t*)0xb8002 = 0x0F00 | '2';
        asm volatile ("cli\nhlt\n");
    }
}
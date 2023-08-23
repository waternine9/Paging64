#include "../include.h"

volatile void __attribute__((section(".main64"))) main64()
{
    *(volatile uint16_t*)0xb8000 = 0x0F00 | 'A';
}
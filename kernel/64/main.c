#include "../include.h"
#include "vmalloc.h"
#include "idt.h"
#include "pic.h"

static const char* Str = "Long Mode Started";

volatile void __attribute__((section(".main64"))) main64()
{
    for (uint64_t i = 0;i < 0x200000;i++)
    {
        *(uint8_t*)(0xFFFFFFFFC1200000 + i) = 0;
    }
    AllocInit();
    PicInit();
    PicSetMask(0xFFFF);
    IdtInit();
    PicSetMask(0);
    *(uint64_t*)0xB8000 = 0;
}
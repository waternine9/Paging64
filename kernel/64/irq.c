#include "pic.h"

__attribute__((section(".data"))) static uint16_t Ticks = 0;
void CHandlerIRQ0()
{
    *(uint16_t*)0xb8000 = 0x0F00 | ('0' + (Ticks++ % 10));
    PicEndOfInterrupt(0);
}
/* Keyboard Interrupt */
void CHandlerIRQ1()
{
    PicEndOfInterrupt(1);
}
/* Channel for Secondary PIC, don't use. */
void CHandlerIRQ2()
{
    PicEndOfInterrupt(2);
}
/* COM2 */
void CHandlerIRQ3()
{
    PicEndOfInterrupt(3);
}
/* COM1 */
void CHandlerIRQ4()
{
    PicEndOfInterrupt(4);
}
/* LPT2 */
void CHandlerIRQ5()
{
    PicEndOfInterrupt(5);
}
/* Floppy Disk */
void CHandlerIRQ6()
{
    PicEndOfInterrupt(6);
}
/* LPT1 (spurious) */
void CHandlerIRQ7()
{
    PicEndOfInterrupt(7);
}
/* CMOS Real time clock */
void CHandlerIRQ8()
{
    PicEndOfInterrupt(8);
}
/* Free for peripherals */
void CHandlerIRQ9()
{
    PicEndOfInterrupt(9);
}
/* Free for peripherals */
void CHandlerIRQ10()
{
    PicEndOfInterrupt(10);
}
/* Free for peripherals */
void CHandlerIRQ11()
{
    PicEndOfInterrupt(11);
}
/* PS/2 Mouse */
void CHandlerIRQ12()
{
    PicEndOfInterrupt(12);
}
/* FPU */
void CHandlerIRQ13()
{
    PicEndOfInterrupt(13);
}
/* Primary ATA */
void CHandlerIRQ14()
{
    PicEndOfInterrupt(14);
}
/* Secondary ATA */
void CHandlerIRQ15()
{
    PicEndOfInterrupt(15);
}
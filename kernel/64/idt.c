#include "idt.h"
#include "print.h"

#define PRESENT_BIT 47
#define DPL_BIT 45
#define RESERVED_1BIT 44

#define GATE_TYPE_BIT 40
#define RESERVED_5BIT 35
#define IST_BIT 32

#define OFFSET32_BIT 0
#define OFFSET16_BIT 48
#define SEGMENT_SEL_BIT 16
#define OFFSET0_BIT 0

typedef struct 
{
    uint16_t offset_1;        // offset bits 0..15
    uint16_t selector;        // a code segment selector in GDT or LDT
    uint8_t  ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    uint8_t  type_attributes; // gate type, dpl, and p fields
    uint16_t offset_2;        // offset bits 16..31
    uint32_t offset_3;        // offset bits 32..63
    uint32_t zero;            // reserved
} __attribute__((packed)) IDTEntry;

__attribute__((aligned(0x10))) static IDTEntry* IDTEntries;

/* Interrupt Handlers */

extern void LoadIDT();
extern void PageFaultS();
extern void GeneralProtectionFaultS();
extern void UnknownFaultS();

extern void HandlerIRQ0();
extern void HandlerIRQ1();
extern void HandlerIRQ2();
extern void HandlerIRQ3();
extern void HandlerIRQ4();
extern void HandlerIRQ5();
extern void HandlerIRQ6();
extern void HandlerIRQ7();
extern void HandlerIRQ8();
extern void HandlerIRQ9();
extern void HandlerIRQ10();
extern void HandlerIRQ11();
extern void HandlerIRQ12();
extern void HandlerIRQ13();
extern void HandlerIRQ14();
extern void HandlerIRQ15();

void PageFault(void)
{
    for (int i = 0; i < 256;i++)
    {
        *((uint16_t*)0xb8000 + i) = 0x0F00 | 'P';
    }
}

void GeneralProtectionFault(void)
{
    for (int i = 0; i < 256;i++)
    {
        *((uint16_t*)0xb8000 + i) = 0x0F00 | 'G';
    }
}

void UnknownFault(void)
{
    for (int i = 0; i < 256;i++)
    {
        *((uint16_t*)0xb8000 + i) = 0x0F00 | 'U';
    }
}

/* Now init */



void IdtInit()
{
    IDTEntries = 0xFFFFFFFFC1300000;
    static void (*Handlers[16])() = {
        HandlerIRQ0,  HandlerIRQ1,  HandlerIRQ2,  HandlerIRQ3,
        HandlerIRQ4,  HandlerIRQ5,  HandlerIRQ6,  HandlerIRQ7,
        HandlerIRQ8,  HandlerIRQ9,  HandlerIRQ10, HandlerIRQ11,
        HandlerIRQ12, HandlerIRQ13, HandlerIRQ14, HandlerIRQ15,
    };
    for (int i = 0;i < 256;i++)
    {
        IDTEntries[i] = (IDTEntry){ 0, 0, 0, 0, 0, 0, 0 };
        if (i == 0xE)   // PAGE FAULT
        {
            IDTEntries[i].type_attributes = 0x8F;
            IDTEntries[i].offset_3 = (((uint64_t)PageFaultS & 0xFFFFFFFF00000000ULL) >> 32);
            IDTEntries[i].offset_2 = (((uint64_t)PageFaultS & 0x00000000FFFF0000ULL) >> 16);
            IDTEntries[i].offset_1 = (((uint64_t)PageFaultS & 0x000000000000FFFFULL) >> 0);
            IDTEntries[i].selector = 0x18; // 64-bit code segment is at 0x18 in the GDT
            
            //asm volatile ("cli\nhlt" :: "a"((uint32_t)IDTEntries[i].offset_1), "d"(0x1234));
        }
        else if (i == 0xD)   // GP FAULT
        {
            IDTEntries[i].type_attributes = 0x8F;
            IDTEntries[i].offset_3 = (((uint64_t)GeneralProtectionFaultS & 0xFFFFFFFF00000000ULL) >> 32);
            IDTEntries[i].offset_2 = (((uint64_t)GeneralProtectionFaultS & 0x00000000FFFF0000ULL) >> 16);
            IDTEntries[i].offset_1 = (((uint64_t)GeneralProtectionFaultS & 0x000000000000FFFFULL) >> 0);
            IDTEntries[i].selector = 0x18; // 64-bit code segment is at 0x18 in the GDT
        }
        else if (i < 32) // UNKNOWN FAULT
        {
            IDTEntries[i].type_attributes = 0x8F;
            IDTEntries[i].offset_3 = (((uint64_t)UnknownFaultS & 0xFFFFFFFF00000000ULL) >> 32);
            IDTEntries[i].offset_2 = (((uint64_t)UnknownFaultS & 0x00000000FFFF0000ULL) >> 16);
            IDTEntries[i].offset_1 = (((uint64_t)UnknownFaultS & 0x000000000000FFFFULL) >> 0);
            IDTEntries[i].selector = 0x18; // 64-bit code segment is at 0x18 in the GDT
        }
        else if (i < 48)
        {
            IDTEntries[i].type_attributes = 0x8E;
            IDTEntries[i].offset_3 = (((uint64_t)Handlers[i - 32] & 0xFFFFFFFF00000000ULL) >> 32);
            IDTEntries[i].offset_2 = (((uint64_t)Handlers[i - 32] & 0x00000000FFFF0000ULL) >> 16);
            IDTEntries[i].offset_1 = (((uint64_t)Handlers[i - 32] & 0x000000000000FFFFULL) >> 0);
            IDTEntries[i].selector = 0x18; // 64-bit code segment is at 0x18 in the GDT
        }
    }
    *(uint16_t*)0x7E50 = sizeof(IDTEntry) * 256 - 1;
    *(uint64_t*)0x7E52 = (uint64_t)IDTEntries;
    LoadIDT();
}
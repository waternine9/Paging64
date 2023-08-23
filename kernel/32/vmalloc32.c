#include "vmalloc32.h"

#define MAX_ADDR_BIT 47
#define PRESENT_BIT 0
#define READWRITE_BIT 1
#define USER_SUPERVISOR_BIT 2
#define WRITE_THROUGH_BIT 3
#define CACHE_DISABLE_BIT 4
#define ACCESSED_BIT 5
#define PAGE_SIZE_BIT 7
#define ADDR_BIT 12
#define AVAILABLE_BIT 52
#define NO_EXECUTE_BIT 63
#define PMT_SIZE 1024

typedef struct
{
    uint32_t Low;
    uint32_t High;
} __attribute__((packed)) PageMapTable;

PageMapTable* Tier4;
PageMapTable* AllocOffset = 0x2000000;

volatile PageMapTable InitTable(int Depth)
{
    PageMapTable Ret;
    Ret.High = 0;
    Ret.Low = (size_t)(AllocOffset) & 0xFFFFF000;
    
    PageMapTable* TempAllocOffset = AllocOffset;
    AllocOffset += PMT_SIZE;
    for (int i = 0;i < PMT_SIZE;i++)
    {
        (*(TempAllocOffset + i)) = (PageMapTable){ 0, 0 };
    }
    if (Depth < 3) for (int i = 0;i < (Depth == 1 ? 16 : (Depth == 0 ? 1 : PMT_SIZE));i++)
    {
        (*TempAllocOffset) = InitTable(Depth + 1);
        TempAllocOffset++;
    }
    return Ret;
}

volatile void Init()
{
    AllocOffset = 0x2000000;
    Tier4 = InitTable(0).Low;
}

volatile bool IdMapR(PageMapTable* Pte, void** Addr, int* Size, int Depth)
{
    int i = 0;
    while (*Size >= 0 && i < PMT_SIZE)
    {
        i++;
        if (Pte->Low & (1 << PRESENT_BIT)) 
        {
            *(uint16_t*)0xb8000 = 0x0F00 | ('0' + Depth);
            return false;
        }
        Pte->Low |= 1 << PRESENT_BIT;
        Pte->Low |= 1 << READWRITE_BIT;
        // ALSO SET USER BIT WHEN ALLOCATING USER MEMORY
        if (Depth == 3)
        {
            Pte->Low |= (size_t)(*Addr) & 0xFFFFF000;
            (*Addr) += 0x1000; // 4 KiB Pages
            (*Size) -= 0x1000;
        }
        else
        {   
            if (!IdMapR(Pte->Low & 0xFFFFF000, Addr, Size, Depth + 1)) return false;
        }
        Pte++;
    }
    return true;
}

volatile bool IdMap(void* Addr, int Size)
{
    return IdMapR(Tier4, &Addr, &Size, 0);
}
#include "vmalloc.h"

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
#define PMT_SIZE 512

typedef struct
{
    uint64_t Data;
} __attribute__((packed, scalar_storage_order("little-endian"))) PageMapTable;

PageMapTable* Tier4;
PageMapTable* AllocOffset = 0x400000;

volatile PageMapTable InitTable(int Depth)
{
    PageMapTable Ret;
    Ret.Data = (uint64_t)AllocOffset & 0xFFFFFFFFF000;
    
    PageMapTable* TempAllocOffset = AllocOffset;
    AllocOffset += PMT_SIZE;
    if (Depth < 3) for (int i = 0;i < PMT_SIZE;i++)
    {
        (*TempAllocOffset) = InitTable(Depth + 1);
        TempAllocOffset++;
    }
    return Ret;
}

volatile bool AllocPage(PageMapTable* BasePte, uint64_t vAddr, uint64_t pAddr, int Depth)
{
    uint64_t Index = (vAddr & (0x1FF << ((3 - Depth) * 9 + 12))) >> ((3 - Depth) * 9 + 12);
    Index &= 0x1FF;
    if (Depth == 3)
    {
        BasePte[Index].Data = pAddr & 0xFFFFFFFFF000;
        BasePte[Index].Data |= 1 << PRESENT_BIT;
        BasePte[Index].Data |= 1 << READWRITE_BIT;
        return true;
    }
    if (BasePte[Index].Data == 0) 
    {
        AllocOffset += PMT_SIZE;
        BasePte[Index].Data = (uint64_t)AllocOffset & 0xFFFFFFFFF000;
        for (int i = 0;i < PMT_SIZE;i++)
        {
            AllocOffset[i].Data = 0;
        }
    }
    BasePte[Index].Data |= 1 << PRESENT_BIT;
    BasePte[Index].Data |= 1 << READWRITE_BIT;
    return AllocPage(BasePte[Index].Data & 0xFFFFFFFFF000, vAddr, pAddr, Depth + 1);
}

volatile void FreePage(PageMapTable* BasePte, uint64_t vAddr, int Depth)
{
    uint64_t Index = ((uint64_t)vAddr & (0x1FF << ((3 - Depth) * 9 + 12))) >> ((3 - Depth) * 9 + 12);
    Index &= 0x1FF;
    if (Depth == 3)
    {
        if (BasePte[Index].Data & (1 << PRESENT_BIT)) return;
        BasePte[Index].Data = 0;
        return;
    }
    if (BasePte[Index].Data == 0) 
    {
        return;
    }
    FreePage(BasePte[Index].Data & 0xFFFFFFFFF000, vAddr, Depth + 1);
    PageMapTable* Check = BasePte[Index].Data & 0xFFFFFFFFF000;
    for (int i = 0;i < PMT_SIZE;i++)
    {
        if (Check[i].Data) return;
    }
    BasePte[Index].Data = 0;
}

volatile bool AllocIdMap(uint64_t Addr, uint64_t Size)
{
    for (uint64_t i = 0;i <= Size / 0x1000;i++)
    {
        if (!AllocPage(Tier4, Addr + i * 0x1000, Addr + i * 0x1000, 0)) return false;
    }
    return true;
}

volatile bool AllocMap(uint64_t vAddr, uint64_t pAddr, uint64_t Size)
{
    for (uint64_t i = 0;i <= Size / 0x1000;i++)
    {
        if (!AllocPage(Tier4, vAddr + i * 0x1000, pAddr + i * 0x1000, 0)) return false;
    }
    return true;
}

static inline void Flush() 
{    
   asm volatile("mov $0x400000, %rax\nmov %rax, %cr3");
}

volatile void AllocUnMap(uint64_t vAddr, uint64_t Size)
{
    for (uint64_t i = 0;i <= Size / 0x1000;i++)
    {
        FreePage(Tier4, vAddr + i * 0x1000, 0);
    }
}

volatile void AllocInit()
{
    AllocOffset = 0x500000;
    Tier4 = 0x400000;
}
; Include the bootloader
section .boot

[BITS 32]
[ORG 0x8400]

kstart:
; Set up segments and stack pointer
    mov eax, 16
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
 
; Set stack to be 14 MB according to x86 Memory Map
    mov eax, 0x00EFFFFF
    mov esp, eax
ReadLoop:
    mov eax, [CurrentLBA]
    cmp eax, 200
    jl ReadSectorsV8086

; Now call the paging initializer
    call 0x8600

    ; Enable PAE
    mov edx, cr4
    or  edx, (1 << 5)
    mov cr4, edx
    
    ; Set LME (long mode enable)
    mov ecx, 0xC0000080
    rdmsr
    or  eax, (1 << 8)
    wrmsr
    
    ; 0x2000000 is the kernel's PML4
    mov eax, 0x2000000
    mov cr3, eax
    
    ; Enable paging
    mov ebx, cr0
    or ebx, (1 << 31)
    mov cr0, ebx

    jmp 0x18:.fabled_64

    ; Don't proceed to the fabled 64-bit land before loading the 64-bit GDT
[BITS 64] ; The fabled 64-bit
.fabled_64:
    mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
; Now call the kernel
    call 0x100000
; For safety, halt if the kernel ever returns. Should not happen.
    cli
    hlt
    
[BITS 32]
CurrentLBA: dd 4 + (0x2000 / 512)
IDT16:
    dw 0x3FF       ; Limit
    dd 0x0         ; Base
ReadSectorsV8086:
    cli
    lgdt [GDT16Desc]
    jmp 8:.PMode16
[BITS 16]
.PMode16:
    mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

    mov eax, cr0
    and eax, ~1
    mov cr0, eax

    jmp 0:.RealMode
.RealMode:
    mov ax, 0x0
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
    mov sp, 0x7E00

    sti

    mov word [DAPACK.cnt], 4
    mov word [DAPACK.dest], 0x8400 + 2048
    mov eax, [CurrentLBA]
    mov [DAPACK.lba], eax
    mov si, DAPACK		; address of "disk address packet"
    mov ah, 0x42		; AL is unused
    mov dl, [0x7E00]
    int 0x13

;   Switch back to pmode
    cli
    lgdt [GDTDesc]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 8:.PMode
[BITS 32]
.PMode:
; Set up segments and stack pointer
    mov eax, 16
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax

    mov eax, 0x00EFFFFF
    mov esp, eax

    mov edi, 0x100000
    mov eax, [CurrentLBA]
    sub eax, 4 + (0x2000 / 512)
    imul eax, 512
    add edi, eax
    mov esi, 0x8400 + 2048
    mov ecx, (512 * 4) / 2
    rep movsw
    mov eax, [CurrentLBA]
    add eax, 4
    mov [CurrentLBA], eax
    jmp ReadLoop

align 4
DAPACK:
db	0x10
db	0
.cnt:	dw  16		; int 13 resets this to # of blocks actually read/written
.dest:	dw	0x8400		; memory buffer destination address (0:7c00)
dw	0		; in memory page zero
.lba:	dd  4		; put the lba to read in this spot
dd	0		; more storage bytes only for big lba's ( > 4 bytes )

GDTStart:
    dq 0 
GDTCode:
    dw 0xFFFF     ; Limit
    dw 0x0000     ; Base
    db 0x00       ; Base
    db 0b10011010 ; Access
    db 0b11001111 ; Flags + Limit
    db 0x00       ; Base
GDTData:
    dw 0xFFFF     ; Limit
    dw 0x0000     ; Base
    db 0x00       ; Base
    db 0b10010010 ; Access
    db 0b11001111 ; Flags + Limit
    db 0x00       ; Base
GDTCode64:
    dw 0xFFFF     ; Limit
    dw 0x0000     ; Base
    db 0x00       ; Base
    db 0b10011010 ; Access
    db 0b10101111 ; Flags + Limit
    db 0x00       ; Base
GDTEnd:

GDTDesc:
    .GDTSize dw GDTEnd - GDTStart - 1 ; GDT size 
    .GDTAddr dd GDTStart          ; GDT address

GDT16Start:
    dq 0
GDT16Code: ; 16-bit code segment descriptor
    dw 0xFFFF     ; Limit
    dw 0x0000     ; Base
    db 0x00       ; Base
    db 0b10011010 ; Access
    db 0b00001111 ; Flags + Limit
    db 0x00       ; Base

GDT16Data: ; 16-bit data segment descriptor
    dw 0xFFFF     ; Limit
    dw 0x0000     ; Base
    db 0x00       ; Base
    db 0b10010010 ; Access
    db 0b00001111 ; Flags + Limit
    db 0x00       ; Base
GDT16End:

GDT16Desc:
    .GDTSize dw GDT16End - GDT16Start - 1
    .GDTAddr dd GDT16Start

times 512 - ($ - $$) db 0

incbin "paging32.img"

times 2048 - ($ - $$) db 0

times 0x2000 - ($ - $$) db 0

incbin "kernel.img"

section .kernelend
kernelend:
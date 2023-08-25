[BITS 64]

extern PageFault
extern GeneralProtectionFault
extern UnknownFault

section .handlers

global LoadIDT
global PageFaultS
global GeneralProtectionFaultS
global UnknownFaultS

global HandlerIRQ0
global HandlerIRQ1
global HandlerIRQ2
global HandlerIRQ3
global HandlerIRQ4
global HandlerIRQ5
global HandlerIRQ6 
global HandlerIRQ7
global HandlerIRQ8
global HandlerIRQ9
global HandlerIRQ10 
global HandlerIRQ11
global HandlerIRQ12 
global HandlerIRQ13 
global HandlerIRQ14 
global HandlerIRQ15

extern CHandlerIRQ0
extern CHandlerIRQ1
extern CHandlerIRQ2
extern CHandlerIRQ3
extern CHandlerIRQ4
extern CHandlerIRQ5
extern CHandlerIRQ6
extern CHandlerIRQ7
extern CHandlerIRQ8
extern CHandlerIRQ9
extern CHandlerIRQ10
extern CHandlerIRQ11
extern CHandlerIRQ12
extern CHandlerIRQ13
extern CHandlerIRQ14
extern CHandlerIRQ15

%macro PUSHA64 0
    push rax
    push rbx
    push rcx
    push rdx
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov ax, 0x10
    mov ss, ax
    mov es, ax
    mov ds, ax
    mov fs, ax
    mov gs, ax
%endmacro

%macro POPA64 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

PageFaultS:
    cli
    cld
    mov word [0xb8016], 0x0F00 | 'P'
    PUSHA64
    call PageFault
    POPA64
    mov rsp, 16
    sti
    iretq

GeneralProtectionFaultS:
    cli
    cld
    mov word [0xb8016], 0x0F00 | 'G'
    PUSHA64
    call GeneralProtectionFault
    POPA64
    add rsp, 16
    sti
    iretq

UnknownFaultS:
    cli
    cld
    mov word [0xb8016], 0x0F00 | 'U'
    PUSHA64
    call UnknownFault
    POPA64
    mov rsp, 8
    sti
    iretq

HandlerIRQ0:
    cli
    cld
    PUSHA64
    call CHandlerIRQ0
    POPA64
    sti
    iretq

HandlerIRQ1:
    cli
    cld
    PUSHA64
    call CHandlerIRQ1
    POPA64
    sti
    iretq

HandlerIRQ2:
    cli
    cld
    PUSHA64
    call CHandlerIRQ2
    POPA64
    sti
    iretq

HandlerIRQ3:
    cli
    cld
    PUSHA64
    call CHandlerIRQ3
    POPA64
    sti
    iretq

HandlerIRQ4:
    cli
    cld
    PUSHA64
    call CHandlerIRQ4
    POPA64
    sti
    iretq

HandlerIRQ5:
    cli
    cld
    PUSHA64
    call CHandlerIRQ5
    POPA64
    sti
    iretq

HandlerIRQ6:
    cli
    cld
    PUSHA64
    call CHandlerIRQ6
    POPA64
    sti
    iretq

HandlerIRQ7:
    cli
    cld
    PUSHA64
    call CHandlerIRQ7
    POPA64
    sti
    iretq

HandlerIRQ8:
    cli
    cld
    PUSHA64
    call CHandlerIRQ8
    POPA64
    sti
    iretq

HandlerIRQ9:
    cli
    cld
    PUSHA64
    call CHandlerIRQ9
    POPA64
    sti
    iretq

HandlerIRQ10:
    cli
    cld
    PUSHA64
    call CHandlerIRQ10
    POPA64
    sti
    iretq

HandlerIRQ11:
    cli
    cld
    PUSHA64
    call CHandlerIRQ11
    POPA64
    sti
    iretq

HandlerIRQ12:
    cli
    cld
    PUSHA64
    call CHandlerIRQ12
    POPA64
    sti
    iretq

HandlerIRQ13:
    cli
    cld
    PUSHA64
    call CHandlerIRQ13
    POPA64
    sti
    iretq

HandlerIRQ14:
    cli
    cld
    PUSHA64
    call CHandlerIRQ14
    POPA64
    sti
    iretq

HandlerIRQ15:
    cli
    cld
    PUSHA64
    call CHandlerIRQ15
    POPA64
    sti
    iretq

LoadIDT:
    lidt [0x7E50]
    sti
    ret
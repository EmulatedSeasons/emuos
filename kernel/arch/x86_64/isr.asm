; The assembly part of the interrupt service routines

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31
global isr32

; macros to make up for the lack of pusha/popa
%macro pushregs 0
    push rax
    push rdi
    push rsi
    push rdx
    push rcx
    push r8
    push r9
    push r10
    push r11
%endmacro

%macro popregs 0
    pop r11
    pop r10
    pop r9
    pop r8
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rax
%endmacro

%macro halt 0
    .loop
        hlt
        jmp .loop
%endmacro

section .text
align 4

extern exception_handler

isr0:
    xchg bx, bx
    pushregs
    mov rdi, 0
    call exception_handler
    halta
    popregs
    iretq

isr1:
    pushregs
    mov rdi, 1
    call exception_handler
    xchg bx, bx
    popregs
    iretq

isr2:
    pushregs
    mov rdi, 2
    call exception_handler
    popregs
    iretq

isr3:
    pushregs
    mov rdi, 3
    call exception_handler
    popregs
    iretq

isr4:
    pushregs
    mov rdi, 4
    call exception_handler
    popregs
    iretq

isr5:
    pushregs
    mov rdi, 5
    call exception_handler
    popregs
    iretq

isr6:
    pushregs
    mov rdi, 6
    call exception_handler
    halt
    popregs
    iretq

isr7:
    pushregs
    mov rdi, 7
    call exception_handler
    popregs
    iretq

isr8:
    pushregs
    mov rdi, 8
    call exception_handler
    halt
    popregs
    iretq

isr9:
    pushregs
    mov rdi, 9
    call exception_handler
    popregs
    iretq

isr10:
    pushregs
    mov rdi, 10
    call exception_handler
    popregs
    ; will deal with error codes later. need to be removed from the stack first
    ; not sure if this works right yet
    sub esp, 8
    iretq

isr11:
    pushregs
    mov rdi, 11
    call exception_handler
    popregs
    sub esp, 8
    iretq

isr12:
    pushregs
    mov rdi, 12
    call exception_handler
    popregs
    sub esp, 8
    iretq

isr13:
    pushregs
    mov rdi, 13
    call exception_handler
    popregs
    sub esp, 8
    iretq

isr14:
    pushregs
    mov rdi, 14
    call exception_handler
    popregs
    sub esp, 8
    iretq

isr15:
    pushregs
    mov rdi, 15
    call exception_handler
    popregs
    iretq

isr16:
    pushregs
    mov rdi, 16
    call exception_handler
    popregs
    iretq

isr17:
    pushregs
    mov rdi, 17
    call exception_handler
    popregs
    sub esp, 8
    iretq

isr18:
    pushregs
    mov rdi, 18
    call exception_handler
    halt
    popregs
    iretq

isr19:
    pushregs
    mov rdi, 19
    call exception_handler
    popregs
    iretq

isr20:
    pushregs
    mov rdi, 20
    call exception_handler
    popregs
    iretq

isr21:
    pushregs
    mov rdi, 21
    call exception_handler
    popregs
    sub esp, 8
    iretq

isr22:
    pushregs
    mov rdi, 22
    call exception_handler
    popregs
    iretq

isr23:
    pushregs
    mov rdi, 23
    call exception_handler
    popregs
    iretq

isr24:
    pushregs
    mov rdi, 24
    call exception_handler
    popregs
    iretq

isr25:
    pushregs
    mov rdi, 25
    call exception_handler
    popregs
    iretq

isr26:
    pushregs
    mov rdi, 26
    call exception_handler
    popregs
    iretq

isr27:
    pushregs
    mov rdi, 27
    call exception_handler
    popregs
    iretq

isr28:
    pushregs
    mov rdi, 28
    call exception_handler
    popregs
    iretq

isr29:
    pushregs
    mov rdi, 29
    call exception_handler
    popregs
    sub esp, 8
    iretq

isr30:
    pushregs
    mov rdi, 30
    call exception_handler
    popregs
    sub esp, 8
    iretq

isr31:
    pushregs
    mov rdi, 31
    call exception_handler
    popregs
    iretq
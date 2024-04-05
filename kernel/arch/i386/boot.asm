; multiboot header constants
MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
MBFLAGS equ MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC+MBFLAGS)

; multiboot header
section .multiboot
align 4
    dd MAGIC
    dd MBFLAGS
    dd CHECKSUM

section .data
%include "arch/i386/gdt.asm"

; defines the stack
section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

; temporary paging tables
align 4
boot_page_directory:
    resb 4096
boot_page_table:
    resb 4096


section .text
global _start:function (_start.end - _start)
_start:
    cli
    ; setup stack
    mov esp, stack_top

    ; push the multiboot header data into the stack for later use
    push eax
    push ebx

    ; get multiboot header data (for memory map)
    ;extern multiboot_main
    ;call multiboot_main

    ;extern _init
    ;Scall _init

    lgdt [gdt_descriptor]
    jmp 0x08:.gdt_jmp

.gdt_jmp:
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp .gdt_jmp2

.gdt_jmp2:

    

    ; setup paging (to map kernel to the higher half)
    ;mov eax, 0
    ;mov cr3, eax
    ;mov eax, cr0
    ;or eax, 0x80000001
    ;mov cr0, eax

    ; setup idt
    extern idt_setup
    call idt_setup
    extern idtr
    lidt [idtr]
    
    ; initialize pic
    push ebp
    mov ebp, esp
    mov ebx, 0x28
    push ebx
    mov ebx, 0x20
    push ebx

    extern pic_remap
    call pic_remap

    pop ebx
    pop ebx
    mov esp, ebp
    pop ebp

    sti

    ; enter the kernel
    extern kernel_main
    call kernel_main

    ; indefinite loop once system exits
    ;cli
.hang: hlt
    jmp .hang
.end:
; GDT and IDT loading routines

section .text
global load_gdt:function
load_gdt:
    lgdt [rdi]
    ret

global reload_segments:function
reload_segments:
    ; reload cs register
    push 0x08
    lea rax, [rel .reload_cs]
    push rax
    retfq

.reload_cs:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

global load_idt:function
load_idt:
    lidt [rdi]
    ret
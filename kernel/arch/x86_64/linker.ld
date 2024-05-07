OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

/* Excecution entry point symbol */
ENTRY(_start)

PHDRS
{
    requests    PT_LOAD     FLAGS((1 << 1) | (1 << 2)); /* Write + Read */
    text        PT_LOAD     FLAGS((1 << 0) | (1 << 2)); /* Execute + Read */
    rodata      PT_LOAD     FLAGS((1 << 2));            /* Read only */
    data        PT_LOAD     FLAGS((1 << 1) | (1 << 2)); /* Write + Read */
    dynamic     PT_DYNAMIC  FLAGS((1 << 1) | (1 << 2)); /* Dynamic PHDR for relocations */
}

SECTIONS
{
    /* Places kernel in the top 2GiB of the address space */
    . = 0xffffffff80000000;

    /* Used to get the beginning and end address of the kernel */
    __kernel_begin = .;

    .requests : {
        KEEP(*(.requests_start_marker))
        KEEP(*(.requests))
        KEEP(*(.requests_end_marker))
    } :requests

    . += CONSTANT(MAXPAGESIZE);

    /* code. multiboot header comes first */
    .text : {
        *(.text .text.*)
    } :text

    . += CONSTANT(MAXPAGESIZE);

    /* read only data */
    .rodata : {
        *(. .rodata.*)
    } :rodata

    /* init array symbols used to get start and end address of the array */
    .init_array : {
        __init_array = .;
        *(.init_array .init_array.*)
        __init_array_end = .;
    } :rodata

    . += CONSTANT(MAXPAGESIZE);

    /* read write data (initialized) */
    .data : {
        *(.data .data.*)
    } :data

    /* Dynamic section for relocations, both in its own PHDR and inside data PHDR */
    .dynamic : {
        *(.dynamic)
    } :data :dynamic

    . += CONSTANT(MAXPAGESIZE);

    /* read write data (uninitialized) and stack */
    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } :data

    /* Discard .note.* and .eh_frame since they may cause issues on some hosts */
    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
    }

    __kernel_end = .;
}
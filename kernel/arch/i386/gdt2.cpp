#include <stdio.h>
#include <stdint.h>
/*
// structure to be encoded at encodeGdtEntry()
struct GDT
{
    uint32_t base;
    uint32_t limit;
    uint8_t access_byte;
    uint8_t flags;
};

GDT strnulldesc {0, 0, 0, 0};
GDT strkernelcode {0, 0xFFFFF, 0x9A, 0xC};
GDT strkerneldata {0, 0xFFFFF, 0x92, 0xC};
GDT strusercode {0, 0xFFFFF, 0xFA, 0xC};
GDT struserdata {0, 0xFFFFF, 0xF2, 0xC};

uint8_t* nulldesc;
uint8_t* kernelcode;
uint8_t* kerneldata;
uint8_t* usercode;
uint8_t* userdata;


void encodeGdtEntry(uint8_t *target, struct GDT source)
{
    // Check the limit to make sure that it can be encoded
    if (source.limit > 0xFFFFF) {printf("GDT cannot encode limits larger than 0xFFFFF\n");}
 
    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0x0F;
 
    // Encode the base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;
 
    // Encode the access byte
    target[5] = source.access_byte;
 
    // Encode the flags
    target[6] |= (source.flags << 4);
}

extern "C" void setupGdt() {

} */
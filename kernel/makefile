# Kernel makefile

CFLAGS := -ffreestanding -Wall -Wextra -g -std=gnu99 -O2 -Iinclude
CXXFLAGS := -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -g -O2 -Iinclude
LDFLAGS := -T arch/$(ARCH)/linker.ld -ffreestanding -g -O2 -Iinclude
ASMFLAGS :=

ifeq ($(ARCH),x86_64)
CFLAGS += -mno-red-zone
CXXFLAGS += -mno-red-zone
LDFLAGS += -mno-red-zone
ASMFLAGS += -felf64
endif

ifndef SYSROOT
	$(error No sysroot specified)
endif


LIBS = -nostdlib -lck -lgcc

#Find all the source files
C_SOURCES := $(shell find $(PWD)/kernel -type f -name '*.c')
CPP_SOURCES := $(shell find $(PWD)/kernel -type f -name '*.cpp')
HEADERS := $(shell find $(PWD) -type f -name '*.h')
ASMFILES := $(shell find $(PWD) -type f -name '*.asm' ! -name 'crti.asm' ! -name 'crtn.asm')
CRTBEGIN := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)
CRTEND := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)

OBJECTS := $(patsubst %.c,%.o,$(C_SOURCES))
OBJECTS += $(patsubst %.cpp,%.o,$(CPP_SOURCES))
OBJECTS += $(patsubst %.asm,%.o,$(ASMFILES))

DEPFILES := $(patsubst %.c,%.d,$(C_SOURCES))
DEPFILES += $(patsubst %.cpp,%.d,$(CPP_SOURCES))

LINKLST := crti.o
LINKLST += $(CRTBEGIN)
LINKLST += $(OBJECTS)
LINKLST += $(LIBS)
LINKLST += $(CRTEND)
LINKLST += crtn.o

-include $(DEPFILES)

.PHONY: all crt clean kernel.bin

all: crti.o crtn.o kernel.bin

kernel.bin: ${OBJECTS}
	$(info [INFO] Linking kernel)
	$(CXX) ${LDFLAGS} -o $@ $(LINKLST)

%.o: %.cpp
	$(info [INFO] Compiling $<)
	$(CXX) ${CXXFLAGS} -MMD -MP -c $< -o $@

%.o: %.c
	$(info [INFO] Compiling $<)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

%.o: %.asm
	$(info [INFO] Assembling $<)
	$(NASM) $< $(ASMFLAGS) -o $@ -g

crti.o: 
	$(NASM) arch/$(ARCH)/crti.asm $(ASMFLAGS) -o $@

crtn.o: 
	$(NASM) arch/$(ARCH)/crtn.asm $(ASMFLAGS) -o $@

install-headers:
	cp -r --preserve=timestamps include/. $(SYSROOT)/usr/include

clean:
	$(info [INFO] Cleaning)
	$(RM) ${OBJECTS} $(DEPFILES) crti.o crtn.o
# Kernel makefile

ifndef GAS
	GAS = i686-elf-as
endif

ifndef CFLAGS
	CFLAGS = -ffreestanding -Wall -Wextra -g -std=gnu99 -O2 -Iinclude
	CFLAGS += --sysroot="$(SYSROOT)"
	CFLAGS += -isystem="/usr/include"
endif

ifndef CXXFLAGS
	CXXFLAGS = -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -D__is_kernel -g -O2 -Iinclude
	CXXFLAGS += -Iinclude
	CXXFLAGS += --sysroot="$(SYSROOT)"
	CXXFLAGS += -isystem="/usr/include"
endif

ifndef LDFLAGS
	LDFLAGS = -T arch/$(ARCH)/linker.ld -ffreestanding -g -O2 -Iinclude
	LDFLAGS += --sysroot="$(SYSROOT)"
	LDFLAGS += -isystem="/usr/include"
endif

ifndef SYSROOT
	$(error No sysroot specified)
endif


LIBS = -nostdlib -lgcc

#Find all the source files
C_SOURCES := $(shell find $(PWD)/kernel -type f -name '*.c')
CPP_SOURCES := $(shell find $(PWD)/kernel -type f -name '*.cpp')
HEADERS := $(shell find $(PWD) -type f -name '*.h')
ASMFILES := $(shell find $(PWD) -type f -name '*.asm' ! -name 'crti.asm' ! -name 'crtn.asm')
GASFILES := $(wildcard *.s)
CRTBEGIN := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)
CRTEND := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)

OBJECTS := $(C_SOURCES:.c=.o)
OBJECTS += ${CPP_SOURCES:.cpp=.o}
OBJECTS += ${ASMFILES:.asm=.o}

LINKLST := crti.o
LINKLST += $(CRTBEGIN)
LINKLST += $(OBJECTS)
LINKLST += $(LIBS)
LINKLST += $(CRTEND)
LINKLST += crtn.o

.PHONY: all crt clean

all: crti.o crtn.o kernel.bin

kernel.bin: ${OBJECTS}
	$(info [INFO] Linking kernel)
	$(CXX) ${LDFLAGS} -o ${PREFIX}/kernel.bin $(LINKLST)

%.o: %.cpp ${HEADERS}
	$(info [INFO] Compiling $<)
	$(CXX) ${CXXFLAGS} -c $< -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(info [INFO] Assembling assembly)
	$(NASM) $< -felf32 -o $@ -g

crti.o: 
	$(NASM) arch/$(ARCH)/crti.asm -felf32 -o $@

crtn.o: 
	$(NASM) arch/$(ARCH)/crtn.asm -felf32 -o $@

install-headers:
	cp -r --preserve=timestamps include $(SYSROOT)/usr/include

clean:
	$(info [INFO] Cleaning)
	rm ${OBJECTS} crti.o crtn.o
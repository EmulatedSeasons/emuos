# Kernel makefile

CFLAGS := -ffreestanding -Wall -Wextra -g -std=gnu99 -O2 -Iinclude --sysroot=$(SYSROOT)
CXXFLAGS := -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -g -O2 -Iinclude --sysroot=$(SYSROOT)
LDFLAGS := -T arch/$(ARCH)/linker.ld -ffreestanding -g -O2 -Iinclude --sysroot=$(SYSROOT)
ASMFLAGS :=

ifeq ($(ARCH),x86_64)
CFLAGS += -mno-red-zone -mcmodel=kernel -mno-sse -mno-sse2 -mno-mmx
CXXFLAGS += -mno-red-zone -mcmodel=kernel -mno-sse -mno-sse2 -mno-mmx
LDFLAGS += -mno-red-zone -mcmodel=kernel -mno-sse -mno-sse2 -mno-mmx
ASMFLAGS += -felf64
endif

ifndef SYSROOT
	$(error No sysroot specified)
endif

LIBS = -nostdlib -lc -lgcc

#Find all the source files
CPP_SRCS := $(shell find . -type f -name '*.cpp' -not -path './arch/*') \
	$(shell find $(PWD)/kernel/arch/$(ARCH) -type f -name '*.cpp')
#HEADERS := $(shell find $(PWD) -type f -name '*.h')
#ASMFILES := $(shell find $(PWD) -type f -name '*.asm' -prune $(PWD)/arch) \
	$(shell find $(PWD)/kernel/arch/$(ARCH) -type f -name '*.asm')

#OBJS := $(patsubst %.c,%.o,$(C_SOURCES))
OBJS := $(patsubst %.cpp,%.o,$(CPP_SRCS))
OBJS += $(patsubst %.asm,%.o,$(ASMFILES))

#DEPFILES := $(patsubst %.c,%.d,$(C_SOURCES))
DEPFILES := $(patsubst %.cpp,%.d,$(CPP_SRCS))

# LINKLST := crti.o
# LINKLST += $(CRTBEGIN)
LINKLST += $(OBJS)
LINKLST += $(LIBS)
# LINKLST += $(CRTEND)
# LINKLST += crtn.o

-include $(DEPFILES)

.PHONY: all clean

all: kernel.bin

kernel.bin: ${OBJS}
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

# crti.o: 
# 	$(NASM) arch/$(ARCH)/crti.asm $(ASMFLAGS) -o $@

# crtn.o: 
# 	$(NASM) arch/$(ARCH)/crtn.asm $(ASMFLAGS) -o $@

install-headers:
	cp -r --preserve=timestamps include/. $(SYSROOT)/usr/include

clean:
	$(info [INFO] Cleaning)
	$(RM) ${OBJS} $(DEPFILES)
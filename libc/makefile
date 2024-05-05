# libck makefile

ifndef AR
	AR = i686-elf-ar
endif

ifndef CFLAGS
	CFLAGS = -ffreestanding -Wall -Wextra -g -std=gnu11 -O2 -Iinclude
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
C_SOURCES := $(shell find $(PWD) -type f -name '*.c')
#CPP_SOURCES := $(shell find $(PWD)/kernel -type f -name '*.cpp')
HEADERS := $(shell find $(PWD) -type f -name '*.h')
#ASMFILES := $(shell find $(PWD) -type f -name '*.asm')

OBJECTS := $(patsubst %.c,%.o,$(C_SOURCES))
# OBJECTS += $(patsubst %.cpp,%.o,$(CPP_SOURCES))
# OBJECTS += $(patsubst %.asm,%.o,$(ASMFILES))

DEPFILES := $(patsubst %.c,%.d,$(C_SOURCES))
# DEPFILES += $(patsubst %.cpp,%.d,$(CPP_SOURCES))

LINKLST += $(OBJECTS)
LINKLST += $(LIBS)

-include $(DEPFILES)

.PHONY: all install-headers install-lib clean

all: libc.a install-lib

libc.a: ${OBJECTS}
	$(info [INFO] Linking $<)
	$(AR) rcs $@ $(OBJECTS)

%.o: %.cpp
	$(info [INFO] Compiling $<)
	$(CXX) ${CXXFLAGS} -MMD -MP -c $< -o $@

%.o: %.c
	$(info [INFO] Compiling $<)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

%.o: %.asm
	$(info [INFO] Assembling $<)
	$(NASM) $< -felf32 -o $@ -g

install-headers:
	cp -r --preserve=timestamps include/. $(SYSROOT)/usr/include

install-lib: libck.a
	cp -r $< $(SYSROOT)/usr/lib/.

clean:
	$(info [INFO] Cleaning)
	$(RM) ${OBJECTS} $(DEPFILES)
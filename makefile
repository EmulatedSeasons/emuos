# The makefile

OS_NAME		:= emuos
ARCH		= i386
SYSROOT = $(PWD)/sysroot

# Variables for easy access of tools like gcc and nasm
CC		= i686-elf-gcc
CXX		= i686-elf-g++
LD		= i686-elf-ld 
NASM		= nasm
QEMU		= qemu-system-i386
ASMFLAGS	= -felf32
#CXXFLAGS 	:= -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
#LDFLAGS	:= -ffreestanding -O2 -nostdlib 

.PHONY: all kernel.bin grub multiboot_test clean

all: libck.a kernel.bin grub

libck.a:
	$(info [INFO] Building libck)
	$(MAKE) -C ./libs/libck/ ARCH=$(ARCH) PREFIX=$(PWD) CC=$(CC) CXX=$(CXX) LD=$(LD) NASM=$(NASM) SYSROOT=$(SYSROOT)

kernel.bin:
	$(info [INFO] Building kernel)
	$(MAKE) -C ./kernel/ ARCH=$(ARCH) PREFIX=$(PWD) CC=$(CC) CXX=$(CXX) LD=$(LD) NASM=$(NASM) SYSROOT=$(SYSROOT)

grub: kernel.bin grub.cfg
	grub-file --is-x86-multiboot $<
	cp kernel.bin isodir/boot
	cp grub.cfg isodir/boot/grub
	grub-mkrescue -o $(OS_NAME).iso isodir

qemu: grub
	$(QEMU) -no-shutdown -no-reboot --serial stdio -s -m 512 -hda $(OS_NAME).iso

install: install-headers install-libraries

install-headers:
	$(MAKE) -C ./kernel/ install-headers SYSROOT=$(SYSROOT)
	$(MAKE) -C ./libs/libck/ install-headers SYSROOT=$(SYSROOT)

install-libraries:
		$(MAKE) -C ./libs/libck/ install-lib SYSROOT=$(SYSROOT)

clean:
	-@$(MAKE) -C ./kernel/ clean SYSROOT=$(SYSROOT)
	-@$(MAKE) -C ./libs/libck/ clean SYSROOT=$(SYSROOT)
	-@$(RM) $(wildcard *.bin *.a)

# The makefile

OS_NAME			:= emuos
export ARCH		?= x86_64
export SYSROOT 	= $(PWD)/sysroot

# Variables for easy access of tools like gcc and nasm
export CC		= $(ARCH)-elf-gcc
export CXX		= $(ARCH)-elf-g++
export AR		= $(ARCH)-elf-ar
export NASM		= nasm
QEMU			= qemu-system-$(ARCH)
#ASMFLAGS	= -felf32
#CXXFLAGS 	:= -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
#LDFLAGS	:= -ffreestanding -O2 -nostdlib 

.PHONY: all limine clean build-all

all: build-all limine

build-all: kernel/kernel.bin libc/libc.a

libc/libc.a: install-headers
	$(info [INFO] Building libc)
	$(MAKE) -C ./libc/ all

kernel/kernel.bin: libc/libc.a install-headers
	$(info [INFO] Building kernel)
	$(MAKE) -C ./kernel/ all

limine: build-all
	cp kernel/kernel.bin isodir/boot
	xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        isodir -o $(OS_NAME).iso
	limine bios-install $(OS_NAME).iso

qemu: limine
	$(QEMU) -no-shutdown -no-reboot --serial stdio -s -m 1024 -hda $(OS_NAME).iso

install: install-headers install-libraries

install-headers:
	$(MAKE) -C ./kernel/ install-headers
	$(MAKE) -C ./libc/ install-headers

install-libraries:
	$(MAKE) -C ./libc/ install-lib

clean:
	-@$(MAKE) -C ./kernel/ clean
	-@$(MAKE) -C ./libc/ clean
	-@$(RM) $(wildcard *.bin *.a)

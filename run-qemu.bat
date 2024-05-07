@echo off
qemu-system-x86_64 -m 128 -drive format=raw,media=cdrom,file=emuos.iso -serial stdio
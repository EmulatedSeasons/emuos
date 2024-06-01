# i have no idea what to name this project


Requires an `x86_64-elf` cross compiler as well as nasm and gnumake. libgcc must be built without redzone and mcmodel=kernel.

isodir/boot/grub and sysroot/ should be created before building for the first time. Headers also have to be copied to sysroot/include first time. Can be done with `make install-headers` or running `setup_sysroot.sh` which also sets up the folders that need to be present in the sysroot.

To build use `make`

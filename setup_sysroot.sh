# Copies include/ header files to sysroot/

# creates the needed sysroot folders if non existant
mkdir -p sysroot/usr/include
mkdir -p sysroot/usr/lib

# copies all needed header files for compiling
cp -r kernel/include/* sysroot/usr/include
cp -r libc/include/* sysroot/usr/include
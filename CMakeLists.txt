cmake_minimum_required(VERSION 3.22)
project(EmuOS LANGUAGES C CXX ASM_NASM)

set(CMAKE_TOOLCHAIN_FILE ${PROJECT_SOURCE_DIR}/misc/i686-elf.cmake)

file(GLOB ASM "")
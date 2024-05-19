#!/bin/sh

mkdir build
clang Tools/x86-DOSCC.c -o build/CC.bin
clang Tools/x86-DOSPP.c -o build/PP.bin
clang Tools/x86-DOSASM.c -o build/ASM.bin
nasm -o MBR BOOTLOADER.s

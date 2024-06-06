#!/bin/sh

mkdir build
clang Tools/x86-DOSCC.c -o build/CC.bin
clang Tools/x86-DOSPP.c -o build/PP.bin
clang Tools/x86-DOSASM.c -o build/ASM.bin
clang Tools/x86-DOSFSMOUNT.c -o build/FSMOUNT.bin
clang Tools/x86-DOSMAKE.c -o build/MAKE.bin
nasm -o MBR BOOTLOADER.s
nasm -o ROOT ROOT.s
nasm -o BOOT.CFG BOOT.s
dd if=MBR of=x86-DOS.img bs=512 count=1
dd if=ROOT of=x86-DOS.img bs=512 count=1 oseek=1
build/MAKE.bin BUILD
build/FSMOUNT.bin MAKEIMG.x86ms
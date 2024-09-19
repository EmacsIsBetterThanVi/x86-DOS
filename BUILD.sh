#!/bin/sh

mkdir build
gcc -iquote include Tools/x86-DOSCC.c -o build/CC.bin
gcc -iquote include Tools/x86-DOSPP.c -o build/PP.bin
gcc -iquote include Tools/x86-DOSASM.c -o build/ASM.bin
gcc -iquote include Tools/x86-DOSFSMOUNT.c -o build/FSMOUNT.bin
gcc -iquote include Tools/x86-DOSMAKE.c -o build/MAKE.bin
nasm -o MBR BOOTLOADER.s
nasm -o ROOT ROOT.s
nasm -o BOOT.CFG BOOT.s
nasm -o build/ASM.TBL DOS-64_Files/lib/ASM.s
dd if=MBR of=x86-DOS.img bs=512 count=1
dd if=ROOT of=x86-DOS.img bs=512 count=1 oseek=1
build/MAKE.bin BUILD
build/FSMOUNT.bin MAKEIMG.x86ms
#!/bin/sh
dd if=/dev/zero of=x86-DOS.img bs=512 count=2880
dd if=MBR of=x86-DOS.img bs=512 oseek=0
o=1
for i in $(ls img); do
dd if=img/$i of=x86-DOS.img bs=512 oseek=$o
o=$o+1
done

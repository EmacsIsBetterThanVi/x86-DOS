#!/bin/sh

#  MAKEIMG.sh
#  x86-DOS
#
#  Created by Raphael Wise on 5/8/24.
#
if $(uname) | grep "Darwin"; then
hdiutil create -fs ExFAT -srcfolder ~/x86-DOS/img/ -volname “x86-DOS” -nospotlight -format UDRO ~/x86-DOS/x86-DOS.img
else
sudo dd if=/dev/zero of=x86-DOS.img bs=1M count=50
sudo mkfs exfat -F x86-DOS.img
sudo mkdir -p /tmp/mount_tmp/ && sudo mount -o loop,rw,sync x86-DOS.img /tmp/mount_tmp
sudo mv img/* /tmp/mount_tmp
sudo umount x86-DOS.img
sudo dd if=MBR of=x86-DOS.img bs=512 count=1 seek=0
fi

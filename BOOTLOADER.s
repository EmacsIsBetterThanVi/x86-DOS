bits 16
org 7c00
jmp start
times $$ + 3 - $ nop
BSPT: dw 18
BH: dw 2
bootDriveNumber: db 0
STACK equ 1000h
KERN equ 1000h
DISK equ 3000h
start:
    cld
    push cs
    pop ds
    mov byte [bootDriveNumber], dl
    cli
    mov ax, STACK
    move ss, ax
    xor sp,sp
    sti
LBA_to_CHS:
    push ax
    xor dx, dx
    div word [BSPT]
    inc dl
    mov cl, dl
    pop ax
    mov dx, 0
    div word [BSPT]
    mov dx, 0
    div word [BH]
    mov dh, dl
    mov ch, al
    mov dl, byte [bootDriveNumber]
    ret
die:
    jmp $
print:
    pusha
    mov ah, 0Eh
    mov bl, 000ah
.loop
    lodsb
    cmp al, 0
    je .end
    int 10h
    jmp .loop
.end
    popa
    ret
times 440 - ($ - $$)  db 0
;------------------------------------------------------------------------------
; Reserved bytes after boot loader
;------------------------------------------------------------------------------
    dw 1337h, 1337h        ; disk unique identifier
    dw 0                ; magic word

;------------------------------------------------------------------------------
; Partition entries
;------------------------------------------------------------------------------

    db 80h                        ; attributes (bootable)
        
                                ; partition first sector
    db 0                        ;     CHS: head
    db 1                        ;     CHS: high cylinder bits and sector bits
    db 0                        ;     CHS: cylinder
    
    db 07h                      ; partition type: FAT12 primary
    
                                ; partition last sector
    db 1                        ;     CHS: head
    db 18                        ;     CHS: high cylinder bits and sector bits
    db 79                        ;     CHS: cylinder
    
    dd 0                        ; first LBA sector
    dd 2880                        ; number of sectors in partition
    
    ; partition 1
    db 7Fh                        ; attributes (invalid)
    times 15 db 0                ; trash for the remainder of partition entry
    
    ; partition 2
    db 7Fh                        ; attributes (invalid)
    times 15 db 0                ; trash for the remainder of partition entry
    
    ; partition 3
    db 7Fh                        ; attributes (invalid)
    times 15 db 0                ; trash for the remainder of partition entry
 
;------------------------------------------------------------------------------
; Magic word at the end
;------------------------------------------------------------------------------
    dw 0AA55h        ; BIOS expects this signature at the end of the boot sector

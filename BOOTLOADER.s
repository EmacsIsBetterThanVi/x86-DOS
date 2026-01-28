bits 16
org 7c00h
jmp start
times $$ + 3 - $ nop
BSPT: dw 18
BHC: dw 2
bootDriveNumber: db 0
STACK equ 1000h
KERN equ 1000h
DISK equ 2000h
SECTORCOUNT: db 0
SECTOR_START: dw 0
DEVKERNEL: db "DEV"
KERNELFILE: db "KERNEL", 0
CONFIGFILE: db "BOOT.CFG", 0
start:
    cld
    push cs
    pop ds
    mov byte [bootDriveNumber], dl
    cli
    mov ax, STACK
    mov ss, ax
    xor sp,sp
    sti
    mov ax, DISK
    mov es, ax
LoadRootDirectoryFileList: ; All files we need are located here, particularly the kernel, we also need this for the kernel to run.
    push word [ROOTDIRECTORY]
    pop word [SECTOR_START]
    mov byte [SECTORCOUNT], 1
    push DISK
    pop es
    xor bx, bx
    call READSECTORS
LoadConfig:
    mov si, CONFIGFILE
    call LOCATE
    mov bx, 512
    call READSECTORS
CheckBoot:
    cmp byte [es:512], 'M'
    je MENU
    mov ah, 0h
    int 16h
    cmp ah, 0Eh
    je MENU
    cmp byte [es:512], 'D'
    je .DEV
    mov si, KERNELFILE
    jmp LoadKernel
.DEV:
    mov si, DEVKERNEL
    jmp LoadKernel
MENU:
    push es
    pop ds
    mov si, 768
.loop:
    call print
    cmp byte [ds:si], 1Bh
    je .skip
    inc si
    cmp byte [ds:si], 02
    je .PROCESS
    jmp .loop
.skip:
    inc si
    cmp byte [ds:si], 0
    jne .skip
    inc si
    jmp .loop
.PROCESS:
    mov ah, 0h
    int 16h
    sub al, 48
    xor ah, ah
    mov di, 2
    mul di
    add ax, 480
    mov bx, ax
    cmp byte [es:bx], 1
    je .end
    cmp byte [es:bx], 2
    je .set
    cmp byte [es:bx], 3
    je .load
    cmp byte [es:bx], 4
    je .togel
    jmp .loop
.set:
    inc bx
    mov bx, [es:bx]
    mov byte [es:bx], cl
    jmp .loop
.togel:
    inc bx
    mov bx, [es:bx]
    mov cl, [es:bx]
    xor cl, 255
    mov byte [es:bx], cl
.load:
    inc bx
    mov cl, [es:bx]
    jmp .loop
.end:
    inc bx
    mov cl, [es:bx]
    xor ch, ch
    mov bx, cx
LoadKernel:
    call LOCATE
    mov ax, KERN
    mov es, ax
    xor bx, bx
    call READSECTORS
    push cs
    pop ds
StartKernel:
    jmp 1000h:0000h
    ; DETERMINE THE LOCATION AND SIZE OF FILE {DS:SI} IN THE LOADED DISK INFO BLOCK
    ; DIRECTORYS ARE FORMATED AS ENTRIES SUCH AS FOLLOWS
    ; NAME: 24 bytes
    ; 0
    ; SIZE: 1 byte
    ; LBA LOCATION: 2 bytes, big endian
    ; PERMISIONS: 1 byte, bits as follows, in decending order of significance:
    ;   EXECUTABLE
    ;   WRITE
    ;   DIRECTORY
    ;   READ(IF 0 THEN THE DIRECTORY CAN NOT BE VIEWED)
    ;   LOW FOUR BITS ARE PROGRAM DEFINED
    ; EXTRA DATA: 3 bytes, program defined
LOCATE:
    push ax
    push bx
    push cx
    push dx
    push es
    push word DISK
	pop es
	push si
	xor bx, bx
	xor di, di
    jmp .loop	
.next:
	pop si
	push si
	add bx, 32
	mov di, bx
.loop:
    mov al, [ds:si]
    cmp al, 0
    je .pend
    cmp al, [es:di]
    jne .next
    inc si
    inc di
	jmp .loop
.pend:
	cmp byte [es:di], 0
	jne .next
.end:
    pop si
    mov di, bx
    add di, 25
    mov al, [es:di]
    mov byte [SECTORCOUNT], al
    and si, 0FFh	
    inc di
    mov al, byte [es:di]
    inc di
    mov ah, byte [es:di]
    mov word [SECTOR_START], ax
.exit:
    pop es
    pop dx
    pop cx
    pop bx
    pop ax	
    ret
    ; READ {SECTORCOUNT} SECTORS STARTING AT {SECTOR_START} INTO {ES:BX}
READSECTORS:
    mov di, word [SECTOR_START]
.loop:
    pusha
    call LBA_to_CHS
    mov ah, 2
    mov al, 1
    int 13h
    popa
    jc .end
    add bx, 512
    inc di
    dec byte [cs:SECTORCOUNT]
    jnz .loop
    clc
.end:
    ret
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
    div word [BHC]
    mov dh, dl
    mov ch, al
    mov dl, byte [bootDriveNumber]
    ret
print:
    pusha
    mov ah, 0Eh
    mov bl, 000ah
.loop:
    lodsb
    cmp al, 0
    je .end
    int 10h
    jmp .loop
.end:
    popa
    ret
%if %eval(440 - ($ - $$)) > 0
%warning %eval(440 - ($ - $$)) bytes remaining
%elif %eval(440 - ($ - $$)) < 0
%fatal "OUT OF SPACE"
%else
%warning "no bytes remaining."
%endif
times 440 - ($ - $$)  db 0 ; MAKE IT EXACTLY 440 BYTES
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
    
    db 031h                      ; I am stealing your reserved partition entry Microsoft, deal with it.
    
                                ; partition last sector
    db 1                        ;     CHS: head
    db 18                        ;     CHS: high cylinder bits and sector bits
    db 79                        ;     CHS: cylinder
    
ROOTDIRECTORY:	dd 1                           ; first LBA sector
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

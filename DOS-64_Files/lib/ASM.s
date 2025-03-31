bits 16
db "nop"
times %eval(16 - (($ - $$) % 16)) db 0
db "00"
times %eval(16 - (($ - $$) % 16)) db 0
db "mov r8, r8"
times %eval(16 - (($ - $$) % 16)) db 0
db "88 %2*8+%1+c0"
times %eval(16 - (($ - $$) % 16)) db 0
db "mov m, r8"
times %eval(16 - (($ - $$) % 16)) db 0
db "88 %2*8+6 %1"
times %eval(16 - (($ - $$) % 16)) db 0
db "mov r8, m"
times %eval(16 - (($ - $$) % 16)) db 0
db "8a %1*8+6 %2"
times %eval(16 - (($ - $$) % 16)) db 0
db "mov r6, r6"
times %eval(16 - (($ - $$) % 16)) db 0
db "89 %2*8+%1+c0"
times %eval(16 - (($ - $$) % 16)) db 0
db "mov m, r6"
times %eval(16 - (($ - $$) % 16)) db 0
db "89 %2*8+6 %1"
times %eval(16 - (($ - $$) % 16)) db 0
db "mov r6, m"
times %eval(16 - (($ - $$) % 16)) db 0
db "8b %1*8+6 %2"
times %eval(16 - (($ - $$) % 16)) db 0
db "mov s:m, r8"
times %eval(16 - (($ - $$) % 16)) db 0
db "%s 88 %2*8+6 %1"
times %eval(16 - (($ - $$) % 16)) db 0
db "mov r8, s:m"
times %eval(16 - (($ - $$) % 16)) db 0
db "%s 8a %1*8+6 %2"
times %eval(16 - (($ - $$) % 16)) db 0
db "mov s:m, r6"
times %eval(16 - (($ - $$) % 16)) db 0
db "%s 89 %2*8+6 %1"
times %eval(16 - (($ - $$) % 16)) db 0
db "mov r6, s:m"
times %eval(16 - (($ - $$) % 16)) db 0
db "%s 8b %1*8+6 %2"
times %eval(16 - (($ - $$) % 16)) db 0
db "push r6"
times %eval(16 - (($ - $$) % 16)) db 0
db "50+%1"
times %eval(16 - (($ - $$) % 16)) db 0
db "pop r6"
times %eval(16 - (($ - $$) % 16)) db 0
db "58+%1"
times %eval(16 - (($ - $$) % 16)) db 0
db "push m"
times %eval(16 - (($ - $$) % 16)) db 0
db "ff 36 %1"
times %eval(16 - (($ - $$) % 16)) db 0
db "pop m"
times %eval(16 - (($ - $$) % 16)) db 0
db "8f 06 %1"
times %eval(16 - (($ - $$) % 16)) db 0
db "push s:m"
times %eval(16 - (($ - $$) % 16)) db 0
db "%s ff 36 %1"
times %eval(16 - (($ - $$) % 16)) db 0
db "pop s:m"
times %eval(16 - (($ - $$) % 16)) db 0
db "%s 8f 06 %1"
times %eval(16 - (($ - $$) % 16)) db 0
db "int i8"
times %eval(16 - (($ - $$) % 16)) db 0
db "cd %1"
times %eval(16 - (($ - $$) % 16)) db 0
db "db *8"
times %eval(16 - (($ - $$) % 16)) db 0
db "*(%n)"
times %eval(16 - (($ - $$) % 16)) db 0
db "dd *8"
times %eval(16 - (($ - $$) % 16)) db 0
db "*(%n 00)"
times %eval(16 - (($ - $$) % 16)) db 0
db "dd *6"
times %eval(16 - (($ - $$) % 16)) db 0
db "*(%n)"
times %eval(16 - (($ - $$) % 16)) db 0
db "call i6"
times %eval(16 - (($ - $$) % 16)) db 0
db "e8 %1"
times %eval(16 - (($ - $$) % 16)) db 0
db "call i6:i6"
times %eval(16 - (($ - $$) % 16)) db 0
db "9a %1 %2"
times %eval(16 - (($ - $$) % 16)) db 0
%warning %eval($ - $$)
%warning Remaning space: %eval(65536 - ($ - $$))
; al, cl, dl, bl, a, c, d, b
; ax, cx, dx, bx, sp, bp, si, di
; ES, CS, SS, DS; FS = 64, GS = 65
; encoding = 8*(src)+dest+c0
; abs addr encoding = 8*(src)+6
; segment encoding = seg*8+26

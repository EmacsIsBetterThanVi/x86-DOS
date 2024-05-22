; ROOT DIRECTORY ENTRIES
times 32 db 0 ; Empty for now, probobly bin?
db "KERNEL"
times %eval(25 - %strlen("KERNEL")) db 0 ; stop string
db 10h ; SIZE
db 00, 02 ; LOCATION, TEMPROARY
db 00000000b ; PERMISIONS, NONE FOR KERNEL
times 4 db 0 ; Extra Data, NONE FOR KERNEL
db "BOOT.CFG"
times %eval(25 - %strlen("BOOT.CFG")) db 0 ; stop string
db 1h ; SIZE
db 00, 12h ; LOCATION, TEMPROARY
db 01000000b ; PERMISIONS, WRITE
times 4 db 0 ; Extra Data, NONE FOR BOOT.CFG
times 512 - ($ - $$) db 0 ; ends the file, ensures it is exactly one sector

; ROOT DIRECTORY ENTRIES
times 32 db 0 ; Empty for now
db "KERNEL"
times %eval(25 - %strlen("KERNEL")) db 0 ; stop string
db 10h ; SIZE
db 00, 02 ; LOCATION, TEMPROARY
db 00000000b ; PERMISIONS, NONE FOR KERNEL
times 4 db 0 ; Extra Data, NONE FOR KERNEL
times 512 - ($ - $$) db 0

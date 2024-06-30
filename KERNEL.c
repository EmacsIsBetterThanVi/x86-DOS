/* Interupt table
INTERUPTS 20-27: memory management
20h: Share Memory cx bytes starting at ax:dx named es:bx
21h: Fetch Shared Memory Addrss named es:bx, the address is returned as ax:cx.
22h: Delete Shared Memory named es:bx(The pointer only ceases to be shared, to fully destroy the pointer use 24h).
23h: Allocate ax bytes of memory, pointer is returned as bx:cx.
24h: Free pointer ax:bx of memory.
25h: Wipe memory pointer ax:bx.(leaves the pointer, but clears the data within it)
26h: Disown a pointer.
27h: Claim a pointer(Declares a pointer to be used by the process id ax, pointer is returned as bx:cx)

INTERUPTS 28-2Bh: Proccess Control
28h: Create a new process, using the code in ax:bx.
29h: Suspend process id ax(process stops executing, but all data is saved).
2Ah: Resume process id ax, reloading all data
2Bh: Terminate process id ax(Frees all memory, and removes the process from the process table).

INTERUPTS 2Ch-35h: File System
2Ch: Open a file named ax:bx in mode cx(Read, Write, Append with any combination of B or +), file handle is returned as ax:bx.
2Dh: Close file handle ax:bx.
2Eh: Read cx bytes from file handle ax:bx into pointer es:dx
2Fh: Write cx bytes from pointer es:dx to file handle ax:bx.
30h: Seek file handle ax:bx to byte cx.
31h: Create a file named ax:bx(created with RW permisions).
32h: Delete a file named ax:bx.
33h: Change permisions of file named ax:bx to cx.
34h: Set current working directory to ax:bx.
35h: Get current working directory, pointer is returned as ax:bx.
*/
/* Interupt table(faster than going through syscall)

INTERUPTS 20-27: memory management(MEM)
20h(mshare): Share Memory cx bytes starting at ax:dx named es:bx
21h(mfetch): Fetch Shared Memory Addrss named es:bx, the address is returned as ax:cx.
22h(mdel): Delete Shared Memory named es:bx(The pointer only ceases to be shared, to fully destroy the pointer use 24h).
23h(malloc): Allocate ax bytes of memory, pointer is returned as bx:cx.
24h(free): Free pointer ax:bx of memory.
25h(mwipe): Wipe memory pointer ax:bx.(leaves the pointer, but clears the data within it)
26h(disown): Disown a pointer.
27h(claim): Claim a pointer(Declares a pointer to be used by the process id ax, pointer is returned as bx:cx)

INTERUPTS 28-2Ch: Proccess Control(PC)
28h(start): Create a new process, using the code in ax:bx.
29h(suspend): Suspend process id ax(process stops executing, but all data is saved).
2Ah(resume): Resume process id ax, reloading all data
2Bh(kill): Terminate process id ax(Frees all memory, and removes the process from the process table).
2Ch(info): Get process ax infromation, returns in bx:cx.

INTERUPTS 2Dh-40h: File System(FS)
2Dh(open): Open a file named ax:bx in mode cx(Read, Write, Append with any combination of B or +), file handle is returned as ax:bx.
2Eh(close): Close file handle ax:bx.
2Fh(read): Read cx bytes from file handle ax:bx into pointer es:dx
30h(write): Write cx bytes from pointer es:dx to file handle ax:bx.
31h(seek): Seek file handle ax:bx to byte cx.
32h(rname): Create a file named ax:bx(created with RW permisions).
33h(del): Delete a file named ax:bx.
34h(pset): Change permisions of file named ax:bx to cx.
35h(cwd): Set current working directory to ax:bx.
36h(gwd): Get current working directory, pointer is returned as ax:bx.
37h(mkdir): Create a directory named ax:bx.
38h(dirls): returns a pointer to the contents of ax:bx.

INTERUPTS 7Ah-80h: System and Drivers(SYS)
7Ah(defint): Define interupt al to address bx:cx(will not define 80h, or 7Fh).
7Bh(defsyscall): Define syscall ax:bx to address cx:dx(syscalls are used to call functions in drivers, allowing for many drivers to be loaded, and use a string for more dynamic usage(syscalls are formated as driver;func;arg1,arg2...)).
7Ch(ldriver): Request driver ax:bx, loading it if necessary, returning version information.
7Dh(udriver): Unload driver bx:cx.
7Eh(resint): Restores all system defined interupts. 
7Fh: Get System Information.
80h: Syscall ax:bx.(can call kernel interupts, with the exception of 7Fh and 80h).
*/
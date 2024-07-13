#ifdef x86_DOS
#ifndef intrupts_h
#define intrupts_h
void mshare(char * ptr, int size, char * name){
  asm("pop [SFP]\npop [RIP]\npop dx\npop ax\npop cx\npop bx\npop es\nint 20h\npush [RIP]\npush [SFP]\nret\nSFP: dw 0\nRIP: dw 0");
}
void mfetch(char * name){
  asm("pop [SFP]\npop [RIP]\npop bx\npop es\nint 21h\npush [RIP]\npush [SFP]\nret\nSFP: dw 0\nRIP: dw 0");
}
void mdel(char * name){
  asm("pop [SFP]\npop [RIP]\npop bx\npop es\nint 22h\npush [RIP]\npush [SFP]\nret\nSFP: dw 0\nRIP: dw 0");
}
#endif
#endif
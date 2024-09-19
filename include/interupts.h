#ifdef x86_DOS
#ifndef intrupts_h
#define intrupts_h
void mshare(register char * ax_dx, register int cx, register char * es_bx){
  asm("int 20h");
  return;
}
void * mfetch(register char * es_bx){
  register void * ax_bx;
  asm("int 21h");
  return ax_bx;
}
void mdel(register char * es_bx){
  asm("int 22h");
  return;
}
#endif
#endif
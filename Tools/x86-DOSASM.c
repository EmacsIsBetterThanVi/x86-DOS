//
//  x86-DOSASM.c
//  x86-DOS
//
//  Created by Raphael Wise on 5/14/24.
//
#ifdef x86_DOS
#include <SegTable.h>
#include <SSmap.h>
#include <stdbool.h>
#include <stdio.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include "arr.h"
#include "SSmap.h"
#include <stdbool.h>
#include <setjmp.h>
#endif
/*
ASM contains the following:
INSTUCTION followed by operands, the form of which it must match.
  */
#ifdef x86_DOS
SegTable ASM;
SegTable MACROS;
// TODO: Check SegTable_get() function
#define FETCH_MACRO(name) SegTable_get(name , MACROS)
#define PUT_MACRO(name, value) SegTable_add(join(name, value), MACROS)
#else
char ASM[2048][16];
char OPCODE[2048][16];
char MACRO_NAMES[256][16];
char MACRO_VALUES[256][240];
SSmap codes;
SSmap MACROS;
#define FETCH_MACRO(name) SSmap_get(name, MACROS)
#define PUT_MACRO(name, value) SSmap_put(name, value, MACROS)
#endif
RamFile BIN;
volatile void * ALLOCATED_MEMORY[32];
volatile char ALLOCATION_PTR=0;
char * Xcalloc(int size){
  ALLOCTED_MEMORY[ALLOCATION_PTR]==calloc(size, 1);
  ALLOCATION_PTR++;
  return ALLOCTED_MEMORY[ALLOCATION_PTR-1];
}
volatile char * ERRORMSG[64];
// Parses an argument to determine the type.
// Returns the type of the argument.
char * PARSE(char * CODE){
  char * tmp=Xcalloc(16, 1);
  int j=0;
  for(int i=0; CODE[i]!=0 && j<16; i++){
    if(CODE[i]=='['){
      while(CODE[i]!=']'){
        if(CODE[i]==':'){
          tmp[j]='s';
          j++;
          tmp[j]=':';
          j++;
        }
        i++;
      }
      tmp[j]='m';
    } 
    else if(CODE[i]=='$'){
      i++;
      if(CODE[i]=='e'){
        tmp[j]='r';
        r++;
        tmp[j]='2';
      } else if(CODE[i]=='r'){
        tmp[j]='r';
        r++;
        tmp[j]='4';
      } else{
        i++;
        tmp[j]='r';
        j++;
        if(CODE[i]=='h' || CODE[i]=='l'){
          tmp[j]='8';
        } else if(CODE[i]=='x'){
         tmp[j]='6';
        }
      }
    }
    else if(CODE[i]=='#'){
      tmp[j]='i';
      j++;
      i+=3;
      if(CODE[i]==' ' || CODE[i]==0){
        tmp[j]=8;
      } else {
        i+=2;
        if(CODE[i]==' ' || CODE[i]==0){
          tmp[j]=6;
        } else {
          i+=4;
          if(CODE[i]==' ' || CODE[i]==0){
            tmp[j]=2;
          } else {
            tmp[j]=4;
          }
        }
      }
    }
    else if(CODE[i]=='*'){
      if(tmp[j-2]!='*'){
        tmp[j]='*';
        j++;
        i+=3;
        if(CODE[i]==' ' || CODE[i]==0){
          tmp[j]=8;
        } else {
          i+=2;
          if(CODE[i]==' ' || CODE[i]==0){
            tmp[j]=6;
          } else {
            i+=4;
            if(CODE[i]==' ' || CODE[i]==0){
              tmp[j]=2;
            } else {
              tmp[j]=4;
            }
          }
        }
      } else{
        while(CODE[i]!=' '){
          i++;
        }
        j--;
      }
    } 
    else tmp[j]=CODE[i];
    j++;
  }
  return tmp;
}
// Calculates the final instuction
char * INJECT(char * CODE, char * FULLCODE){
  char value = 0;
  char prev = 0;
  char tmp = 0;
  char * BIN_CODE = Xcalloc(8, 1);
  for(char ptr = 0; ptr<16; ptr++){
    if(prev == 0){
      if(CODE[ptr]=='%'){
        prev='%';
      }
    } else if(prev=='%'){
      if(CODE[ptr]=='s'){
        for(int i=0; FULLCODE[i]!=':'; i++){
          if(CODE[i]=='['){
            i++;
            switch(FULLCODE[i]){
              case 'E':
                value=0x26;
                break;
              case 'C':
                value=0x2E;
                break;
              case 'S':
                value=0x36;
                break;
              case 'D':
                value=0x3E;
                break;
              default:
                asprintf(&ERRORMSG, "INVALID SEGMENTATION REGISTER: %cS IN ", FULLCODE[i]);
                longjmp(ERROR, 1);
            }
          } 
        }
      }
    }
  }
}
// Handles reading the Assembly
char * ASSEMBLE(char * ASCODE){
  char * ASMCODE;
  char * ARG;
  /* 
  STATEs:
  false - READ INSTRUCTION
  true - READ AND TRANSLATE OPERAND
  */
  bool STATE=false;
  ASMCODE=Xcalloc(16, sizeof(char));
  ARG=Xcalloc(16, sizeof(char));
  char ARGpointer=0;
  char NEXT_OP=0;
  for(char i=0; ASCODE[i]!=0; i++){
    if(STATE==0){
      if(ASCODE[i]==' '){
        STATE=true;
        Strcopy(ASMCODE, 16, ARG);
        CLEAR(ARG, 16, sizeof(char));
        NEXT_OP=ARGpointer+1;
        ARGpointer=0;
      } else if(ASCODE[i]==';' || ASCODE[i]=='\n'){
        Strcopy(ASMCODE, 16, ARG);
        break;
      } else if(ASCODE[i]==':'){
        PUT_MACRO(ARG, stoa(BIN_pointer));
        break;
      } else{
        ARG[ARGpointer]=ASCODE[i];
        ARGpointer++;
      }
    } else {
      if(ASCODE[i]==' '){
        Strcopy(ASMCODE+NEXT_OP, 16-NEXT_OP, PARSE(ARG));
        CLEAR(ARG, 16, sizeof(char));
        NEXT_OP=ARGpointer+1;
        ARGpointer=0;
      } else if(ASCODE[i]==';' || ASCODE[i]=='\n'){
        Strcopy(ASMCODE+NEXT_OP, 16-NEXT_OP, PARSE(ARG));
        break;
      } else{
        ARG[ARGpointer]=ASCODE[i];
        ARGpointer++;
      }
    }
  }
  if(ARGpointer!=0) {
    Strcopy(ASMCODE+NEXT_OP, 16-NEXT_OP, PARSE(ARG));
  }
  free(ARG);
  char * BINCODE=Xcalloc(16, sizeof(char));
  #ifdef x86_DOS
  Pair pair;
  Pair instuct1;
  Pair instuct2;
  for(int i=0; i<ASM.length; i++){
    pair=new_Pair(SegTable_get(i, &ASM), 16, 16);
    if(Strequ(pair.key, ASMCODE)){
      BINCODE=pair.value;
      break;
    }
    free(pair);
  }
  #else
  char * BINCODE= SSmap_get(ASMCODE, codes);
  #endif
  free(ASMCODE);
  return BINCODE;
}
char * ASSEMBLE_BIN(RamFile ASM, RamFile BIN){
  PUT_MACRO("ADDRESS", "0");
  char * INTERACTIONS=0;
  void * restore=malloc(16, 1);
  while (ASM.pointer<ASM.length){
    CLEAR(restore, 16, 1);
    INTERACTIONS=restore;
    for(int i=0; i<16; i++){
      if(ASM[ASM_pointer]=='\n'){
        ASM_pointer++;
        break;
      }
      INTERACTIONS[i]=ASM[ASM_pointer];
      ASM_pointer++;
    }
    if(INTERACTIONS[0]!='%'){
      INTERACRIONS=ASSEMBLE(INTERACTIONS);
      PutRamFile(BIN, INTERACTIONS);
      free(INTERACTIONS);
      PUT_MACRO("ADDRESS", stoa(BIN.pointer));
    } else if(strncmp(INTERACTIONS, "%include", 8)==0){
      
    }
  }
}
int main(int argc, char ** argv){
  jmp_buf ERROR;
  #ifdef x86_DOS
  int tmp;
  if(tmp=(ARG_SCAN(argv, argc, "-T"))!=0)){
    ASM=SegTable_init(argv[tmp+1], 65536, 32);
  } else{
    ASM=SegTable_init("C:/lib/ASM.TBL", 65536, 32);
  }
  MACROS=SegTable_init(0, 65536, 256);
  #else
  codes = SSmap_init(ASM, OPCODE, 2048);
  MACROS  = SSmap_init(MACRO_NAMES, MACRO_VALUES, 256);
  #endif
  FILE * fptr=fopen(argv[1], "r");
  if(setjmp(ERROR)){
    while(ALLOCATION_PTR!=0){
      ALLOCATION_PTR--;
      free(ALLOCTED_MEMORY[ALLOCATION_PTR]);
    }
    free(RAM_FILE);
    fclose(fptr);
    printf(ERRORMSG);
    free(ERRORMSG);
    return 127;
  } else{
    
  }
}
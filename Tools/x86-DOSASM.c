//
//  x86-DOSASM.c
//  x86-DOS
//
//  Created by Raphael Wise on 5/14/24.
//
#ifndef x86_DOS
char * insert(char CHAR, char * str, int length){
  for(int i=length; i>0; i--){
    str[i]=str[i-1];
  }
  str[0]=CHAR;
  return str;
}
char * itoa(int x, char * store, int base){
  int i=0;
  int tmp=0;
  while(x>0){
    tmp=(int)x%base;
    x-=tmp;
    x/=base;
    store=insert(tmp,store,i+1);
  }
  return store;
}
#endif
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#ifdef x86_DOS
#include <SegTable.h>
#include <SSmap.h>
#include <RamFile.h>
#else
#include "RamFile.h"
#include "arr.h"
#include "SSmap.h"
#endif
jmp_buf ERROR;
/*
ASM contains the following:
INSTUCTION followed by operands, the form of which it must match.
  */
#ifdef x86_DOS
SegTable * ASM;
SegTable * MACROS;
#define PUT_MACRO(name, value) SegTable_add(join(name, value), MACROS)
char * FETCH_MACRO(char * name){
  char * rvalue;
  Pair pair;
  for(int i=0; i<MACROS->length; i++){
    pair=new_Pair(SegTable_get(i, &MACROS), 16, 240);
    if(Strequ(pair->key, name)){
      rvalue=pair->value;
      break;
    }
    free(pair);
  }
  free(pair);
  return rvalue;
}
#else
char ASM[2048][16];
char OPCODE[2048][16];
char MACRO_NAMES[256][16];
char MACRO_VALUES[256][240];
SSmap * codes;
SSmap * MACROS;
#define FETCH_MACRO(name) SSmap_get(name, MACROS)
#define PUT_MACRO(name, value) SSmap_put(name, value, MACROS)
#endif
RamFile * ASM_FILE;
RamFile * BIN;
volatile void * ALLOCATED_MEMORY[32];
volatile char ALLOCATION_PTR=0;
volatile void * Xcalloc(int size){
  ALLOCATED_MEMORY[ALLOCATION_PTR]=calloc(size, 1);
  ALLOCATION_PTR++;
  return ALLOCATED_MEMORY[ALLOCATION_PTR-1];
}
void Xfree(void * ptr){
  free(ptr);
  bool SHIFT_BACK=false;
  for(int i=0; i<ALLOCATION_PTR; i++){
    if(SHIFT_BACK){
      ALLOCATED_MEMORY[i-1]=ALLOCATED_MEMORY[i];
    }
    if(&(ALLOCATED_MEMORY[i])==&ptr){
      free(ptr);
      SHIFT_BACK=true;
    }
  }
  if(SHIFT_BACK){
    ALLOCATION_PTR--;
    ALLOCATED_MEMORY[ALLOCATION_PTR]=NULL;
  }
}
// Parses an argument to determine the type.
// Returns the type of the argument.
char * PARSE(char * CODE){
  char * tmp=Xcalloc(16);
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
        j++;
        tmp[j]='2';
      } else if(CODE[i]=='r'){
        tmp[j]='r';
        j++;
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
  char * BIN_CODE = Xcalloc(8);
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
                printf("INVALID SEGMENTATION REGISTER: %cS IN %s\n", FULLCODE[i], FETCH_MACRO("__FILE__"));
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
  ASMCODE=Xcalloc(16);
  ARG=Xcalloc(16);
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
        PUT_MACRO(ARG, itoa(BIN->pointer));
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
  Xfree(ARG);
  #ifdef x86_DOS
  char * BINCODE=Xcalloc(16);
  Pair pair;
  for(int i=0; i<ASM_FILE->length; i++){
    pair=new_Pair(SegTable_get(i, &ASM), 16, 16);
    if(Strequ(pair->key, ASMCODE)){
      BINCODE=pair->value;
      break;
    }
    Xfree(pair);
  }
  Xfree(pair);
  #else
  char * BINCODE= SSmap_get(ASMCODE, codes);
  #endif
  Xfree(ASMCODE);
  return BINCODE;
}
char * ASSEMBLE_BIN(RamFile * ASM, RamFile * BIN){
  PUT_MACRO("ADDRESS", "0");
  char * INTERACTIONS=NULL;
  void * restore=malloc(16);
  while (ASM_FILE->pointer<ASM_FILE->length){
    CLEAR(restore, 16, 1);
    INTERACTIONS=restore;
    for(int i=0; i<16; i++){
      if(ASM_FILE->ptr[ASM_FILE->pointer]=='\n'){
        ASM_FILE->pointer++;
        break;
      }
      INTERACTIONS[i]=ASM_FILE->ptr[ASM_FILE->pointer];
      ASM_FILE->pointer++;
    }
    if(INTERACTIONS[0]!='%'){
      INTERACTIONS=ASSEMBLE(INTERACTIONS);
      PutRamFile(BIN, INTERACTIONS);
      Xfree(INTERACTIONS);
      PUT_MACRO("ADDRESS", itoa(BIN->pointer));
    } else if(strncmp(INTERACTIONS, "%include", 8)==0){ // Includes an external assembly file
      
    } else if(strncmp(INTERACTIONS, "%scan", 5)==0){ // Scans to the end of the file for labels so they don't throw an error later
      
    } else if(strncmp(INTERACTIONS, "%fixaddr", 8)==0){ // writes the current address to the bytes immidiatly behind the target label

    } else if(strncmp(INTERACTIONS, "%define", 7)==0){ // Defines a macro

    } else {
      printf("UNKNOWN ASSEMBLER INSTUCTION: %s IN %s\n", INTERACTIONS, FETCH_MACRO("__FILE__"));
    }
  }
}
int main(int argc, char ** argv){
  if(argv[1][0]=='-'){
    if(argv[1][1]=='r'){
      ASM_FILE = LoadRamFile(argv[2], 65536);
    } else if(argv[1][1]=='h'){
      printf("USAGE: %s [-rh] NAME [options]\n", argv[0]);
      printf("-h    Prints this message and exits\n-r    Use a RAM file for input\nOPTIONS:\n-R    Outputs to the provided RAM file\n-o    Outputs to the provided file\n");
      return 0;
    }else{
      printf("FIRST ARGUMENT MUST BE A FILE NAME OR -r\n");
      return 1;
    }
  }else{
    ASM_FILE = OpenRamFile(argv[1], 65536, 0);
    PUT_MACRO("__FILE__", argv[1]);
  }
  #ifdef x86_DOS
  int tmp;
  if((tmp=(ARG_SCAN(argv, argc, "-T")))!=0){
    ASM=SegTable_init(argv[tmp+1], 65536, 32);
  } else{
    ASM=SegTable_init("C:/lib/ASM.TBL", 65536, 32);
  }
  MACROS=SegTable_init(0, 65536, 256);
  #else
  codes = SSmap_init(ASM, OPCODE, 2048);
  MACROS  = SSmap_init(MACRO_NAMES, MACRO_VALUES, 256);
  #endif 
  if(setjmp(ERROR)){
    while(ALLOCATION_PTR!=0){
      ALLOCATION_PTR--;
      Xfree(ALLOCATED_MEMORY[ALLOCATION_PTR]);
    }
    CloseRamFile(BIN);
    CloseRamFile(ASM_FILE);
    return 127;
  } else{
    
  }
}

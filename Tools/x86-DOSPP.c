//
//  x86-DOSPP.c
//  x86-DOS
//
//  Created by Raphael Wise on 5/14/24.
//
#include <stdio.h>
#include <stdbool.h>
#include "arr.h"
#ifdef x86_DOS
#include <SegTable.h>
#define PUT_MACRO(name, value) SegTable_add(join(name, value), MACROS)
#else
#include "SSmap.h"
#define PUT_MACRO(name, value) SSmap_put(name, value, MACROS)
#endif
#ifdef x86_DOS
SegTable MACROS;
#else
char ** MACRO_NAMES;
char ** MACRO_VALUES;
SSmap MACROS;
#endif
char * File;
int PREPROCCES(char * fname){
  FILE * fptr=fopen(fname, "r");
  int linen=0;
  PUT_MACRO("__FILE__", fname);
  char * line=calloc(2048, sizeof(char));
  bool inFunction = false;
  while(fgets(line, 2048, fptr)!=NULL){
    linen++;
    PUT_MACRO("__LINE__", stoa(linen));
    if(line[0]=='#'){
      if(Strfind(line, "#include", ' ')){
        int START;
        if((START=Cfind(line, '"'))!=-1){
          char * tfname=line+START+1;
          char * ffname=calloc(Carrlen(tfname)-1, sizeof(char));
          Strcopy(ffname, Carrlen(tfname)-1, tfname);
        } else if((START=Cfind(line, '<'))!=-1){
          char * tfname=line+START+1;
          char * ffname=calloc(Carrlen(tfname)-1, sizeof(char));
          Strcopy(ffname, Carrlen(tfname)-1, tfname);
        }
      }
    }
  }
}
int main(int argc, char ** argv){
  #ifdef x86_DOS
  MACROS=SegTable_init(0, 65536, 256);
  #else
  MACRO_NAMES = calloc(4096, 1);
  MACRO_VALUES = calloc(61440, 1);
  MACROS = SSmap_init(MACRO_NAMES, MACRO_VALUES, 256);
  #endif
  File = calloc(65536, 1);
  PUT_MACRO("x86_DOS", "1");
  PUT_MACRO("__FILE__", argv[1]);
  PREPROCCES(argv[1]);
  return 0;
}

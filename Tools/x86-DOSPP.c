//
//  x86-DOSPP.c
//  x86-DOS
//
//  Created by Raphael Wise on 5/14/24.
//
#ifdef x86_DOS
#include <SegTable.h>
#include <stdio.h>
#define PUT_MACRO(name, value) SegTable_add(join(name, value), MACROS)
#else
#include <stdio.h>
#include <stdlib.h>
#include "arr.h"
#include "SSmap.h"
#define PUT_MACRO(name, value) SSmap_put(name, value, MACROS)
#endif
#ifdef x86_DOS
SegTable MACROS;
#else
char MACRO_NAMES[256][16];
char MACRO_VALUES[256][240];
SSmap MACROS;
#endif
char * FILE;
int PREPROCCES(char * fname){
  FILE * fptr=fopen(fname, "r");
  int linen=0;
  PUT_MACRO(MACROS, "__FILE__", argv[1]));
  char * line=calloc(2048, sizeof(char));
  bool inFunction false;
  while(fgets(line, 2048, fptr)!=NULL){
    linen++;
    PUT_MACRO(MACROS, "__LINE__", stoa(linen));
    if(line[0]=='#'){
      if(Strfind(line, "#include", ' ')){
        int START;
        if((START=Cfind(line, '"'))!=-1){
          char * tfname=line+START+1;
          char * ffname=calloc(strlen(tfname)-1, sizeof(char));
          Strcopy(ffname, strlen(tfname)-1, tfname);
        } else if((START=Cfind(line, '<'))!=-1){
          char * tfname=line+START+1;
          char * ffname=calloc(strlen(tfname)-1, sizeof(char));
          Strcopy(ffname, strlen(tfname)-1, tfname);
        }
      }
    }
  }
}
int main(int argc, char ** argv){
  #ifdef x86_DOS
  MACROS=SegTable_init(0, 65536, 256);
  #else
  MACROS = SSmap_init(MACRO_NAMES, MACRO_VALUES, 256);
  #endif
  FILE = calloc(65536, sizeof(char));
  PUT_MACRO(MACROS, "x86_DOS", "1");
  PUT_MACRO(MACROS, "__FILE__", argv[1]));
  PRECPROCCES(argv[1]);
  return 0;
}
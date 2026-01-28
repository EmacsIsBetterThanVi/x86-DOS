//
//  x86-DOSPP.c
//  x86-DOS
//
//  Created by Raphael Wise on 5/14/24.
//
#include <stdio.h>
#include <stdbool.h>
#include "../include/arr.h"
#ifdef x86_DOS
#include <SegTable.h>
#define PUT_MACRO(name, value) SegTable_add(join(name, value), MACROS)
#else
#include "../include/SSmap.h"
#define PUT_MACRO(name, value) SSmap_put(name, value, MACROS)
#define PUT_MACROI(name, value) SSmap_puti(name, value, MACROS)
#endif
#ifdef x86_DOS
SegTable MACROS;
#else
char ** MACRO_NAMES;
char ** MACRO_VALUES;
SSmap * MACROS;
#endif
char * File;
int PREPROCESS(char * fname){
  FILE * fptr=fopen(fname, "r");
  int linen=0;
  PUT_MACRO("__FILE__", fname);
  char * line=calloc(2048, sizeof(char));
  bool inFunction = false;
  while(fgets(line, 2048, fptr)!=NULL){
    linen++;
    PUT_MACROI("__LINE__", &linen);
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
      } else if (Strfind(line, "#define", ' ')) {
        int START = Cfind(line, ' ');
        char *TMP = line + START + 1;
        START = Cfind(TMP, ' ');
	line[START]=0;
        char *KEY = calloc(Carrlen(TMP) + 1, sizeof(char));
        Strcopy(KEY, Carrlen(TMP), TMP);
	TMP = TMP+START+1;
        char *VALUE = calloc(Carrlen(TMP) + 1, sizeof(char));
	Strcopy(VALUE, Carrlen(TMP), TMP);
        PUT_MACRO(KEY, VALUE);
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
  PREPROCESS(argv[1]);
  return 0;
}

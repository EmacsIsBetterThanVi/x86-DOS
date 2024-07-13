#ifdef x86_DOS
#ifndef SegTable_H
#include <stdlib.h>
#include <arr.h>
#include <utils.h>
typedef struct SegTable {
  char * ptr;
  Dimensions dim;
  int length;
} SegTable;
SegTable SegTable_init(char * fname, int size, int elementSize){
  SegTable s;
  if(size%elementSize==0){
    if(fname!=0){
      s.ptr=fopen(fname, "r");
      FILE * fptr=fopen(fname, "rb");
      char * ptr = (char*)malloc(size);
      s.length=fread(ptr, 1, size, fptr)/elementSize;
      fclose(fptr);
    }
    s.ptr=ptr;
    s.dim=Dimension(size/elementSize, elementSize);
  }
  return s;
}
void SegTable_free(char * ptr){
  free(ptr);
}
int SegTable_add(char * ptr, SegTable table){
  int i=0;
  if(table.length >= table.dim.hight) return 0;
  while(ptr[i]!=0 && i<table.dim.width){
    table.ptr[(table.length*table.dim.width)+i]=ptr[i];
    i++;
  }
  table.length++;
  return i;
}
int SegTable_put(char * ptr, SegTable table, int index){
  int i=0;
  if(index >= table.length) return 0;
  while(ptr[i]!=0 && i<table.dim.width){
    table.ptr[(index*table.dim.width)+i]=ptr[i];
    i++;
  }
  return i;
}
char * SegTable_get(int index, SegTable table){
  char * ptr=calloc(table.dim.width, sizeof(char));
  for(int i=0; i<table.dim.width; i++){
    ptr[i]=table.ptr[(index*table.dim.width)+i];
  }
  return ptr;
}
#define SegTable_H
#endif
#endif
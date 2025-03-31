#ifndef RamFile_H
#define RamFile_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef x86_DOS
#include <sys/mman.h>
#include <fcntl.h>
#endif
typedef struct RamFile {
  char * ptr;
  int length;
  int pointer;
} RamFile;
#define RamFile_END -1
#define RamFile_START 0
RamFile * OpenRamFile(char * fname, int size, int spointer){
  RamFile * rf=(RamFile *)malloc(sizeof(RamFile));
  char * ptr;
  if(fname!=0){
    FILE * fptr=fopen(fname, "rb");
    ptr = (char*)calloc(size, 1);
    rf->length=fread(ptr, 1, size, fptr);
    fclose(fptr);
  } else{
    ptr = (char*)calloc(size, 1);
    rf->length=0;
  }
  if(spointer!=-1) {
    rf->pointer=spointer;
  }
  rf->ptr=ptr;
  return rf;
}
void CloseRamFile(RamFile * rf){
  free(rf->ptr);
}
void WriteRamFile(RamFile * rf, char * fname){
  FILE * fptr=fopen(fname, "wb");
  fwrite(rf->ptr, 1, rf->length, fptr);
  fclose(fptr);
}
#ifdef x86_DOS
void StoreRamFile(RamFile * rf, char * fname){
  mshare(rf->ptr, rf->length, fname);
}
RamFile * LoadRamFile(char * fname, int size){
  RamFile * rf;
  rf->ptr=mfetch(fname);
  rf->length=size;
  rf->pointer=0;
  return rf;
}
#else
void StoreRamFile(RamFile * rf, char * fname){
  void * ramfile = mmap(NULL, rf->length, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, open(fname, O_RDWR), 0);
  memcpy(ramfile, rf->ptr, rf->length);
}
RamFile * LoadRamFile(char * fname, int size){
  RamFile * rf=(RamFile *)malloc(sizeof(RamFile));
  rf->ptr=(char *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, open(fname, O_RDWR), 0);
  rf->length=size;
  rf->pointer=0;
  return rf;
}
#endif
int PutRamFile(RamFile * rf, char * data){
  int i=0;
  while(data[i]!=0){
    rf->ptr[rf->pointer]=data[i];
    rf->pointer++;
    i++;
  }
  if(rf->pointer>rf->length){
    rf->length=rf->pointer;
  }
  return i;
}
char * GetRamFile(RamFile * rf, int size){
  char * tmp=(char *)calloc(size, sizeof(char));
  for(int i=0; i<size; i++){
    tmp[i]=rf->ptr[rf->pointer];
    rf->pointer++;
  }
  return tmp;
}
void SeekRamFile(RamFile * rf, int offset){
  rf->pointer = offset;
}
#endif

#ifndef RamFile_H
#define RamFile_H
#include <stdlib.h>
typedef struct RamFile {
  char * ptr;
  int length;
  int pointer
} RamFile;
#define RamFile_END -1
#define RamFile_START 0
RamFile OpenRamFile(char * fname, int size, int spointer){
  RamFile rf;
  if(fname!=0){
    FILE * fptr=fopen(fname, "rb");
    char * ptr = (char*)calloc(size);
    rf.length=fread(ptr, 1, size, fptr);
    fclose(fptr);
  } else{
    char * ptr = (char*)calloc(size);
    rf.length=0;
  }
  if(spointer!=-1) {
    rf.pointer=spointer;
  }
  rf.ptr=ptr;
  return rf;
}
void CloseRamFile(RamFile rf){
  free(rf.ptr);
}
void WriteRamFile(RamFile rf, char * fname){
  FILE * fptr=fopen(fname, "wb");
  fwrite(rf.ptr, 1, rf.length, fptr);
  fclose(fptr);
}
void StoreRamFile(RamFile rf, char * fname){
  mshare(rf.ptr, rf.length, fname);
}
RamFile LoadedRamFile(char * fname){
  mfetch(fname);
}
int PutRamFile(RamFile rf, char * data){
  int i=0;
  while(data[i]!=0){
    rf.ptr[rf.pointer]=data[i];
    rf.pointer++;
    i++;
  }
  if(rf.pointer>rf.length){
    rf.length=rf.pointer;
  }
}
char * GetRamFile(RamFile rf, int size){
  char * tmp=calloc(size, sizeof(char));
  for(int i=0; i<size; i++){
    tmp[i]=rf.ptr[rf.pointer];
    rf.pointer++;
  }
  return tmp;
}
void SeekRamFile(RamFile rf, int offset){
  rf.pointer = offset
}
#endif
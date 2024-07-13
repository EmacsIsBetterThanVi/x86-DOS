#ifndef RamFile_H
#define RamFile_H
#include <stdlib.h>
typedef struct RamFile {
  char * ptr;
  int length;
} RamFile;
RamFile OpenRamFile(char * fname, int size){
  RamFile rf;
  FILE * fptr=fopen(fname, "rb");
  char * ptr = (char*)malloc(size);
  rf.length=fread(ptr, 1, size, fptr);
  fclose(fptr);
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
#endif
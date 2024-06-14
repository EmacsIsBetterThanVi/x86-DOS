#ifndef ARR_H
#include <stdlib.h>
int Strequ(char * arg1, char * arg2){
  int i=0;
  while(arg1!=0 && arg2!=0){
    if(arg1[i]!=arg2[i]) return 0;
  }
  if(arg1[i]==0 && arg2[i]==0) return 1;
  return 0;
}
int Carrlen(char * arr){
  int i=0;
  while(arr[i]!=0){
    i++;
  }
  return i;
}
// returns the index or -1 on failure
int Sarrfind(char ** arr, char * str){
  int i=0;
  while(arr[i]!=0){
    if(Strequ(arr[i], str)) return i;
    i++;
  }
  return -1
}
// Appends str to arr
void Sarradd(char ** arr, char * str){
  int x=Carrlen(arr);
  char ** arr2=calloc(sizeof(char*)*(x+1));
  for(int i=0; i<x; i++){
    arr2[i]=arr[i];
  }
  arr2[x]=str;
  free(arr);
  arr=arr2;
  return;
}
#define ARR_H
#endif
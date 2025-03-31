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
int Cfind(char * str, char c){
  int i=0;
  while(str[i]!=0){
    if(str[i]==c) return i;
  }
  return -1;
}
void Strcopy(char * str1, int size, char * str2){
  int i=0;
  while(str2[i]!=0 && i<size){
    str1[i]=str2[i];
    i++;
  }
  str1[size-1]=0;
}
int Carrlen(char * arr){
  int i=0;
  while(arr[i]!=0){
    i++;
  }
  return i;
}
int Strfind(char * str1, char * str2, char split){
  int i=0;
  int x=0;
  while(str1[i]!=0 && str2[i]!=0){
    if(str1[i]==split && str2[i]==split){
      x=i;
      i++;
      while(str1[i]==str2[i]){
        if(str1[i]==split && str1[i]==split){
          return x;
        }
        if(str1[i]==0 && str1[i]==0){
          return x;
        }
        i++;
      }
      i++;
    }
  }
  return -1;
}
// returns the index or -1 on failure
int Sarrfind(char ** arr, char * str){
  int i=0;
  while(arr[i]!=0){
    if(Strequ(arr[i], str)) return i;
    i++;
  }
  return -1;
}
int Sarrlen(char ** arr){
    int i=0;
    while(arr[i][0]!=0){
        i++;
    }
    return i;
}
// Appends str to arr
void Sarradd(char ** arr, char * str){
  int x=Sarrlen(arr);
  char ** arr2=calloc(sizeof(char*)*(x+1), 1);
  for(int i=0; i<x; i++){
    arr2[i]=arr[i];
  }
  arr2[x]=str;
  free(arr);
  arr=arr2;
  return;
}
// Clears the contents of an array, one byte at a time
void CLEAR(void * ptr, int size, int element_size){
  for(int i=0; i<size*element_size; i++){
    ((char *)ptr)[i]=0;
  }
  return;
}
#define ARR_H
#endif

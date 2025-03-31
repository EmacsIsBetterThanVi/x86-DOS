#ifndef Utils_H
#include <stdlib.h>
#include <stdbool.h>
#define Utils_H
typedef struct Dimensions{
  int width;
  int hight;
} Dimensions;
typedef struct Pair{
  char * key;
  char * value;
} Pair;
Dimensions * Dimension(int hight, int width){
  Dimensions * d = (Dimensions *)malloc(sizeof(Dimensions));
  d->width=width;
  d->hight=hight;
  return d;
};
Pair * new_Pair(char * str, int keysize, int fullsize){
  char * key=str;
  char * value=str+keysize;
  Pair * p=(Pair *)malloc(sizeof(Pair));
  p->key=key;
  p->value=value;
  return p;
}
char * join(char * str1, char * str2){
  char * str=calloc(strlen(str1)+strlen(str2)-1, sizeof(char));
  int i=0;
  while(str1[i]!=0){
    str[i]=str1[i];
    i++;
  }
  while(str2[i]!=0){
    str[i]=str2[i-strlen(str1)];
    i++;
  }
  return str;
}
Pair * Split(char delim, char * str, int maxsize){
  int i=0;
  int k=0;
  Pair * p=(Pair *)malloc(sizeof(Pair));
  char * key=(char *)calloc(maxsize, sizeof(char));
  char * value=(char *)calloc(maxsize, sizeof(char));
  while(str[k]!=delim){
    key[i]=str[k];
    i++;
    k++;
  }
  i=0;
  while(str[k]!=0){
    value[i]=str[k];
    k++;
    i++;
  }
  p->key=key;
  p->value=value;
  return p;
}
char * stoa(short num){
  char * str=(char *)calloc(6, sizeof(char));
  short i=0;
  while(num!=0){
    str[i]=(num%10)+48;
    num/=10;
    i++;
  }
}
#ifdef ARR_H
// Scans argv for a given argument, returns the index of the argument, or 0 on failure
int ARG_SCAN(char ** argv, int argc, char * target){
  for(i=1; i<argc; i++){
    if(Strequ(argv[i], target)) return i;
  }
  return 0
}
#endif
#endif

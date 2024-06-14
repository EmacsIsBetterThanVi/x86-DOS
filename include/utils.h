#ifndef Utils_H
#include <stdlib.h>
#define Utils_H
typedef struct Dimensions{
  int width;
  int hight;
} Dimensions;
typedef struct Pair{
  char * key;
  char * value;
} Pair;
Dimensions Dimension(int hight, int width){
  Dimensions d;
  d.width=width;
  d.hight=hight;
  return d;
};
Pair new_Pair(char * str, int keysize, int fullsize){
  char * key=str
  char * value=str+keysize;
  Pair p;
  p.key=key;
  p.value=value;
  return p;
}
Pair Split(char delim, char * str, int maxsize){
  int i=0;
  int k=0;
  Pair p;
  char * key=calloc(maxsize, sizeof(char));
  char * value=calloc(maxsize, sizeof(char));
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
  p.key=key;
  p.value=value;
  return p;
}
#endif
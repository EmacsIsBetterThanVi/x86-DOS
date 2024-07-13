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
bool eq(char * str1, char * str2, char end){
  int i=0;
  while(str1[i]==str2[i]){
    if(str1[i]==end){
      return true;
    }
    i++;
  }
  return false;
}
int eqpos(char * str1, char * str2, char split){
  int i=0;
  int x=0;
  while(str1[i]!=0 && str2[i]!=0){
    if(str1[i]==split && str2[i]==split){
      x=i;
      i++;
      while(str1[i]==str1[i]){
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
#endif
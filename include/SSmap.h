#ifndef SSmap_H
#ifdef x86_DOS
#include <arr.h>
#else
#include "arr.h"
#endif
typedef struct SSmap {
  char ** keys;
  char ** values;
  int count;
  int maxlength;
} SSmap;
int SSmap_count(SSmap * map){
  if(map->count==0){
    int i=0;
    while(map->keys[i]!=0){
      if(map->count==map->maxlength){
        map->count=map->maxlength;
        return map->count;
      }
      i++;
    }
    map->count=i;
  }
  return map->count;
}
SSmap * SSmap_init(char ** names, char ** values, int length){
  SSmap * m = (SSmap *)malloc(sizeof(SSmap));
  m->keys=names;
  m->values=values;
  m->maxlength=length;
  SSmap_count(m);
  return m;
}
char * SSmap_get(char * key, SSmap * map){
  int x=Sarrfind(map->keys, key);
  if(x==-1) return 0;
  return map->values[x];
}
void SSmap_put(char * key, char * value, SSmap * map){
  int x=Sarrfind(map->keys, key);
  if(x!=-1){
    map->values[x]=value;
  } else {
    if(map->maxlength==0){
      Sarradd(map->keys, key);
    } else{
      if(map->count!=map->maxlength){
        map->keys[map->count]=key;
        map->values[map->count]=value;
        map->count++;
      } 
    } 
  } 
} 

#define SSmap_H
#endif

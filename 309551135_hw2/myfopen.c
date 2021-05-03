#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<dlfcn.h>
#include<sys/stat.h>
FILE* (*old_fopen)(const char* path, const char* mode) = NULL;
FILE* fopen(const char* path, const char* mode){
  FILE* ret;
  if(old_fopen == NULL){
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_fopen = dlsym(handle, "fopen");
  }
  if(old_fopen != NULL) 
    ret = old_fopen(path, mode);
  printf("[logger] fopen(\"%s\", \"%s\") = %p\n", path, mode, ret);
  return ret;
}

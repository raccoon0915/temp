#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<dlfcn.h>
#include<sys/stat.h>
static int (*old_creat)(const char* path, mode_t mode) = NULL;
int creat(const char* path, mode_t mode){
  ssize_t size = 0;
  int oct_mode = mode & (S_IRWXU | S_IRWXG | S_IRWXO);
  int ret = 0;
  if(old_creat == NULL){
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_creat = dlsym(handle, "creat");
  }
  if(old_creat != NULL) 
    ret = old_creat(path, mode);
  printf("[logger] creat(\"%s\", %o) = %d\n", path, oct_mode, ret);
  return ret;
}

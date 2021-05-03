#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<dlfcn.h>
#include<sys/stat.h>
static int (*old_chmod)(const char* path, mode_t mode) = NULL;
int chmod(const char* path, mode_t mode){
  ssize_t size = 0;
  int oct_mode = mode & (S_IRWXU | S_IRWXG | S_IRWXO);
  int ret = 0;
  if(old_chmod == NULL){
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_chmod = dlsym(handle, "chmod");
  }
  if(old_chmod != NULL) 
    ret = old_chmod(path, mode);
  printf("[logger] chmod(\"%s\", %o) = %d\n", path, oct_mode, ret);
  return ret;
}

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<dlfcn.h>
static int (*old_close)(int fd) = NULL;
int close(int fd){
  char procpath[200];
  char filepath[200];
  ssize_t size = 0;
  int ret = 0;
  if(old_close == NULL){
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_close = dlsym(handle, "close");
  }
  if(fd < 0) perror("");
  snprintf(procpath, 200, "/proc/self/fd/%d", fd);
  size = readlink(procpath, filepath, 200);
  if(size < 0) perror("");
  filepath[size] = '\0';
  if(old_close != NULL) 
    ret = old_close(fd);
  printf("[logger] close(\"%s\") = %d\n", filepath, ret);
  return ret;
}

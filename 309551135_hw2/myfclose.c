#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<dlfcn.h>
static int (*old_fclose)(FILE* fp) = NULL;
int fclose(FILE* fp){
  char procpath[200];
  char filepath[200];
  ssize_t size = 0;
  int ret = 0;
  int fd = fileno(fp);
  if(old_fclose == NULL){
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_fclose = dlsym(handle, "fclose");
  }
  if(fd < 0) perror("");
  snprintf(procpath, 200, "/proc/self/fd/%d", fd);
  size = readlink(procpath, filepath, 200);
  if(size < 0) perror("");
  filepath[size] = '\0';
  if(old_fclose != NULL) 
    ret = old_fclose(fp);
  printf("[logger] fclose(\"%s\") = %d\n", filepath, ret);
  return ret;
}

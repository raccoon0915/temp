#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<dlfcn.h>
#include<ctype.h>
static size_t (*old_fread)(void* ptr, size_t size, size_t nmemb, FILE* stream) = NULL;
size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream){
  char procpath[200];
  char filepath[200];
  ssize_t n = 0;
  size_t ret;
  int fd = fileno(stream);
  if(old_fread == NULL){
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_fread = dlsym(handle, "fread");
  }
  if(fd < 0) perror("");
  snprintf(procpath, 200, "/proc/self/fd/%d", fd);
  n = readlink(procpath, filepath, 200);
  if(n < 0) perror("");
  filepath[n] = '\0';
  if(old_fread != NULL) 
    ret = old_fread(ptr, size, nmemb, stream);
  /*-------------isprint--------------*/
  char* temp = (char*)ptr;
  char output[200];
  for(int i=0; i<nmemb*size; i++){
    if(isprint(temp[i]))
      output[i] = temp[i];
    else
      output[i] = '.';
  }
  output[nmemb*size] = '\0';
  printf("[logger] fread(\"%s\", %lu, %lu, \"%s\") = %lu\n", output, size, nmemb, filepath, ret);
  return ret;
}

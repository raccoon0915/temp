#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<dlfcn.h>
#include<sys/stat.h>
static int (*old_chown)(const char* path, uid_t owner, gid_t group) = NULL;
int chown(const char* path, uid_t owner, gid_t group){
  int ret = 0;
  if(old_chown == NULL){
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_chown = dlsym(handle, "chown");
  }
  if(old_chown != NULL) 
    ret = old_chown(path, owner, group);
  printf("[logger] chown(\"%s\", %d, %d) = %d\n", path, owner, group, ret);
  return ret;
}

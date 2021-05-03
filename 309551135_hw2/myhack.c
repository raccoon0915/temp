#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<stdlib.h>
int chmod(const char* path, mode_t mode){
  printf("hello chmod\n");
  return 1;
}

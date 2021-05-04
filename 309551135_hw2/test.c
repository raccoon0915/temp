#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<stdlib.h>
int main(int argc, char** argv){
/*--------------------------close-----------------------------*/
/*  int fd = open("test.txt", O_RDONLY | O_CREAT);
  close(fd);*/
/*--------------------------chown-----------------------------*/
/*  chown("test.txt", 123, 456);*/
/*--------------------------creat-----------------------------*/
/*  creat("test.txt", 0661);*/
/*--------------------------fopen-----------------------------*/
/*  FILE* fp;
  fp = fopen("test.txt", "w");
  fclose(fp);  */
/*--------------------------fread-----------------------------*/
  char* buffer = malloc(sizeof(char)*10);
  size_t ret = 0;
  FILE* fp = fopen("test.txt", "r");
  ret = fread(buffer, 3, 3, fp);
  printf("fread output is %s, return is %lu\n", buffer, ret);
/*--------------------------*/  
  return 0;
}

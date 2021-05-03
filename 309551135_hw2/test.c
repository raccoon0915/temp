#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
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
  char buffer[100];
  FILE* fp = fopen("test.txt", "r");
  fread(buffer, 100, 1, fp);
  printf("fread output = %s", buffer);
  fclose(fp);
  return 0;
}

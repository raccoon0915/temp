/*raccoon:
1. Remember to close fp(if fp!= NULL)
2. Remember to clear temp*/
#include<iostream>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<filesystem>
#include<dirent.h>
#include<cstring>
#include<string>
#include<pwd.h>
#include<unistd.h>
#include<errno.h>
#include<sstream>
#include<fstream>
using namespace std;
char path[100], temp[1024];/*raccoon:print buffer*/
const char *FD[3] = {"cwd", "root", "exe"};
void f_type(char* path, FILE* fp){
  struct stat info;
  stat(path, &info);
  if(fp == NULL)cout << "TYPE=unknown ";
    else{
      switch(info.st_mode & S_IFMT){
	case S_IFDIR:
	  cout << "TYPE=DIR ";
	  break;
	case S_IFSOCK:
	  cout << "TYPE=SOCK ";
          break;
	case S_IFREG:
	  cout << "TYPE=REG ";
	  break;
	case S_IFCHR:
	  cout << "TYPE=CHR ";
          break;
	case S_IFIFO:
	  cout << "TYPE=FIFO ";
	  break;
	default:
	  cout << "TYPE=unknown ";
	  break;
	}
    }
}
void prefix(long pid){
/*--------------------COMMAND------------------------*/
  FILE* fp;
  sprintf(path, "/proc/%ld/comm", pid);
  fp = fopen(path, "r");
  if(fp != NULL){
  fscanf(fp, "%s", temp);
  cout << "COMMAND=" << temp << " ";
  }	
  memset(temp, 0, sizeof(temp));/*raccoon:clear temp*/
  if(fp != NULL)fclose(fp);
/*----------------------PID--------------------------*/
  cout << "PID=" << pid << " ";
/*---------------------USER--------------------------*/
  sprintf(path, "/proc/%ld/loginuid", pid);/*raccoon:warning!this path is wrong*/
  fp = fopen(path, "r");
  if(fp != NULL){
    fscanf(fp, "%s", temp);
    long uid = stol(temp);
    if(uid == 4294967295)uid=0;/*raccoon:uid=-1 means yet to set*/
    struct passwd* pw = getpwuid(uid);
    if(pw != NULL)
      cout << "USER=" << pw->pw_name <<" ";
    else
      cout << "USER=unknown ";
  }
  memset(temp, 0, sizeof(temp));/*raccoon:clear temp*/
  if(fp != NULL)fclose(fp);
}
void f_mode(long pid, long fd){
}
int main(int argc, char**argv){
	long tgid;
	bool done = false;/*raccoon:done for check whether print finish*/
	struct dirent* ent;
	FILE* fp;
	DIR* proc = opendir("/proc");
	if(proc == NULL)
		perror("");
	while(ent = readdir(proc)){
		/*--------------------PID------------------------*/
		if(!isdigit(*ent->d_name))
			continue;
		tgid = strtol(ent->d_name, NULL, 10);
		/*---------------------TYPE--------------------------*/ 
		for(int i=0; i<3; i++){
		  prefix(tgid);
		/*---------------root, cwd, exe FD-------------------*/
//			else if(FD[i] == "fd")cout << "FD=NOFD ";
	          cout << "FD=" << FD[i] << " ";
		/*------------------------TYPE-----------------------*/
	          sprintf(path, "/proc/%ld/%s", tgid, FD[i]);
		  fp = fopen(path, "r");
		  f_type(path, fp);
		/*------------------------NODE-----------------------*/
		  struct stat info;
		  stat(path, &info);
	  	  if(fp == NULL)cout << "NODE=" << " ";
		  else cout << "NODE=" << info.st_ino << " ";
		/*-----------------------NAME------------------------*/
		  if(errno == 13)/*raccoon:13=permission denied*/
				cout << path << " (readlink: Permission denied)" << endl;
//		  else if(errno == 2)/*raccoon:2=no such file*/
//			cout << path << endl;
		  else{
		  	readlink(path, temp, sizeof(temp));
			cout << "NAME="  << temp << endl;
		  }
		  if(fp != NULL)fclose(fp);
		}
		/*----------------------mem FD-----------------------*/
		sprintf(path, "/proc/%ld/maps", tgid);
		fp = fopen(path, "r");
		if(fp != NULL){
		  string a, b, c, d, e, f;
  	 	  string line;
  		  ifstream infile(path);
  	 	  /*raccoon:temp_s for check the duplicate*/
  		  string temp_s;
  		  while(getline(infile, line)){
  		    stringstream ss;
   		    ss << line;
   		    ss >> a >> b >> c >> d >> e;
    		  /*raccoon:Cannot handle final delete.*/
    		  if(e != "0" && e != temp_s){
    		    prefix(tgid);
      		    cout << "FD=mem ";
		    f_type(path, fp);
      		    cout << "NODE=" << e << " ";
      		    ss >> f;
     		    cout << "NAME=" << f << endl;
   		  }
    		  /*raccoon:Maybe wrong here.*/
    		  temp_s = e;
  		  }
  		}
  		if(fp != NULL)fclose(fp);
  		/*----------------------fd FD-----------------------*/
  		sprintf(path, "/proc/%ld/fd", tgid);
  		fp = fopen(path, "r");
  		if(errno == 13){
  		  prefix(tgid);
  		  cout << "FD=NOFD" << " ";
  		  cout << "TYPE=" << " ";
  		  cout << "NODE=" << " ";
  		  /*raccoon:Prefix will change path.*/
  		  sprintf(path, "/proc/%ld/fd", tgid);
  		  cout << path << " (opendir: Permission denied)" << endl;
  		}
  		else{
  		  long fd;
  		  struct stat info;
  		  struct dirent* fd_ent;
  		  DIR* fdptr = opendir(path);
  		  while(fd_ent = readdir(fdptr)){
		    fd = strtol(fd_ent->d_name, NULL, 10);
  		    sprintf(path, "/proc/%ld/fd/%ld", tgid, fd);
  		    stat(path, &info);
  		  }
  		}
	}
	return 0;	
}

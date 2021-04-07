/*raccoon:
1. Remember to close fp(if fp!= NULL)
2. Remember to clear temp
3. Remember to check whether path is right*/
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
#include<iomanip>
using namespace std;
char path[100], temp[1024];/*raccoon:print buffer*/
const char *FD[3] = {"cwd", "root", "exe"};

void f_type(char* path, FILE* fp);
void prefix(long pid);
char f_mode(long pid, long fd);

int main(int argc, char**argv){
	long tgid;
	struct dirent* ent;
	FILE* fp;
	DIR* proc = opendir("/proc");
	if(proc == NULL)perror("");
	cout << "COMMAND"
	     << setw(25) << "PID"
	     << setw(11) << "USER"
	     << setw(14) << "FD"
	     << setw(10) << "TYPE"
	     << setw(9)  << "NODE"
	     << setw(12) << "NAME" << endl;
	while(ent = readdir(proc)){
		/*--------------------PID------------------------*/
		if(!isdigit(*ent->d_name))
			continue;
		tgid = strtol(ent->d_name, NULL, 10);
		for(int i=0; i<3; i++){
		/*---------------root, cwd, exe FD-------------------*/
		  prefix(tgid);
	          cout << setw(14) <<FD[i];/*raccoon:FD*/
		/*------------------------TYPE-----------------------*/
	          sprintf(path, "/proc/%ld/%s", tgid, FD[i]);
		  fp = fopen(path, "r");
		  f_type(path, fp);
		/*------------------------NODE-----------------------*/
		  struct stat info;
		  stat(path, &info);
	  	  if(fp == NULL)cout << setw(9) << " ";/*raccoon:empty node*/
		  else cout << setw(9) << info.st_ino;/*raccoon:node*/
		/*-----------------------NAME------------------------*/
		  if(errno == 13)/*raccoon:13=permission denied*/
				cout << setw(12) << path << " (readlink: Permission denied)" << endl;
//		  else if(errno == 2)/*raccoon:2=no such file*/
//			cout << path << endl;
		  else{
		  	readlink(path, temp, sizeof(temp));
		  	string str = temp;
			cout << setw(8+str.length()) << temp << endl;/*raccoon:name*/
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
    		    if(line.find("(deleted)") != -1){
    		      cout << setw(14) <<"del";/*raccoon:FD*/
    		      cout << setw(10) <<"unknown";/*raccoon:TYPE*/
    		    }
      		    else{ 
      		      cout << setw(14) <<"mem";/*raccoon:FD*/
		      f_type(path, fp);
		    }
      		    cout << setw(9) <<e;/*raccoon:node*/
      		    if(line.find("(deleted)") != -1){
      		      ss >> f;
      		      f.append(" (deleted)");
     		      cout << setw(8+f.length()) << f << endl;/*raccoon:name*/
     		    }
     		    else{
     		      ss >> f;
     		      cout << setw(8+f.length()) << f << endl;/*raccoon:name*/
     		    }
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
  		  /*raccoon:empty NOFD, TYPE, node*/
  		  cout << setw(14) <<"NOFD";
  		  /*raccoon:Prefix will change path.*/
  		  sprintf(path, "/proc/%ld/fd", tgid);
  		  cout << setw(12) <<path << " (opendir: Permission denied)" << endl;
  		}
  		else{
  		  int check = 0;
  		  long fd;
  		  char permission;
  		  string fd_name;
  		  string checkDel;/*raccoon:for fd to check deleted*/
  		  struct dirent* fd_ent;
  		  struct stat info;
  		  sprintf(path, "/proc/%ld/fdinfo", tgid);
  		  DIR* fdptr = opendir(path);
  		  while(fd_ent = readdir(fdptr)){
  		    /*raccoon:check < 2 to skip . and .. directory*/
  		    if(check < 2){check ++;continue;}
		    fd = strtol(fd_ent->d_name, NULL, 10);
 		    permission = f_mode(tgid, fd);/*raccoon:get fd#*/
  		    fd_name = fd_ent->d_name;
  		    fd_name.push_back(permission);
  		    /*---------------root, cwd, exe FD-------------------*/
  		    prefix(tgid);
  		    /*raccoon:FD*/
  		    cout << setw(14) << fd_name;
  		    /*------------------------TYPE-----------------------*/
  		    sprintf(path, "/proc/%ld/fd/%ld", tgid, fd);
  		    readlink(path, temp, sizeof(temp));
  		    checkDel = temp;
  		    if(checkDel.find("(deleted)") != -1)
  		      cout << setw(10) <<"unknown";/*raccoon:type*/
  		    else 
  		      f_type(path, fp);
  		    /*------------------------NODE-----------------------*/
  		    stat(path, &info);
  		    /*raccoon:node*/
  		    cout << setw(9) << info.st_ino;
  		    /*-----------------------NAME------------------------*/
  		    readlink(path, temp, sizeof(temp));
  		    string str = temp;
		    cout << setw(8+str.length()) << temp << endl;/*raccoon:name*/
  		  }
  		}
	}
	return 0;	
}
void f_type(char* path, FILE* fp){
  struct stat info;
  stat(path, &info);
  if(fp == NULL)cout << setw(10) <<"unknown";
    else{
      switch(info.st_mode & S_IFMT){
	case S_IFDIR:	cout << setw(10) << "DIR" ;	break;
	case S_IFSOCK:	cout << setw(10) << "SOCK";	break;
	case S_IFREG:	cout << setw(10) << "REG";	break;
	case S_IFCHR:	cout << setw(10) << "CHR";	break;
	case S_IFIFO:	cout << setw(10) << "FIFO";	break;
	default:	cout << setw(10) << "unknown";  break;
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
    cout << temp << setw(32-strlen(temp));/*raccoon:COMMAND*/
  }	
  memset(temp, 0, sizeof(temp));/*raccoon:clear temp*/
  if(fp != NULL)fclose(fp);
  /*----------------------PID--------------------------*/
  cout << pid;/*raccoon:PID*/
  /*---------------------USER--------------------------*/
  sprintf(path, "/proc/%ld/loginuid", pid);/*raccoon:warning!this path is wrong*/
  fp = fopen(path, "r");
  if(fp != NULL){
    fscanf(fp, "%s", temp);
    long uid = stol(temp);
    if(uid == 4294967295)uid=0;/*raccoon:uid=-1 means yet to set*/
    struct passwd* pw = getpwuid(uid);
    /*raccoon:USER*/
    if(pw != NULL)cout << setw(11) << pw->pw_name;
    else cout << setw(11) <<"unknown";
  }
  memset(temp, 0, sizeof(temp));/*raccoon:clear temp*/
  if(fp != NULL)fclose(fp);
}
char f_mode(long pid, long fd){
  string a, b, c, d;
  sprintf(path, "/proc/%ld/fdinfo/%ld", pid, fd);
  ifstream infile(path);
  infile >> a >> b >> c >> d;
  switch(d.back()){
    case '0':	return 'r';	break;
    case '1':	return 'w';	break;
    case '2':	return 'u';	break;
    default:	return ' ';	break;
  }
}

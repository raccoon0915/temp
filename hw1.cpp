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
using namespace std;
const char *FD[5] = {"cwd", "root", "exe", "maps", "fd"};
void f_type(char* path, FILE* fp){
	struct stat info;
	stat(path, &info);
	if(fp == NULL){
		cout << "TYPE=unknown ";
		cout << "NODE= ";
	}
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
		cout << "NODE=" << info.st_ino << " ";
	}
	
} 
int main(int argc, char**argv){
	long tgid;
	struct dirent* ent;
	DIR* proc = opendir("/proc");
	if(proc == NULL)
		perror("failed to open proc...");
	char path[100], temp[1024];
	FILE* fp;
	while(ent = readdir(proc)){
		/*--------------------PID------------------------*/
		if(!isdigit(*ent->d_name))
			continue;
		tgid = strtol(ent->d_name, NULL, 10);

		/*---------------------TYPE--------------------------*/ 
		for(int i=0; i<5; i++){
		/*---------------------maps--------------------------*/
		/*raccoon:We need to check whether under the root mode before using "maps".*/
			if(FD[i] == "maps"){
				sprintf(path, "/proc/%c/maps", *ent->d_name);
				fp = fopen(path, "r");
				if(fp == NULL)continue;
			}
		/*--------------------COMMAND------------------------*/
			sprintf(path, "/proc/%c/comm", *ent->d_name);
			fp = fopen(path, "r");
			if(fp != NULL)
				fscanf(fp, "%s", temp);
			cout << "COMMAND=" << temp << " ";
			memset(temp, 0, sizeof(temp));/*raccoon:clear temp*/
		/*----------------------PID--------------------------*/
			cout << "PID=" << tgid << " ";
		/*---------------------USER--------------------------*/
			struct stat info;
			sprintf(path, "/proc/%c/loginuid", *ent->d_name);
			stat(path, &info);
			struct passwd* pw = getpwuid(info.st_uid);
			if(pw != NULL)
				cout << "USER=" << pw->pw_name <<" ";
			else
				cout << "USER=unknown ";
		/*-----------------------FD-----------------------*/
			if(FD[i] == "maps")cout << "FD=mem ";
			else if(FD[i] == "fd")cout << "FD=NOFD ";
			else cout << "FD=" << FD[i] << " ";
		/*----------------------TYPE--------------------------*/
			sprintf(path, "/proc/%c/%s", *ent->d_name, FD[i]);
			fp = fopen(path, "r");
			if(FD[i] == "fd" && fp == NULL){
				cout << "TYPE= ";
				cout << "NODE= ";
			}
			else f_type(path, fp);
		/*----------------------NAME--------------------------*/
			if(fp == NULL && FD[i] == "fd")
				cout << path << " (opendir: Permission denied)" << endl;
			else if(fp == NULL)
				cout << path << " (readlink: Permission denied)" << endl;
			else{
				readlink(path, temp, sizeof(temp));
				cout << temp << endl;
			}	
//			cout << "NAME=" << temp << endl;
		/*---------------------NODE--------------------------*/
//			cout << "NODE=" << ent->d_ino << endl;	
		}
	}
	if(fp != NULL)fclose(fp);
	return 0;
	
}

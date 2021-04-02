#include<iostream>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<filesystem>
#include<dirent.h>
#include<cstring>
#include<string>
#include<pwd.h>
using namespace std; 
int main(int argc, char**argv){
	long tgid;
	struct dirent* ent;
	DIR* proc = opendir("/proc");
	if(proc == NULL)
		perror("failed to open proc...");
	char path[100], temp[100];
	FILE* fp;
	while(ent = readdir(proc)){
		/*--------------------PID------------------------*/
		if(!isdigit(*ent->d_name))
			continue;
		tgid = strtol(ent->d_name, NULL, 10);
		cout << "PID=" << tgid << " ";
		/*--------------------USER------------------------*/
		struct stat info;
		sprintf(path, "/proc/%c/loginuid", *ent->d_name);
		fp = fopen(path, "r");
		if(fp != NULL){
			stat(path, &info);
			struct passwd* pw = getpwuid(info.st_uid);
			if(pw != 0)
				cout << "USER=" << pw->pw_name << " ";
		/*--------------------NODE & TYPE------------------------*/
		struct dirent* ent2/*raccoon:*/
		/*--------------------COMMAND------------------------*/
		sprintf(path, "/proc/%c/comm", *ent->d_name);
		fp = fopen(path, "r");
		if(fp != NULL)
			fscanf(fp, "%s", temp);
		cout << "COMMAND=" << temp << endl;
		/*--------------------COMMAND------------------------*/
	}
	fclose(fp);
	return 0;
	
}

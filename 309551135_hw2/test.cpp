#include<iostream>
#include<string.h>
int main(int argc, char** argv){
  for(int i=1; i<argc; i++){
    switch(argv[i]){
      case "-h":
	cout << ""
	     << "-p: set the path to logger.so, default = ./logger.so\n"
	     << "-o: print output to file, print to "stderr" if no file specified\n"
	     << "--: separate the arguments for logger and for the command"
      case ""
    }
  }
}

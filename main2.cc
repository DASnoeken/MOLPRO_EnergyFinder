#include <iostream>
#include <stdlib.h>
#include "ffindE.hh"
#include "FindEException.hh"
//Version 3.0 of the MOLPRO energy finder. Made by Daan A. Snoeken
int main(int argc, char** argv){
	std::cout<<"Hey there folks!\nWelcome to the MOLPRO energy finder version 3.0.\nMake sure MOLPRO outputfile names end in \".out\".\nMake sure to follow the correct MOLPRO output format."<<std::endl;
	std::cout<<"Made by Daan A. Snoeken, March 2019"<<std::endl;
	std::string jn;
	//system("ls *.out > ls.ls");
	if(argc==1){
		std::cout<<"Please enter the job name:"<<std::endl;
		std::cin>>jn;
		findE fe("ls.ls");
		fe.setJobName(jn);
		fe.newFile();
	}else if(argc==2){
		jn = std::string(argv[1]);
		findE fe("ls.ls");
		fe.setJobName(jn);
		fe.newFile();
	}else if(argc==3){
		jn = std::string(argv[1]);
		int j = ::atoi(argv[2]);
		findE fe("ls.ls",j);
		fe.setJobName(jn);
		fe.newFile();
	}else if(argc==4){
		jn = std::string(argv[1]);
		int i = ::atoi(argv[2]);
		int j = ::atoi(argv[3]);
		findE fe("ls.ls",i,j);
		fe.setJobName(jn);
		fe.newFile();
	}else{
		//system("\\rm ls.ls");
		FindEException X("Wrong number of input arguments!");
		std::cout<<X<<std::endl<<X.what()<<std::endl<<"You gave "<<argc-1<<" input arguments!"<<std::endl<<"This must lie between 0 and 3."<<std::endl;
		throw X;
	}
	system("\\rm ls.ls");
}

/* 
 * Using this to find molpro energies. This version is more general and faster than older versions.
 * Another upside to this program, is that it generates scilab compatible output that would be usable for the fitting of a potential.
 * The output will put all the energies in an n-dimensional hypermatrix, E_{ijkl...}. Depending on the dimensionality of the key.
 * The key is defined by the name of the MOLPRO outputfiles. The MOLPRO output should be named as "$jobname-$key.out".
 * The key should be shaped as ##-##-##-##-... where ## represents a number or a word/letter-number combination. Example (in 3d): potential-R1-P12-T5.out. Where "potential" represents the job name.
 * Please also note that it is CRUCIAL that the molpro output is in a table as defined in molpro itself. Molpro can generate a nice table with the table command. 
 * The line where a table is made, should be directly preceded by a line that says "text,table" in the input. This is how the program knows where to look for the energies.
 * */
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>			//strnlen
#include <sstream>
#include "FindEException.hh"
#ifndef FINDE_HH
#define FINDE_HH
class findE{
	public:
		findE(const char* filename):_file(filename){					//ctor
			if(_file.fail()){
				FindEException X("File failed to open!");
				std::cout<<X<<std::endl<<X.what()<<std::endl<<"Filename was: "<<filename<<std::endl;
				throw X;
			}
			filen1 = filename;
			settSize();
			int j;
			int i;
			std::string line;
			std::ifstream helper(filename);
			helper>>line;
			std::string command = "cat "+line;
			system(command.c_str());
			std::cout<<"You have to specify which state from your table you want to use (i.e. The table row):"<<std::endl;
			std::cin>>j;
			nos=j;
			std::cout<<"You have to specify which method from the table you want to use (i.e. the collumn number):"<<std::endl;
			std::cin>>i;
			selectCol=i;
			fFile.open("potential.dat",std::ofstream::out|std::ofstream::trunc);
		}
		
		findE(const char* filename,int j):_file(filename){				//overloaded ctor
			if(_file.fail()){
				FindEException X("File failed to open!");
				std::cout<<X<<std::endl<<X.what()<<std::endl<<"Filename was: "<<filename<<std::endl;
				throw X;
			}
			filen1 = filename;
			settSize();
			nos=j;
			int i;
			std::string line;
			std::ifstream helper(filename);
			helper>>line;
			std::string command = "cat "+line;
			system(command.c_str());
			std::cout<<"You have to specify which method from the table you want to use (i.e. the collumn number):"<<std::endl;
			std::cin>>i;
			selectCol=i;
			fFile.open("potential.dat",std::ofstream::out|std::ofstream::trunc);

		}
		
		findE(const char* filename,int i, int j):_file(filename){		//even more overloaded ctor
			if(_file.fail()){
				FindEException X("File failed to open!");
				std::cout<<X<<std::endl<<X.what()<<std::endl<<"Filename was: "<<filename<<std::endl;
				throw X;
			}
			filen1=filename;
			settSize();
			nos=i;
			selectCol = j;
			fFile.open("potential.dat",std::ofstream::out|std::ofstream::trunc);
		}
		
		~findE(){std::cout<<"\033[1;32mThat's All, Folks!\033[0m"<<std::endl;_file.close();mpfile.close();sciFile.close();fFile.close();}	//dtor
		
		void read(){													//Function to read the MOLPRO file
			std::string line;
			std::string word = "TABLE";
			while(std::getline(mpfile,line)){
				if(line.find(word)!=std::string::npos && line.find("text,TABLE")==std::string::npos){
//					genKey();
					for(int i=1; i<=tSize*(nos)+1;i++){					
						mpfile>>line;
					}
					dims="";
					for(int j=1; j<selectCol; j++){
						dims+=line+"    ";
						mpfile>>line;
					}
					Energy=line;
					write();
//					std::pair<std::string,std::string> potMapPair(key,Energy);
//					potMap.insert(potMapPair);
					break;
				}
			}
			newFile();
		}
		
		void write(){
			fFile<<dims<<Energy<<std::endl;
		}
		
		void newFile(){													//Function to access the next MOLPRO file
			_file>>mpfName;
			findJobName(mpfName);
			if(_file.eof()){
				std::cout<<"All files read."<<std::endl;
			}else{
				mpfile.close();
				mpfile.open(mpfName.c_str());
				if(mpfile.fail()){
					FindEException X("Molpro file failed to open!");
					std::cout<<X<<std::endl<<X.what()<<std::endl<<"Filename was: "<<mpfName<<std::endl;
					throw X;
				}
				read();
			}
		}
		
		void setJobName(std::string s){
			jobName=s;
			jobNameLength = std::strlen(jobName.c_str());
		}
		
		std::string getJobName(){
			return jobName;
		}
		
		std::size_t findJobName(std::string s){							//Function to find the jobname from the MOLPRO outputname
			found = s.find(jobName);
			if(found!=std::string::npos){
				return found;
			}
			else{
				//newFile();
				FindEException X("Job name not found!");
				std::cout<<X<<std::endl<<X.what()<<std::endl<<"Entered job name was: "<<jobName<<std::endl<<"Molpro file name was: "<<mpfName<<std::endl;
				throw X;
			}
		}
		
		int getJobNameLength(){
			return jobNameLength;
		}
	
		void settSize(){												//Function used to find the size of the MOLPRO table
			int tS = 0;
			std::string line;
			if(!mpfName.empty()){
				Tabmpfile.open(mpfName);
			}else{
				std::string mpfName2;
				_file2.open(filen1);
				if(_file2.fail()){
					FindEException X("File in settSize() failed to open!");
					std::cout<<X<<std::endl<<X.what()<<std::endl<<"filename was "<<filen1<<std::endl;
					throw X;
				}
				_file2>>mpfName2;
				Tabmpfile.open(mpfName2);
				_file2.close();
			}
			std::string word = "text,TABLE";							//The line after this should be a list of all the collumns
			while(getline(Tabmpfile,line)){
				if(line.find(word)!=std::string::npos){
					Tabmpfile>>line;
					std::istringstream iss(line);
					std::string token;
					while(std::getline(iss,token,',')){
						tS++;
					}
				}
			}
			Tabmpfile.close();
			tSize=tS-1;
		}
		
		int gettSize(){
			return tSize;
		}
		
		void setNOS(int i){												//NOS stands for no. of states, or basically the number of rows in the MOLPRO table
			nos=i;
		}
		
	private:
		std::ifstream _file;											//Refers to the ls.ls file created in main
		std::ifstream _file2;											//ls.ls 2
		std::ifstream mpfile;											//Molpro output file
		std::ifstream Tabmpfile;										//Used to find the table dimensions
		std::ofstream sciFile;											//scilab output file
		std::ofstream fFile;											//outputfile for the fortran routine
		std::string mpfName;											//Molpro output file name
		std::string jobName;											//Jobname
		int jobNameLength;												//length of jobName string
		std::size_t found;												//return value for the findJobName function
		std::string key;												//Key of the unordered map
		std::string Energy;												//Value of the unordered map
		std::string dims;												//Potential dimension values
		const char* filen1;												//filename copy
		int tSize;														//amount of collumns in table
		int nos;														//rows in table
		int selectCol;													//Select this collumn
};
#endif

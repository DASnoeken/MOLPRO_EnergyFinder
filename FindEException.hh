#include <iostream>
#ifndef FINDEEXCEPTION_HH
#define FINDEEXCEPTION_HH
class FindEException{
	public:
		FindEException(const char* name):_what(name){}
		const char* what(){return _what.c_str();}
		
	private:
		std::string _what;
};
std::ostream& operator <<(std::ostream& os, const FindEException& fee){
	return(os<<"\033[1;31mError!\033[0m");
}
#endif

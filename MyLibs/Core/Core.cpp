//#include"Procon2013/Exception/Exceptions.h"

#include"Core.h"


namespace pro{

std::string charToString(const char* str_c,int n){
	
	std::string str="";
	
	for(int i=0;i<n;i++){
		str += str_c[i];
		if(str_c[i]=='\0') break;
	}

	return str;
}

const char* stringToChar(const std::string& str){
	char* c_str = new char[str.size()+1];
	strcpy_s(c_str,str.size()+1,str.c_str());
	return c_str;
}

}
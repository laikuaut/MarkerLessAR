#pragma once

#include"../Core/type_c.h"

#include<string>
#include<typeinfo>
#include<stdexcept>

namespace pro{

/**
 *  —áŠOˆ—‚Ì©ìŠî’êƒNƒ‰ƒX
 */
class PRO_EXPORTS Exception : public std::exception
{
private:

protected:
	string name;
	string message;
	string fileName;
	string functionName;
	int throwLine;
public:
	Exception();
	Exception(const string& aMessage);
	Exception(const string& aMessage,
				const string& aFile,
				const string& aFunc,
				int aLine);
	virtual ~Exception();

	virtual const char* what() const throw();
	virtual const char* getName() const throw();
	virtual const char* getFileName() const throw();
	virtual const char* getFuncName() const throw();
	virtual int   getThrowLine() const throw();
	virtual void showError() const throw();
};

}
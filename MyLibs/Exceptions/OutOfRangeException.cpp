#include "OutOfRangeException.h"

#include<sstream>
//#include"Procon2013\Core\Core.h"
//#include"Procon2013\Camera\Camera.h"

namespace pro{

// ����`�V���{������
// warning C4661
template class OutOfRangeException<int>;
template class OutOfRangeException<long>;
template class OutOfRangeException<float>;
template class OutOfRangeException<double>;

// �T���v��
//try{
//	throw OutOfRangeException<double>(-1.10,"main.cpp","main()");
//}catch(const OutOfRangeException<double>& e){
//	e.showError();
//}

template<typename Type> 
OutOfRangeException<Type>::OutOfRangeException(void)
{
}

template<typename Type>
OutOfRangeException<Type>::OutOfRangeException
	(Type aNum)
{
	setNum(aNum);
}

template<typename Type>
OutOfRangeException<Type>::OutOfRangeException
	(Type aNum,const string& aAgument)
{
	setNum(aNum,aAgument);
}

template<typename Type>
OutOfRangeException<Type>::OutOfRangeException
	(const string& aMessage) : Exception(aMessage)
{
	message = aMessage;
}

template<typename Type>
OutOfRangeException<Type>::OutOfRangeException
	(const string& aMessage,const string& aFile,const string& aFunc,int aLine)
	: Exception(aMessage,aFile,aFunc,aLine)
{
}

template<typename Type>
OutOfRangeException<Type>::OutOfRangeException
	(Type aNum,const string& aFile,const string& aFunc,int aLine)
	: Exception("",aFile,aFunc,aLine)
{
	setNum(aNum);
}

template<typename Type>
OutOfRangeException<Type>::OutOfRangeException
	(Type aNum,const string& aAgument, const string& aFile,const string& aFunc,int aLine)
	: Exception("",aFile,aFunc,aLine)
{
	setNum(aNum,aAgument);
}

template<typename Type> 
OutOfRangeException<Type>::~OutOfRangeException(void)
{
}

template<typename Type>
void OutOfRangeException<Type>::setNum(Type aNum){
	num = aNum;
	std::stringstream ss;
	ss << num << "�͔͈͊O�Ȓl�ł��B";
	message = ss.str();
}

template<typename Type>
void OutOfRangeException<Type>::setNum(Type aNum,const string& aAgument){
	num = aNum;
	agument = aAgument;
	std::stringstream ss;
	ss << agument << " = " << num << " " << "�͔͈͊O�Ȓl�ł��B";
	message = ss.str();
}

}

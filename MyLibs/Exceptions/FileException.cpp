#include "FileException.h"

namespace pro{

FileException::FileException(void)
{
}

FileException::FileException(const string& aMassage) : Exception(aMassage){

}

FileException::FileException(const string& aMessage,
				const string& aFile,
				const string& aFunc,
				int aLine)
			: Exception(aMessage,aFile,aFunc,aLine){
}

FileException::FileException(const error_code aNum){
	setErrorCode(aNum);
}

FileException::FileException(const error_code aNum,const string& aOpenFName){
	setErrorCode(aNum,aOpenFName);
}

FileException::FileException(const error_code aNum,
				const string& aOpenFName,
				const string& aFile,
				const string& aFunc,
				int aLine)
				:Exception("",aFile,aFunc,aLine){
	setErrorCode(aNum,aOpenFName);
}

FileException::~FileException(void)
{
}

void FileException::setErrorCode(const error_code aCode,const string aOpenFName){
	code = aCode;
	switch(aCode){
	case OPEN:
		message = aOpenFName + "���J���܂���ł����B";
		break;
	case CLOSE:
		message = aOpenFName + "������܂���ł����B";
		break;
	case WRITE:
		message = aOpenFName + "���������߂܂���ł����B";
		break;
	case READ:
		message = aOpenFName + "���ǂݍ��߂܂���ł����B";
		break;
	case DIRECTORY:
		message = aOpenFName + "�̓f�B���N�g���ł��B";
		break;
	case NOT_EXIST:
		message = aOpenFName + "�͑��݂��܂���B";
		break;
	default:
		message = "�\�����ʃG���[";
		break;
	}
}

void FileException::setErrorCode(const error_code aCode){
	setErrorCode(aCode,"�t�@�C��");
}

FileException::error_code FileException::getErrorCode() const{
	return code;
}

}
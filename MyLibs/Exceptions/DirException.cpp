#include "DirException.h"

namespace pro{


DirException::DirException(void)
{
}

DirException::DirException(const string& aMessage,
				const string& aFile,
				const string& aFunc,
				int aLine)
			: Exception(aMessage,aFile,aFunc,aLine){
}

DirException::DirException(const error_code aCode){
	setErrorCode(aCode);
}

DirException::DirException(const error_code aCode,const string& aPath){
	setErrorCode(aCode,aPath);
}

DirException::DirException(const error_code aCode,
				const string& aPath,
				const string& aFile,
				const string& aFunc,
				int aLine)
				:Exception("",aFile,aFunc,aLine){
	setErrorCode(aCode,aPath);
}

DirException::~DirException(void)
{
}

void DirException::setErrorCode(const error_code aCode,const string aPath){
	code = aCode;
	switch(aCode){
	case EXIST:
		message = aPath + "�͂��łɑ��݂��Ă��܂��B";
		break;
	case NOT_EXIST:
		message = aPath + "�͑��݂��܂���B";
		break;
	case NOT_EMPTY:
		message = aPath + "�͋�ł͂���܂���B";
		break;
	case PATH_ERROR:
		message = aPath + "�͎g�p�ł��Ȃ��������܂�ł��܂��B";
		break;
	case NOT_DIRECTORY:
		message = aPath + "�̓f�B���N�g���ł͂���܂���B";
		break;
	default:
		message = "�\�����ʃG���[";
		break;
	}
}

void DirException::setErrorCode(const error_code aCode){
	setErrorCode(aCode,"�t�@�C��");
}

DirException::error_code DirException::getErrorCode() const{
	return code;
}

}
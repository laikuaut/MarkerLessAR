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
		message = aPath + "はすでに存在しています。";
		break;
	case NOT_EXIST:
		message = aPath + "は存在しません。";
		break;
	case NOT_EMPTY:
		message = aPath + "は空ではありません。";
		break;
	case PATH_ERROR:
		message = aPath + "は使用できない文字を含んでいます。";
		break;
	case NOT_DIRECTORY:
		message = aPath + "はディレクトリではありません。";
		break;
	default:
		message = "予期せぬエラー";
		break;
	}
}

void DirException::setErrorCode(const error_code aCode){
	setErrorCode(aCode,"ファイル");
}

DirException::error_code DirException::getErrorCode() const{
	return code;
}

}
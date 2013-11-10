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
		message = aOpenFName + "が開けませんでした。";
		break;
	case CLOSE:
		message = aOpenFName + "が閉じれませんでした。";
		break;
	case WRITE:
		message = aOpenFName + "が書き込めませんでした。";
		break;
	case READ:
		message = aOpenFName + "が読み込めませんでした。";
		break;
	case DIRECTORY:
		message = aOpenFName + "はディレクトリです。";
		break;
	case NOT_EXIST:
		message = aOpenFName + "は存在しません。";
		break;
	default:
		message = "予期せぬエラー";
		break;
	}
}

void FileException::setErrorCode(const error_code aCode){
	setErrorCode(aCode,"ファイル");
}

FileException::error_code FileException::getErrorCode() const{
	return code;
}

}
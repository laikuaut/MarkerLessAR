// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include "CameraException.h"

namespace pro{

CameraException::CameraException(void)
{

}

CameraException::CameraException(const error_code aCode){
	setErrorCode(aCode);
}

CameraException::CameraException(const error_code aCode,
				const string& aFile,
				const string& aFunc,
				int aLine)
			: Exception("",aFile,aFunc,aLine){
	setErrorCode(aCode);
}

CameraException::CameraException(const string& aMessage)
		: Exception(aMessage){

}

CameraException::CameraException(const string& aMessage,
				const string& aFile,
				const string& aFunc,
				int aLine)
			: Exception(aMessage,aFile,aFunc,aLine) {

}

CameraException::~CameraException(void)
{
}

void CameraException::setErrorCode(error_code aCode){
	code = aCode;
	switch (code)
	{
	case OPEN:
		message = "カメラが起動できませんでした。";
		break;
	default:
		message = "予期せぬエラー";
		break;
	}
}

}

// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once
#include "Exception.h"

namespace pro{

/**
 *  ファイル入出力用の例外処理
 */
class PRO_EXPORTS FileException :
	public Exception
{
public:

	typedef enum error_code{
		OPEN,
		CLOSE,
		WRITE,
		READ,
		DIRECTORY,
		NOT_EXIST
	}error_code;

protected:
	
	error_code code;
	string openFileName;

private :
	
	void setErrorCode(error_code aCode);
	void setErrorCode(error_code aCode,string file_name);

public:
	
	FileException(void);
	FileException(const error_code aCode);
	FileException(const error_code aCode,
				const string& aOpenFName);
	FileException(const error_code aCode,
				const string& aOpenFName,
				const string& aFile,
				const string& aFunc,
				int aLine);
	FileException(const string& aMessage);
	FileException(const string& aMessage,
				const string& aFile,
				const string& aFunc,
				int aLine);

	error_code getErrorCode() const;

	virtual ~FileException(void);

};

}
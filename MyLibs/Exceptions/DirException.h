#pragma once
#include "Exception.h"

namespace pro{

class PRO_EXPORTS DirException :
	public Exception
{
public:

	typedef enum error_code{
		EXIST			// ���݂��Ă���
		,NOT_EXIST		// ���݂��Ă��Ȃ�
		,NOT_EMPTY		// ��łȂ�
		,PATH_ERROR		// �p�X���Ԉ���Ă���
		,NOT_DIRECTORY	// �f�B���N�g���ł͂Ȃ�
		//,NOT_PATH		// 
	}error_code;

protected:
	
	error_code code;
	string path;

private:

	void setErrorCode(error_code num);
	void setErrorCode(error_code num,string file_name);

public:
	DirException(void);
	DirException(const error_code aCode);
	DirException(const error_code aCode,
				const string& aPath);
	DirException(const error_code aCode,
				const string& aPath,
				const string& aFile,
				const string& aFunc,
				int aLine);
	DirException(const string& aMessage);
	DirException(const string& aMessage,
				const string& aFile,
				const string& aFunc,
				int aLine);

	~DirException(void);

	error_code getErrorCode() const;
};

}
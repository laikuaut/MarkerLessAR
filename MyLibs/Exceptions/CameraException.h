#pragma once
#include "exception.h"

namespace pro{

class PRO_EXPORTS CameraException :
	public Exception
{
public:

	typedef enum error_code{
		OPEN
	}error_code;

protected:

	error_code code;

private:

	void setErrorCode(error_code aCode);

public:
	CameraException(void);
	CameraException(const error_code aCode);
	CameraException(const error_code aCode,
				const string& aFile,
				const string& aFunc,
				int aLine);
	CameraException(const string& aMessage);
	CameraException(const string& aMessage,
				const string& aFile,
				const string& aFunc,
				int aLine);

	~CameraException(void);
};

}
// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

#include"Exception.h"

namespace pro{

/**
 *  ˆø”‚É“ü‚ê‚½’l‚ª”ÍˆÍŠO‚Ì—áŠOˆ—
 */
template<typename Type> class PRO_EXPORTS OutOfRangeException
	: public Exception
{
protected:
	Type num;
	string agument;
private:
	void setNum(Type aNum);
	void setNum(Type aNum, const string& aAgument);
public:
	OutOfRangeException(void);
	OutOfRangeException(Type aNum);
	OutOfRangeException(Type aNum,const string& aAgument);
	OutOfRangeException(const string& aMessage);
	OutOfRangeException(const string& aMessage,
				const string& aFile,
				const string& aFunc,
				int aLine);
	OutOfRangeException(Type aNum,
				const string& aFile,
				const string& aFunc,
				int aLine);
	OutOfRangeException(Type aNum,
				const string& aAgument,
				const string& aFile,
				const string& aFunc,
				int aLine);

	~OutOfRangeException(void);
};

}
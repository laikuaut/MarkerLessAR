// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

#include"..\Core\Core.h"

#include"opencv2\core\core.hpp"
#include"opencv2\highgui\highgui.hpp"

namespace pro{

class PRO_EXPORTS MouseEvent
{
protected:

	int event,x,y,flag;

protected:
	MouseEvent(void);
	~MouseEvent(void);

	virtual void onMouse_impl(int event,int x,int y,int flag) = 0;
	static void onMouse(int event, int x, int y, int flag, void* data);
	//void onMouse(int event, int x, int y, int flag, void* data);

};

}
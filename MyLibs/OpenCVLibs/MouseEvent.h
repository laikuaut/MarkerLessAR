#pragma once

#include"..\Core\Core.h"

#include"opencv2\core\core.hpp"
#include"opencv2\highgui\highgui.hpp"

namespace pro{

class PRO_EXPORTS MouseEvent
{
public:

	int event,x,y,flag;

public:
	MouseEvent(void);
	~MouseEvent(void);

	virtual void onMouse_impl(int event,int x,int y,int flag) = 0;
	static void onMouse(int event, int x, int y, int flag, void* data);

};

}
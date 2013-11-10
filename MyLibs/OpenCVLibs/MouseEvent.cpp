// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include "MouseEvent.h"

namespace pro{

MouseEvent::MouseEvent(void)
{
}


MouseEvent::~MouseEvent(void)
{
}

void MouseEvent::onMouse(int event,int x,int y,int flag,void *data){
	MouseEvent *me = reinterpret_cast<MouseEvent*>(data);
	me->onMouse_impl(event,x,y,flag);
}


}
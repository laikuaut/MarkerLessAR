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
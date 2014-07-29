#include "DXGInput.h"

#include <cstdlib>

 CDXGInput::CDXGInput()
{
	memset(m_KeyMap, 0, sizeof(m_KeyMap));
	memset(m_ButtonMap, 0, sizeof(m_ButtonMap));
}
void CDXGInput::frameBegin()
{

}

void CDXGInput::frameUpdate()
{
   for(int i = 0; i <256; i++)
   {
	   if(isKeyDown((char)i))
	   {
       DXG_LOG("%c key down\n",(char) i);
	   }
   }
   for(int i = 0; i <3; i++)
   {
	   if(isMouseDown(i))
         DXG_LOG("%d mbutton down\n", i);
   }
}

void CDXGInput::frameEnd()
{

}

bool CDXGInput::isMouseDown(int vButton)
{
	Ret_Val_If_Fail(false, vButton >= 0 && vButton <3 && "vbutton must be 0, 1, or 2");

	return m_ButtonMap[vButton] == 1;
}
bool CDXGInput::isMouseUp(int vButton)
{
	Ret_Val_If_Fail(false, vButton >= 0 && vButton <3 && "vbutton must be 0, 1, or 2");

	return m_ButtonMap[vButton] == 0;
}
void CDXGInput::getMousePos(int &outX, int &outY)
{
	outX = m_MouseX;
	outY = m_MouseY;
}

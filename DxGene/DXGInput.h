
#ifndef DXGINPUT_H
#define DXGINPUT_H
#include "dxgbase.h"
#include "idxgframelistner.h"


class CDXGInput:
	public IDXGFrameListner
{
	
private:
	unsigned char m_KeyMap[256];
	unsigned char m_ButtonMap[4];
	int m_MouseX;
	int m_MouseY;
	friend LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
public:
	static const unsigned int DXGINPUT_LBUTTON = 0;
	static const unsigned int DXGINPUT_MBUTTON = 1;
	static const unsigned int DXGINPUT_RBUTTON = 2;
public:
	CDXGInput();
	virtual ~CDXGInput(void){};
public:
	virtual void frameBegin();
	virtual void frameUpdate();
	virtual void frameEnd();
	virtual int getUpdateOrder(){ return 0;}
public:
	bool isKeyDown(char vKey){ return m_KeyMap[(unsigned char)vKey] == 1;}
	
	bool isKeyUp(char vKey){ return m_KeyMap[(unsigned char)vKey] == 0;};
	bool isMouseDown(int vButton);
	bool isMouseUp(int vButton);
	void getMousePos(int &outX, int &outY);
};
#endif
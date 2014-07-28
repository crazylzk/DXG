#pragma once
#include "DXGBase.h"
#include <vector>

class IDXGFrameListner;
class CDXGInput;
class CDXGEnv
{
	
	typedef std::vector<IDXGFrameListner *> m_Lisnter;
private:
	int m_WinWidth;
	int m_WinHeight;
	bool m_VSync;
	bool m_FullScreen;
	HWND m_WinHandle; /*Windows Handle*/
	ID3D11Device *m_Device; /*Device  handle*/
	IDXGISwapChain * m_SwapChain;
	ID3D11DeviceContext *m_Context;
	ID3D11RenderTargetView * m_DefaultRTV;
	ID3D11Texture2D  *m_DepthStencilTX;
	ID3D11DepthStencilView * m_DefaultDSV;
    
	CDXGInput *m_Input;
  
	static CDXGEnv * s_Instance;


private:
	friend LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	bool initWindow(int vPosX, int vPosY, int vWinWidth, int vWinHeight);
	bool initDevice();
	bool initInput();
	bool deInitInput();
	void deInitEnv();
	void render();
	void onResize(int vWidth, int vHeight);

public:
	CDXGEnv(void);
	bool initEnv(int vPosX, int vPosY, int vWinWidth, int vWinHeight, bool vVerticlSync = true, bool vFullScreen = false);
	void registerFrameListner(int vUpdateOrder, IDXGFrameListner * vListner);
	void unregsiterFrameListner(IDXGFrameListner * vListner);
	void goMainLoop();
	void swapBuffers();
	HWND  getWindowHandle() { return m_WinHandle ;}
	ID3D11DeviceContext *getDeviceContext(); 
	ID3D11Device   *getDevice();
	ID3D11RenderTargetView *getDefaultRTV();
	ID3D11DepthStencilView *getDefaultDSV();
	ID3D11Texture2D * getDefaultDepthStencilTX();
	CDXGInput * getInput();
	static CDXGEnv * instance();
	
	~CDXGEnv(void);
};

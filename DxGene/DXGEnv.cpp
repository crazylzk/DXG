#include "DXGEnv.h"

CDXGEnv * CDXGEnv::s_Instance = NULL;
CDXGEnv::CDXGEnv(void) : m_WinHeight(0), m_WinWidth(0),m_FullScreen(false),
						m_WinHandle(NULL),m_Device(NULL),m_Context(NULL),m_DefaultRTV(NULL),m_DefaultDSV(NULL),
						m_SwapChain(NULL),m_DepthStencilTX(NULL),m_VSync(false),m_Input(NULL)
{
	if (s_Instance)
	{
		DXG_LOG("CDXGENV should only create once\n");
	}
	s_Instance = this;
}


CDXGEnv::~CDXGEnv(void)
{
}

CDXGEnv * CDXGEnv::instance()
{
	Ret_Val_If_Fail(NULL, s_Instance != NULL );

	return s_Instance;
}

ID3D11Device * CDXGEnv::getDevice()
{
	Ret_Val_If_Fail(NULL, m_Device != NULL);

	return m_Device;
}

ID3D11DeviceContext * CDXGEnv::getDeviceContext()
{
	Ret_Val_If_Fail(NULL, m_Context != NULL);

	return m_Context;
}

ID3D11RenderTargetView * CDXGEnv::getDefaultRTV()
{
	Ret_Val_If_Fail(NULL, m_DefaultRTV != NULL);

	return m_DefaultRTV;
}

ID3D11DepthStencilView * CDXGEnv::getDefaultDSV()
{
	Ret_Val_If_Fail(NULL, m_DefaultDSV != NULL);

	return m_DefaultDSV;
}

ID3D11Texture2D * CDXGEnv::getDefaultDepthStencilTX()
{
	Ret_Val_If_Fail(NULL, m_DepthStencilTX != NULL);

	return m_DepthStencilTX;
}



bool CDXGEnv::initEnv(int vPosX, int vPosY, int vWinWidth, int vWinHeight, bool vVerticlSync, bool vFullScreen)
{
	Ret_Val_If_Fail(false, vWinWidth > 0 && vWinHeight > 0);
  
	m_WinHeight = vWinHeight;
	m_WinWidth = vWinWidth;
	m_VSync = vVerticlSync;
	m_FullScreen = vFullScreen;

	if (!initWindow(vPosX, vPosY, vWinWidth, vWinHeight))
	{
		return false;
	}

	if(!initInput())
	{
		DXG_LOG("input init failded\n");
		return false;
	}

	if(!initDevice())
	{
		return false;
	}
	return true;
}

void CDXGEnv::deInitEnv()
{
	if(m_FullScreen)
	{
		m_SwapChain->SetFullscreenState(FALSE, NULL);
	}
	if(m_Context) m_Context->ClearState();
	Safe_Release(m_DefaultDSV);
	Safe_Release(m_DefaultRTV);
	Safe_Release(m_DepthStencilTX);

	Safe_Release(m_SwapChain);
	Safe_Release(m_Context);
	Safe_Release(m_Device);
}

void CDXGEnv::goMainLoop()
{
	MSG msg = {0};
	
	Ret_If_Fail(m_Device!=NULL && m_Context != NULL);
	
	while(WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			render();
		}
	}
	deInitEnv();
	
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch( message )
	{
	case WM_PAINT:
		hdc = BeginPaint( hWnd, &ps );
		EndPaint( hWnd, &ps );
		break;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	case WM_SIZE:
		{
			RECT rc;
			GetClientRect(CDXGEnv::instance()->getWindowHandle(),&rc);
			CDXGEnv::instance()->onResize(rc.right - rc.left, rc.bottom - rc.top);
		}
		break;


	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return 0;
}

void CDXGEnv::onResize(int vWidth, int vHeight)
{
	ID3D11Texture2D* pBuffer;
	D3D11_TEXTURE2D_DESC descDepth;
	HRESULT hr;
	m_Context->OMSetRenderTargets(0, 0, 0);

	/* Resize color render target View */
	m_DefaultRTV->Release();
	m_SwapChain->ResizeBuffers(2, vWidth, vHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	
	hr = m_SwapChain->GetBuffer(0, __uuidof( ID3D10Texture2D),
		(void**) &pBuffer );
	// Perform error handling here!

	hr = m_Device->CreateRenderTargetView(pBuffer, NULL,
		&m_DefaultRTV);
	// Perform error handling here!
	pBuffer->Release();

	m_DefaultDSV->Release();
	m_DepthStencilTX->Release();


	ZeroMemory( &descDepth, sizeof(descDepth) );
	descDepth.Width = vWidth;
	descDepth.Height = vHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_Device->CreateTexture2D( &descDepth, NULL, &m_DepthStencilTX );
	
	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_Device->CreateDepthStencilView(m_DepthStencilTX, &descDSV, &m_DefaultDSV);
	m_Context->OMSetRenderTargets(1, &m_DefaultRTV, m_DefaultDSV );

	// Set up the viewport.
	D3D11_VIEWPORT vp;
	vp.Width = vWidth;
	vp.Height = vHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_Context->RSSetViewports( 1, &vp);

}


bool CDXGEnv::initWindow(int vPosX, int vPosY, int vWinWidth, int vWinHeight)
{
	    bool hasReg = false;
		WNDCLASSEX wcex;
		memset(&wcex, 0 ,sizeof(wcex));
		wcex.cbSize = sizeof( WNDCLASSEX );
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandle(NULL);
		wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = L"DXGeneClass";
		RECT rc = { vPosX, vPosY, vWinWidth, vWinHeight };

		do 
		{
			if(!RegisterClassEx( &wcex))
				break;
			hasReg = true;

			AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

			m_WinHandle = CreateWindow( L"DXGeneClass", L"Direct3D 11 Tutorial ", WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wcex.hInstance,
				NULL );
			if(!m_WinHandle)
				break;

			ShowWindow( (HWND)m_WinHandle, 1 );
			
			return true;
		} while( 0 );

		if(hasReg)
		{
			UnregisterClass(wcex.lpszClassName,wcex.hInstance);
		}
		
		return false;
}

bool CDXGEnv::initInput()
{
	m_Input = new CDXGInput();
	registerFrameListner(1, m_Input);
	return true;
}

void CDXGEnv::deInitInput()
{
	unregsiterFrameListner(m_Input);
	delete m_Input;
	m_Input = NULL;

}
CDXGInput * CDXGEnv::getInput()
{
	Ret_Val_If_Fail(NULL, m_Input != NULL && "input should used after initEnv");
	
	return m_Input;
}

bool CDXGEnv::initDevice()
{
	RECT rc;
	HRESULT hr;
	unsigned int width, height;
	unsigned int createFlags = 0;
	D3D_DRIVER_TYPE type = D3D_DRIVER_TYPE_HARDWARE;
	ID3D11Texture2D *depthStencil = NULL;

	D3D_FEATURE_LEVEL levels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	D3D_FEATURE_LEVEL rlevel;
	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0 , sizeof(sd));
	GetClientRect((HWND) m_WinHandle, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	
	sd.BufferCount = 2;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = (HWND)m_WinHandle;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = !m_FullScreen;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	do {

		hr = D3D11CreateDeviceAndSwapChain(NULL, type, NULL,createFlags,levels,3,D3D11_SDK_VERSION,&sd,&m_SwapChain,&m_Device,
			&rlevel, &m_Context);

		if(FAILED(hr))
			break;

		ID3D11Texture2D *pBackBuffer = NULL;

		hr = m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
		if( FAILED( hr ) )
			break;

		hr = m_Device->CreateRenderTargetView( pBackBuffer, NULL, &m_DefaultRTV);
		pBackBuffer->Release();
		if( FAILED( hr ) )
			break;

		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory( &descDepth, sizeof(descDepth) );
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = m_Device->CreateTexture2D( &descDepth, NULL, &m_DepthStencilTX );
		if( FAILED( hr ) )
			break;

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory( &descDSV, sizeof(descDSV) );
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = m_Device->CreateDepthStencilView(m_DepthStencilTX, &descDSV, &m_DefaultDSV);
		if( FAILED( hr ) )
			break;
		m_Context->OMSetRenderTargets(1,&m_DefaultRTV, m_DefaultDSV);

		D3D11_VIEWPORT vp;
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX =  vp.TopLeftY = 0;
		m_Context->RSSetViewports(1, &vp);

		return true;
	} while(0);

	return false;

}

void CDXGEnv::render()
{

	float clearColor[] = {0.0f, 0.0f ,0.0f, 1.0f};
	m_Context->ClearRenderTargetView(m_DefaultRTV, clearColor);
	m_Context->ClearDepthStencilView(m_DefaultDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	swapBuffers();
}

void CDXGEnv::swapBuffers()
{
	m_SwapChain->Present(m_VSync ? 1 : 0, 0);
}
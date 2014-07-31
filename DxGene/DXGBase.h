#ifndef DXG_BASE_H 
#define DXG_BASE_H

#include <windows.h>
#include <winuser.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <D3Dcompiler.h>
#include <xnamath.h>
#include <cstdio>
#define DXG_LOG printf

#define Ret_If_Fail(x) { if(!(x)){DXG_LOG("E#at%s:%d line "#x"\n", __FUNCTION__, __LINE__);} }
#define Ret_Val_If_Fail(val, x) { if(!(x)){ DXG_LOG("E#at%s:%d line "#x"\n", __FUNCTION__, __LINE__); return val; }}
#define Break_If_Fail(val, x)  { if(!(x)){ DXG_LOG("E#at%s:%d line "#x"\n", __FUNCTION__, __LINE__); break; }} 
#define Safe_Release(p) {if((p)){ (p)->Release(); (p) = NULL;}}
#endif
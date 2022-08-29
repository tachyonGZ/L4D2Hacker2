#define WIN32_LEAN_AND_MEAN

#include <iostream>

#include <windows.h>

#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "Detours.h"
#pragma comment(lib, "Detours.lib")

#include "CConsole.h"
#include "CGameMemory.hpp"
#include "CDirect3D.hpp"
#include "CIMGUI.hpp"
#include "CSettings.hpp"

typedef HRESULT (__stdcall *EndScene_t)(IDirect3DDevice9 *thiz);
EndScene_t g_oldEndScene = nullptr;
typedef HRESULT (__stdcall *Reset_t)(IDirect3DDevice9 *thiz, D3DPRESENT_PARAMETERS *pPresentationParameters);
Reset_t g_oldReset = nullptr;

WNDPROC g_oldWndProc = nullptr;
HWND gWndHandle = nullptr;

LRESULT __stdcall newWndProc(HWND wndHandle, unsigned message, WPARAM wParam, LPARAM lParam)
{
	CDirect3D &direct3D = CDirect3D::GetInstance();
	CIMGUI &imgui = CIMGUI::GetInstance();
    if(WM_KEYDOWN == message && VK_F9 == wParam)
    {
		imgui.Toggle();
    }
    else if(WM_SIZE == message)
    {
		if(wParam != SIZE_MINIMIZED)
		{
			if(direct3D.IsInit())
			{
        		direct3D.Resize(wndHandle);
			}
			/*
			imgui.Reset();
			*/
		}
    }
    if (imgui.WndProcHandler(wndHandle, message, wParam, lParam))
        return 1;
    return ::CallWindowProc(g_oldWndProc, wndHandle, message, wParam, lParam);
}

HRESULT __stdcall newReset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	CDirect3D &direct3D = CDirect3D::GetInstance();
	direct3D.Reset();
	/*
	g_IMGUI.Reset();
	*/
	HRESULT hRes = g_oldReset(pDevice, pPresentationParameters);
	direct3D.EndReset();
	return hRes;
}

HRESULT __stdcall newEndScene(IDirect3DDevice9 *pDevice)
{
	CDirect3D &direct3D = CDirect3D::GetInstance();
	CIMGUI &imgui = CIMGUI::GetInstance();

    if(!direct3D.IsInit())
    {
        if(direct3D.Init(pDevice))
        {
            CConsole::WriteLine("[Direct3D]Direct3D初始化成功");
        }
        else
        {
            CConsole::WriteLine("[Direct3D]Direct3D初始化失败");
        }
    }

    direct3D.Draw(pDevice);

	if(!imgui.IsInit())
	{
		imgui.Init(pDevice);
	}

	imgui.Draw();

    return g_oldEndScene(pDevice);
}

void OnHook()
{
    gWndHandle = ::FindWindow("Valve001", "Left 4 Dead 2 - Direct3D 9");
    // hook WndProc
    g_oldWndProc = reinterpret_cast<WNDPROC>(::SetWindowLong(gWndHandle, GWL_WNDPROC, reinterpret_cast<LONG>(newWndProc)));

    // hook EndScene
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.hInstance = ::GetModuleHandle(nullptr);
    wc.lpfnWndProc = DefWindowProc;
    wc.lpszClassName = "DummyWindow";

    ::RegisterClassEx(&wc);

    HWND hWnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        "",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );

    IDirect3D9* pFactroy = ::Direct3DCreate9(D3D_SDK_VERSION);

    if(pFactroy == nullptr)
    {
        CConsole::WriteLine("[HOOK]IDirect3D9创建失败");
        return;
    }

    D3DPRESENT_PARAMETERS pp = { 0 };
    pp.Windowed = TRUE;
	pp.SwapEffect = D3DSWAPEFFECT_COPY;

    IDirect3DDevice9* pDevice = nullptr;

    HRESULT hRes = pFactroy -> CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &pp,
        &pDevice
    );

    if(FAILED(hRes))
    {
        CConsole::WriteLine("[HOOK]IDirect3DDevice9创建失败");
        return;
    }

    DWORD_PTR *pVTable = reinterpret_cast<DWORD_PTR *>(pDevice);
    pVTable = reinterpret_cast<DWORD_PTR *>(*pVTable);
	g_oldReset = reinterpret_cast<Reset_t>(pVTable[16]);
    g_oldEndScene = reinterpret_cast<EndScene_t>(pVTable[42]);

    ::DetourTransactionBegin();
    ::DetourUpdateThread(::GetCurrentThread());
	::DetourAttach(reinterpret_cast<void **>(&g_oldReset), newReset);
    ::DetourAttach(reinterpret_cast<void **>(&g_oldEndScene), newEndScene);
    ::DetourTransactionCommit();

    if(pDevice != nullptr) pDevice -> Release();
    if(pFactroy != nullptr) pFactroy -> Release();
    ::DestroyWindow(hWnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

void OnUnhook()
{
    // Unhook EndScene
    ::DetourTransactionBegin();
    ::DetourUpdateThread(GetCurrentThread());
    ::DetourDetach(reinterpret_cast<void **>(&g_oldEndScene), newEndScene);
    ::DetourTransactionCommit();

    // unhook WndProc
    if(g_oldWndProc)
    {
        SetWindowLong(gWndHandle, GWL_WNDPROC, reinterpret_cast<LONG>(g_oldWndProc));
    }

	CIMGUI::GetInstance().Uninit();
}

DWORD OnDllAttach(void *)
{
    if(CConsole::Init())
    {
        CConsole::WriteLine("[主线程][DLL]CConsole类初始化成功");
    }
    else
    {
        CConsole::WriteLine("[主线程][DLL]CConsole类初始化失败");
		return 1;
    }

    if(CGameMemory::GetInstance().Init())
    {
        CConsole::WriteLine("[主线程][DLL]CGameMemory类初始化成功");
    }
    else
    {
        CConsole::WriteLine("[主线程][DLL]CGameMemory类初始化失败");
        return 1;
    }

	CSettings::GetInstance().Init();

    OnHook();
    return 0;
}

DWORD OnDllDetach(void *)
{
    OnUnhook();

	CSettings::GetInstance().Uninit();
    CConsole::UnInit();
    return 0;
}

bool __stdcall DllMain(HMODULE hModule, DWORD dwReason, void *lpReserved)
{
    if(DLL_PROCESS_ATTACH == dwReason)
    {
        ::DisableThreadLibraryCalls(hModule);

        DWORD dwThreadID = 0;
        ::CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)OnDllAttach, nullptr, 0, &dwThreadID);
		
    }
    else if(DLL_PROCESS_DETACH == dwReason)
    {
        DWORD dwThreadID = 0;
        ::CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)OnDllDetach, nullptr, 0, &dwThreadID);
    }
    
    return true;
}
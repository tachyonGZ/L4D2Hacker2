#pragma once

#define WIN32_LEAN_AND_MEAN

#include <d3dx9.h>
#include <windows.h>

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND wndHandle, UINT message, WPARAM wParam, LPARAM lParam);

class CIMGUIBase
{
private:
	bool isInit = false;
	bool isActive = false;
	bool isLost = false;
protected:
	virtual void CreateResource() = 0;
public:
	void Init(IDirect3DDevice9 *const pDevice);
	void Uninit();
	void Draw();
	void Reset();
	void EndReset();
	void Toggle();
	long WndProcHandler(HWND wndHandle, unsigned message, WPARAM wParam, LPARAM lParam);

	inline bool IsInit()
	{
		return isInit;
	}

	virtual void Render() = 0;
};
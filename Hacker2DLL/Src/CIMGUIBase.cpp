#include "CIMGUIBase.hpp"

void CIMGUIBase::Init(IDirect3DDevice9 *const pDevice)
{
	if(pDevice == nullptr) return;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	D3DDEVICE_CREATION_PARAMETERS dcp{ 0 };
    pDevice -> GetCreationParameters(&dcp);

	ImGui_ImplWin32_Init(dcp.hFocusWindow);
    ImGui_ImplDX9_Init(pDevice);

	CreateResource();

	isInit = true;
}

void CIMGUIBase::Uninit()
{
	if(!isInit) return;

	ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	isInit = false;
}

void CIMGUIBase::Toggle()
{
	if(!isInit) return;
	isActive = !isActive;
}

void CIMGUIBase::Draw()
{
	if(isLost) return;

	if(isActive)
	{
		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX9_NewFrame();
		ImGui::NewFrame();
		Render();
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
}

void CIMGUIBase::Reset()
{
	if(!isInit)	return;

	isLost = true;
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void CIMGUIBase::EndReset()
{
	if(!isInit)	return;

	ImGui_ImplDX9_CreateDeviceObjects();
	isLost = false;
}

long CIMGUIBase::WndProcHandler(HWND wndHandle, unsigned message, WPARAM wParam, LPARAM lParam)
{
	if(!isInit)	return 0;
	return ImGui_ImplWin32_WndProcHandler(wndHandle, message, wParam, lParam);
}
#include "CIMGUI.hpp"

void CIMGUI::CreateResource()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	//io.Fonts -> AddFontDefault();
	io.Fonts -> AddFontFromFileTTF("D:\\HackerText.ttf", 14.0f, nullptr, io.Fonts -> GetGlyphRangesDefault());
}

void CIMGUI::Render()
{
	CSettings &settings = CSettings::GetInstance();
	ImGui::Begin("L4D2Hacker Menu");
	ImGui::Text(u8"ƒ„∫√ ¿ΩÁ");
	if(ImGui::Checkbox("Show Model Blue Line", &settings.isShowModelBlueLine))
	{
		CGameMemory::GetInstance().ToggleModelBlueLine(settings.isShowModelBlueLine);
	}
	if(ImGui::Checkbox("Show Footstep", &settings.isShowFootstep))
	{
		CGameMemory::GetInstance().ToggleFootStep(settings.isShowFootstep);
	}
	ImGui::ShowDemoWindow();
	/*
	if(ImGui::Checkbox("Third Person", settings.isThirdPersonPerspective))
	{
		CGameMemory::GetInstance().ToggleThirdPersonPerspective(settings.isThirdPersonPerspective);
	}
	*/
	ImGui::End();
}
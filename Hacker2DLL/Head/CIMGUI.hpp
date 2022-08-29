#pragma once

#define WIN32_LEAN_AND_MEAN

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "CSettings.hpp"
#include "CGameMemory.hpp"

#include "CIMGUIBase.hpp"
#include "CSingleton.hpp"

class CIMGUI : public CIMGUIBase, public CSingleton<CIMGUI>
{
private:
	virtual void CreateResource();
public:
	virtual void Render();
};
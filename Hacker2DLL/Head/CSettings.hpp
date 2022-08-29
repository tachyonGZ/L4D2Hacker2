#pragma once

#define WIN32_LEAN_AND_MEAN

#include "CSettingsBase.hpp"
#include "CSingleton.hpp"

class CIMGUI;

class CSettings : public CSettingsBase, public CSingleton<CSettings>
{
private:
	bool isShowModelBlueLine = false;
	bool isShowFootstep = false;
	bool isThirdPersonPerspective = false;

	friend class CIMGUI;
};
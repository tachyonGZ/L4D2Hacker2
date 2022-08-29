#include "CDirect3DBase.hpp"

bool CDirect3DBase::Init(IDirect3DDevice9 *const pDevice)
{
    if(pDevice == nullptr) return false;

    D3DDEVICE_CREATION_PARAMETERS dcp{ 0 };
    pDevice -> GetCreationParameters(&dcp);

    Resize(dcp.hFocusWindow);

    if(!CreateResource(pDevice)) return false;

	isInit = true;

    return true;
}

void CDirect3DBase::Uninit()
{
    DestroyResource();

	isInit = false;
}

void CDirect3DBase::Draw(IDirect3DDevice9 *const pDevice)
{
	Render(pDevice);
}

void CDirect3DBase::Reset()
{
	isLost = true;
	LostResource();
}

void CDirect3DBase::EndReset()
{
	if(isLost)
	{
		ResetResource();
	}
	isLost = false;
}
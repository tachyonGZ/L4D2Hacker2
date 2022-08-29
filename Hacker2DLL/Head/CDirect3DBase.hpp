#pragma once

#define WIN32_LEAN_AND_MEAN

#include <d3dx9.h>
#include <windows.h>

#include "CDirect3DMath.hpp"

class CDirect3DBase	: public CDirect3DMath
{
private:
    IDirect3DDevice9* pDevice = nullptr;

    bool isInit = false;
	bool isLost = false;
public:
    bool Init(IDirect3DDevice9 *const pDevice);
    void Uninit();
	void Draw(IDirect3DDevice9 *const pDevice);
	void Reset();
	void EndReset();

	inline bool IsInit()
	{
		return isInit;
	}

    virtual bool CreateResource(IDirect3DDevice9 *const pDevice) = 0;
    virtual void DestroyResource() = 0;
	virtual void Render(IDirect3DDevice9 *const pDevice) = 0;
	virtual void LostResource() = 0;
	virtual void ResetResource() = 0;
    virtual void Resize(HWND wndHandle) = 0;

    static inline void SafeRelease(IUnknown *unknown)
	{
		if(unknown != nullptr) unknown -> Release();
	}
};
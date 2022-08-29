#pragma once

#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <array>

#include <d3dx9.h>
#include <windows.h>

#include "CDirect3DBase.hpp"
#include "CGameMemory.hpp"
#include "CConsole.h"

#include "CScreenData.hpp"
#include "CSingleton.hpp"

struct D3DVector
{
	float x, y, z, rhw;
	std::int32_t color;
};

class CDirect3D : public CDirect3DBase, public CSingleton<CDirect3D>
{
private:

    ID3DXLine *pPerspectiveLine = nullptr;
    ID3DXLine *pHealthLine = nullptr;
    ID3DXLine *pSpindleLine = nullptr;

    static constexpr float perspectiveLineWidth = 2.0f;
    static constexpr float healthLineWidth = 10.0f;
    static constexpr float spindleLineWidth = 1.0f;

    ID3DXFont *pFont = nullptr;
    static constexpr int fontLogicalSize = 20;

	D3DXVECTOR2 spindlePoint{ 0.0f, 0.0f };
    D3DXVECTOR2 WndRect{ 0.0f, 0.0f };

	float matrix[4][4]{ 0.0f };

	using ScreenDataArray_t = std::array<ScreenData, CGameMemory::MAX_PLAYER>;
	using PerspectiveVertex_t = D3DXVECTOR2[5];
	using HealthVertex_t = D3DXVECTOR2[2];
	using SpindleVertex_t = D3DXVECTOR2[2];

	ScreenDataArray_t ScreenDataArray;

    void DrawForPlayer(CGameMemory &gameMemory);
	void DrawForScreenData();

	IDirect3DVertexBuffer9 *pVertexBuffer = nullptr;

public:
	CDirect3D() = default;
	CDirect3D(const CDirect3D&) = delete;
	CDirect3D &operator=(const CDirect3D&) = delete;

    virtual bool CreateResource(IDirect3DDevice9 *const pDevice);
    virtual void DestroyResource();
    virtual void Render(IDirect3DDevice9 *const pDevice);
	virtual void LostResource();
	virtual void ResetResource();
	virtual void Resize(const HWND wndHandle);
};
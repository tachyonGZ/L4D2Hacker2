#pragma once

#include <memory>

#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>

#include "GameMemory.h"
#include "Graphics.h"
#include "CConsole.h"
#include "CDirect3DBase.h"

class CDirect3D : public CDirect3DBase
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

    D3DXVECTOR2 SpindleVertex[2]{ {0.0f, 0.0f} };
    Graphics::rectType WndRect{ 0 };

    
    void NDCToScreen(const float (&NDC_pos)[2], D3DXVECTOR2 &screen_pos);
    bool WorldToScreen(const float (&world_pos)[3], D3DXVECTOR2 &screen_pos);

    void DrawForPlayer(const unsigned int playerIndex);
    void DrawSpindle(const D3DXVECTOR2 &screenPos, const D3DCOLOR color);
    void DrawHealth(const int health, const D3DXVECTOR2 &screenPos, const float heigh);
    void DrawName(const char *name, const D3DXVECTOR2 &screenPos, const D3DCOLOR color);
public:
    virtual bool CreateResource(IDirect3DDevice9 *const pDevice);
    virtual void DestroyResource();
    virtual void OnResize(const HWND wndHandle);
    virtual void Draw();
};
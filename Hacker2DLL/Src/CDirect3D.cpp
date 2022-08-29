#include "CDirect3D.hpp"

inline void DrawScreenPerspectiveRect(ID3DXLine *pLine, const D3DXVECTOR2 (&vertex)[5], const D3DCOLOR color)
{
	pLine -> Draw(vertex, 5, color);
}

inline void DrawScreenHealth(ID3DXLine *pLine, const D3DXVECTOR2 (&vertex)[2])
{
	pLine -> Draw(vertex, 2, D3DCOLOR_XRGB(221, 0, 27));
}

inline void DrawScreenSpindle(ID3DXLine *pLine, const D3DXVECTOR2 (&vertex)[2], const D3DCOLOR color)
{
	pLine -> Draw(vertex, 2, color);
}

inline void DrawScreenName(ID3DXFont *pFont, const char *name, RECT &rect, const D3DCOLOR color)
{
	if(name == nullptr) return;
	pFont -> DrawTextA(nullptr, name, -1, &rect, DT_CALCRECT, color);
	pFont -> DrawTextA(nullptr, name, -1, &rect, DT_BOTTOM | DT_LEFT, color);
}

void CDirect3D::DrawForPlayer(CGameMemory &gameMemory)
{
	CGameMemory::PlayerDataArray_t &playerDataArray = gameMemory.PlayerDataArray;
	for(size_t playerIndex = 1; playerIndex < playerDataArray.size(); playerIndex++)
    {
		
		ScreenDataArray_t::value_type &ScreenData = ScreenDataArray[playerIndex];
		ScreenData.isActive = false;

		CGameMemory::PlayerDataArray_t::value_type &PlayerData = playerDataArray[playerIndex];
		if(!PlayerData.isActive) continue;

		float WorldTopPos[3]{
			PlayerData.worldPos[0],
			PlayerData.worldPos[1],
			PlayerData.worldPos[2]
		};

		if(CGameMemory::death_health == PlayerData.health) continue;

		if(CGameMemory::person_team == PlayerData.team)
		{
			PlayerData.worldPos[2] -= CGameMemory::person_zpos_offset;
			WorldTopPos[2] += CGameMemory::person_heigh;
			//teamColor = D3DCOLOR_XRGB(0,255,0);
			ScreenData.teamColor = D3DCOLOR_XRGB(0,255,0);
		}
		else if(CGameMemory::special_team == PlayerData.team)
		{
			
			PlayerData.worldPos[2] -= CGameMemory::special_zpos_offset;
			WorldTopPos[2] += CGameMemory::special_heigh;
			//teamColor = D3DCOLOR_XRGB(255,0,0);
			ScreenData.teamColor = D3DCOLOR_XRGB(0,255,0);
		}
		else
		{
			ScreenData.isActive = false;
			continue;
		}

		D3DXVECTOR2 screenVertex;

		if(WorldToScreen(matrix, PlayerData.worldPos, WndRect, screenVertex))
		{
			D3DXVECTOR2 screenTopVertex;
			if(WorldToScreen(matrix, WorldTopPos, WndRect, screenTopVertex))
			{
				ScreenData.isActive = true;
				SpindleVertex_t &spindleVertex = ScreenData.spindleVertex;
				spindleVertex[0].x = screenTopVertex.x;
    			spindleVertex[0].y = screenTopVertex.y;
				spindleVertex[1].x = spindlePoint.x;
				spindleVertex[1].y = spindlePoint.y;

				float rectHeigh = screenVertex.y - screenTopVertex.y;

				PerspectiveVertex_t &perspectiveVertex = ScreenData.perspectiveVertex;
				perspectiveVertex[4].x = perspectiveVertex[3].x 			// 窗口高度
									= perspectiveVertex[0].x 
									= (screenTopVertex.x - rectHeigh * 0.3f);  
				perspectiveVertex[4].y = perspectiveVertex[1].y 			// 窗口宽度
									= perspectiveVertex[0].y 
									= screenTopVertex.y;                       
				perspectiveVertex[2].x = perspectiveVertex[1].x 
									= perspectiveVertex[0].x + rectHeigh * 0.6f;    // 透视框x坐标
				perspectiveVertex[2].y = perspectiveVertex[3].y 
									= perspectiveVertex[0].y + rectHeigh;           // 透视框y坐标

				HealthVertex_t &healthVertex = ScreenData.healthVertex;
				healthVertex[0].x = healthVertex[1].x = perspectiveVertex[3].x - 12.0f;
				healthVertex[0].y = perspectiveVertex[3].y + perspectiveLineWidth;
				healthVertex[1].y = perspectiveVertex[3].y;
				healthVertex[1].y -= (PlayerData.health / 100.0f) * (rectHeigh + perspectiveLineWidth);
				
				ScreenData.nameRect.left = static_cast<long>(perspectiveVertex[0].x);
				ScreenData.nameRect.top = static_cast<long>(perspectiveVertex[0].y) - fontLogicalSize;
				ScreenData.name = PlayerData.name;
			}
		}
	}
}

void CDirect3D::Resize(const HWND wndHandle)
{
	if(wndHandle == nullptr) return;

    RECT rect = { 0 };
    ::GetWindowRect(wndHandle, &rect);
    WndRect.x = static_cast<float>(rect.right - rect.left);
    WndRect.y = static_cast<float>(rect.bottom - rect.top);

    spindlePoint.x = static_cast<float>((rect.right - rect.left) * 0.5f);
    spindlePoint.y = 0;
}

void CDirect3D::DrawForScreenData()
{
	for(ScreenDataArray_t::value_type &ScreenData : ScreenDataArray)
	{
		if(ScreenData.isActive)
		{
			DrawScreenPerspectiveRect(pPerspectiveLine, ScreenData.perspectiveVertex, ScreenData.teamColor);
			DrawScreenSpindle(pSpindleLine, ScreenData.spindleVertex, ScreenData.teamColor);
			DrawScreenHealth(pHealthLine, ScreenData.healthVertex);
			DrawScreenName(pFont, ScreenData.name, ScreenData.nameRect, ScreenData.teamColor);
		}
	}
}

void CDirect3D::Render(IDirect3DDevice9 *const pDevice)
{
	CGameMemory &gameMemory = CGameMemory::GetInstance();
	if(!gameMemory.GetMatrix(matrix)) return;

	gameMemory.ReadPlayerData();

	DrawForPlayer(gameMemory);

	DrawForScreenData();

	if(nullptr == pDevice) return;

	/*

	IDirect3DVertexBuffer9 *pStreamData = nullptr;
	unsigned offsetInBytes;
	unsigned stride;
	DWORD FVF;
	HRESULT hRes = pDevice -> GetStreamSource(0, &pStreamData, &offsetInBytes, &stride);
	if(FAILED(hRes)) return;
	hRes = pDevice -> GetFVF(&FVF);
	if(FAILED(hRes)) return;
	hRes = pDevice -> SetStreamSource(0, pVertexBuffer, 0, sizeof(D3DVector));
	if(FAILED(hRes)) return;
	hRes = pDevice -> SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	if(FAILED(hRes)) return;
	hRes = pDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	if(FAILED(hRes)) return;
	hRes = pDevice -> SetStreamSource(0, pStreamData, offsetInBytes, stride);
	if(FAILED(hRes)) return;
	hRes = pDevice -> SetFVF(FVF);
	if(FAILED(hRes)) return;
	*/
}

void CDirect3D::LostResource()
{
	pFont -> OnLostDevice();
	pSpindleLine -> OnLostDevice();
	pHealthLine -> OnLostDevice();
	pPerspectiveLine -> OnLostDevice();
}

void CDirect3D::ResetResource()
{
	pPerspectiveLine -> OnResetDevice();
	pHealthLine -> OnResetDevice();
	pSpindleLine -> OnResetDevice();
	pFont -> OnResetDevice();
}

bool CDirect3D::CreateResource(IDirect3DDevice9 *const pDevice)
{
    if(pDevice == nullptr) return false;

    HRESULT hRes;

    hRes = ::D3DXCreateLine(pDevice, &pPerspectiveLine);
    if(pPerspectiveLine == nullptr || FAILED(hRes)) return false;
    pPerspectiveLine -> SetAntialias(false);
    pPerspectiveLine -> SetWidth(perspectiveLineWidth);

    hRes = ::D3DXCreateLine(pDevice, &pHealthLine);
    if(pHealthLine == nullptr || FAILED(hRes)) return false;
    pHealthLine -> SetAntialias(false);
    pHealthLine -> SetWidth(healthLineWidth);

    hRes = ::D3DXCreateLine(pDevice, &pSpindleLine);
    if(pSpindleLine == nullptr || FAILED(hRes)) return false;
    pSpindleLine -> SetAntialias(true);
    pSpindleLine -> SetWidth(spindleLineWidth);

    hRes = ::D3DXCreateFontA(
        pDevice,
        fontLogicalSize,
        0,
        FW_DONTCARE,
        D3DX_DEFAULT,
        false,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        FF_DONTCARE,
        "Arial",
        &pFont
    );
    if(pFont == nullptr || FAILED(hRes)) return false;
	/*
	DWORD pRenderState_D3DRS_LIGHTING;
	hRes = pDevice -> GetRenderState(D3DRS_LIGHTING, &pRenderState_D3DRS_LIGHTING);
	if(FAILED(hRes)) return false;
	hRes = pDevice -> SetRenderState(D3DRS_LIGHTING, false);
	if(FAILED(hRes)) return false;
	hRes = pDevice -> CreateVertexBuffer(6 * sizeof(D3DVector), 0, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_SYSTEMMEM, &pVertexBuffer, nullptr);
	if(FAILED(hRes)) return false;
	if(nullptr == pVertexBuffer) return false;
	D3DVector *pVector = nullptr;
	hRes = pVertexBuffer -> Lock(0, 0, reinterpret_cast<void **>(&pVector), 0);
	if(FAILED(hRes)) return false;

	pVector[0] = D3DVector{ 220.0f, 340.0f, 0, 0xFFFF0000};
	pVector[1] = D3DVector{ 220.0f, 140.0f, 0, 0xFFFF0000};
	pVector[2] = D3DVector{ 420.0f, 140.0f, 0, 0xFFFF0000};
	pVector[3] = D3DVector{ 220.0f, 340.0f, 0, 0xFFFF0000};
	pVector[4] = D3DVector{ 420.0f, 140.0f, 0, 0xFFFF0000};
	pVector[5] = D3DVector{ 420.0f, 340.0f, 0, 0xFFFF0000};

	hRes = pVertexBuffer -> Unlock();
	hRes = pDevice -> SetRenderState(D3DRS_LIGHTING, pRenderState_D3DRS_LIGHTING);
	*/
    return true;
}

void CDirect3D::DestroyResource()
{
	SafeRelease(pFont);
	SafeRelease(pSpindleLine);
	SafeRelease(pHealthLine);
    SafeRelease(pPerspectiveLine);
}
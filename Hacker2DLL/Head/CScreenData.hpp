#pragma once

#define WIN32_LEAN_AND_MEAN

class CDirect3D;

class ScreenData
{
private:
	bool isActive = false;
	D3DCOLOR teamColor = 0;
	D3DXVECTOR2 perspectiveVertex[5]{ {0.0f, 0.0f} };
	D3DXVECTOR2 healthVertex[2]{ {0.0f, 0.0f} };
	D3DXVECTOR2 spindleVertex[2]{ {0.0f, 0.0f} };
	const char *name = nullptr;
	RECT nameRect{ 0 };

	friend class CDirect3D;
};
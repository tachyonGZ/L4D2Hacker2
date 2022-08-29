#pragma once

#define WIN32_LEAN_AND_MEAN

class CGameMemory;
class CDirect3D;

class CPlayerData
{
	bool isActive = false;
	float worldPos[3]{ 0.0f };
	int health;
	int team;
	int type;
	const char *name;

	friend class CGameMemory;
	friend class CDirect3D;
};
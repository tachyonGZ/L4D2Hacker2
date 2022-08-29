#pragma once

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <string>
#include <memory>
#include <array>
#include <unordered_map>
#include <cstring>

#include <windows.h>
#include <d3dx9.h>

#include "CSingleton.hpp"
#include "CPlayerData.hpp"

class CDirect3D;

class CGameMemory : public CSingleton<CGameMemory>
{
private:
    //static constexpr std::int32_t matrix_offset = 0x600F9C;
	static constexpr std::int32_t matrix_offset = 0xFD83C;
    static constexpr std::int32_t player_array_offset = 0x7374F4;
    static constexpr std::int32_t player_array_element_size = 0x10;
    static constexpr std::int32_t pos_offset = 0xA0;

    static constexpr std::int32_t health_offset = 0xEC;
	static constexpr std::int32_t death_health = 0;
    static constexpr std::int32_t invalid_health = 1;

    static constexpr std::int32_t team_offset = 0xE4;
	static constexpr std::int32_t invalid_team = 0;
    static constexpr std::int32_t person_team = 2;
    static constexpr std::int32_t special_team = 3;

    static constexpr std::int32_t type_offset = 0x140;
	static constexpr std::int32_t invalid_type= 0;

	static constexpr std::int32_t draw_model_offset = 0x71F768;
	static constexpr std::int32_t normal_model = 1;
	static constexpr std::int32_t blue_line_model = 2;

	static constexpr std::int32_t person_perspective_offset = 0x7AF400;
	static constexpr std::int32_t first_person = 0;
	static constexpr std::int32_t third_person = 256;

	static constexpr std::int32_t draw_footstep_offset = 0x751D38;
	static constexpr std::int32_t hide_footstep = 0;
	static constexpr std::int32_t show_footstep = 1;
	

    std::int32_t engine_base_address = NULL;
    std::int32_t client_base_address = NULL;
	std::int32_t materialsystem_base_address = NULL;
    std::int32_t player_array_address = NULL;
    float *my_matrix_address = nullptr;
	std::int32_t *draw_model_address = nullptr;
	std::int32_t  *person_perspective_address = nullptr;
	std::int32_t *draw_footstep_address = nullptr;

	static constexpr size_t MAX_PLAYER = 32;
    static constexpr float person_heigh = 70.0f;
    static constexpr float person_width = 20.0f;
    static constexpr float special_heigh = 70.0f;
    static constexpr float special_width = 20.0f;
    static constexpr float person_zpos_offset = 5.0f;
    static constexpr float special_zpos_offset = 5.0f;

	static constexpr const char *invalid_name= "What fuck is this?";

	static const std::unordered_map<int, std::string> nameMap;

	using PlayerDataArray_t = std::array<CPlayerData, MAX_PLAYER>;
	using PerspectiveVertex_t = D3DXVECTOR2[5];
	using HealthVertex_t = D3DXVECTOR2[2];
	using SpindleVertex_t = D3DXVECTOR2[2];

	PlayerDataArray_t PlayerDataArray; 

	friend class CDirect3D;
public:
	CGameMemory() = default;
	CGameMemory(const CDirect3D&) = delete;
	CGameMemory &operator=(const CDirect3D&) = delete;

	void ReadPlayerData();
	void ToggleModelBlueLine(bool &isActive);
	void ToggleThirdPersonPerspective(bool &isActive);
	void ToggleFootStep(bool &isActive);

    bool Init();
	bool GetMatrix(float (&matrix)[4][4]);
};
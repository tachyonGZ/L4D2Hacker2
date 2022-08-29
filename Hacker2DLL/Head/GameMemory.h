#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include <windows.h>

class GameMemory
{
private:
    static constexpr DWORD matrix_offset = 0x600F9C;
    static constexpr DWORD player_array_offset = 0x7374E4;
    static constexpr DWORD player_array_element_size = 0x10;
    static constexpr DWORD pos_offset = 0x94;
    static constexpr DWORD health_offset = 0xEC;
    static constexpr DWORD team_offset = 0xE4;
    static constexpr DWORD type_offset = 0x140;

    static const std::unordered_map<int, std::string> nameMap;

    static const int GetPlayerTypeByIndex(const unsigned int index);
public:
    static DWORD engine_base_address;
    static DWORD client_base_address;
    static DWORD player_array_address;
    static float *my_matrix_address;

    static constexpr unsigned int MAX_PLAYER = 32;
    static constexpr float person_heigh = 70.0f;
    static constexpr float person_width = 20.0f;
    static constexpr float special_heigh = 70.0f;
    static constexpr float special_width = 20.0f;
    static constexpr float person_zpos_offset = 5.0f;
    static constexpr float special_zpos_offset = 5.0f;

    static constexpr int invalid_health = 0;

    static constexpr int invalid_team = 0;
    static constexpr int person_team = 2;
    static constexpr int special_team = 3;

    static constexpr int invalid_type= 0;

    static bool Init();
    static const float *GetPlayerPosByIndex(const unsigned int index);
    static const int GetPlayerHealthByIndex(const unsigned int index);
    static const int GetPlayerTeamByIndex(const unsigned int index);
    static const char *GetPlayerNameByIndex(const unsigned int index);
};
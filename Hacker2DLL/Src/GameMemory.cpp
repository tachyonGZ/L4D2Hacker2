#include "GameMemory.h"

DWORD GameMemory::engine_base_address = 0;
DWORD GameMemory::client_base_address = 0;
DWORD GameMemory::player_array_address = 0;
float *GameMemory::my_matrix_address = nullptr;

const std::unordered_map<int, std::string> GameMemory::nameMap{
    {11, "Nick"},
    {12, "Rochelle"},
    {13, "Coach"},
    {14, "Ellis"},
    {26, "Hunter"},
    {27, "Smoker"},
	{28, "Boomer"},
	{29, "Spitter"},
	{30, "Jockey"},
	{31, "Charger"},
	{32, "Tank"},
    {16777227, "Nick"},
    {16777228, "Rochelle"},
    {16777229, "Coach"},
    {16777230, "Ellis"},
};

bool GameMemory::Init()
{
    engine_base_address = reinterpret_cast<DWORD>(GetModuleHandle("engine.dll"));
    if(!engine_base_address)
    {
        return false;
    }

    client_base_address = reinterpret_cast<DWORD>(GetModuleHandle("client.dll"));
    if(!client_base_address)
    {
        return false;
    }

    player_array_address = client_base_address + player_array_offset;
    my_matrix_address = reinterpret_cast<float *>(engine_base_address + matrix_offset);

    std::cout   << "[GameMemory]engine.dll基址:" << reinterpret_cast<void *>(engine_base_address) << '\n'
                << "[GameMemory]client.dll基址:" << reinterpret_cast<void *>(client_base_address) << '\n'
                << "[GameMemory]玩家数组基址:" << reinterpret_cast<void *>(player_array_address) << '\n'
                << "[GameMemory]矩阵基址:" << reinterpret_cast<void *>(my_matrix_address) << '\n';

    return true;
}

const float *GameMemory::GetPlayerPosByIndex(const unsigned int index)
{
    DWORD pPlayer =  *(reinterpret_cast<DWORD *>(player_array_address + index * player_array_element_size));
    if(pPlayer == NULL) return nullptr;
    return reinterpret_cast<const float *>(pPlayer + pos_offset);
}

const int GameMemory::GetPlayerHealthByIndex(const unsigned int index)
{
    DWORD pPlayer = *(reinterpret_cast<DWORD *>(player_array_address + index * player_array_element_size));
    if(pPlayer == NULL) return invalid_health;
    const int *pHealth = reinterpret_cast<const int *>(pPlayer + health_offset);
    if(pHealth == nullptr)  return invalid_health;
    return *pHealth;
}

const int GameMemory::GetPlayerTeamByIndex(const unsigned int index)
{
    DWORD pPlayer = *(reinterpret_cast<DWORD *>(player_array_address + index * player_array_element_size));
    if(pPlayer == NULL) return invalid_team;
    const int *pTeam = reinterpret_cast<const int *>(pPlayer + team_offset);
    if(pTeam == nullptr)  return invalid_team;
    return *pTeam;
}

const int GameMemory::GetPlayerTypeByIndex(const unsigned int index)
{
    DWORD pPlayer = *(reinterpret_cast<DWORD *>(player_array_address + index * player_array_element_size));
    if(pPlayer == NULL) return invalid_type;
    const int *pType = reinterpret_cast<const int *>(pPlayer + type_offset);
    if(pType == nullptr)  return invalid_type;
    return *pType;
}

const char *GameMemory::GetPlayerNameByIndex(const unsigned int index)
{
    const int type = GetPlayerTypeByIndex(index);
    if(type == invalid_type){ return nullptr; }
    try{ return nameMap.at(type).c_str(); }
    catch(std::out_of_range){ return nullptr; }
}
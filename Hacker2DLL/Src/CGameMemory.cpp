#include "CGameMemory.hpp"

const std::unordered_map<int, std::string> CGameMemory::nameMap{
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

bool CGameMemory::Init()
{
    engine_base_address = reinterpret_cast<DWORD>(GetModuleHandle("engine.dll"));
    if(NULL == engine_base_address) return false;

    client_base_address = reinterpret_cast<DWORD>(GetModuleHandle("client.dll"));
    if(NULL == client_base_address) return false;

	materialsystem_base_address = reinterpret_cast<DWORD>(GetModuleHandle("materialsystem.dll"));
	if(NULL == materialsystem_base_address) return false;

    player_array_address   		= 								   client_base_address + player_array_offset;
    //my_matrix_address   		= reinterpret_cast<float *>		  (engine_base_address + matrix_offset);
	my_matrix_address   		= reinterpret_cast<float *>		  (materialsystem_base_address + matrix_offset);
	draw_model_address   		= reinterpret_cast<std::int32_t *>(client_base_address + draw_model_offset);
	person_perspective_address 	= reinterpret_cast<std::int32_t *>(client_base_address + person_perspective_offset);
	draw_footstep_address   	= reinterpret_cast<std::int32_t *>(client_base_address + draw_footstep_offset);

    std::cout   << "[CGameMemory]engine.dll基址:" << reinterpret_cast<void *>(engine_base_address) << '\n'
                << "[CGameMemory]client.dll基址:" << reinterpret_cast<void *>(client_base_address) << '\n'
                << "[CGameMemory]玩家数组基址:" << reinterpret_cast<void *>(player_array_address) << '\n'
                << "[CGameMemory]矩阵基址:" << my_matrix_address << '\n'
				<< "[CGameMemory]绘制模型模式基址" << draw_model_address << '\n'
				<< "[CGameMemory]人物视角基址" << person_perspective_address << '\n'
				<< "[CGameMemory]绘制足迹基址" << draw_footstep_address << '\n';
				
				

    return true;
}

bool CGameMemory::GetMatrix(float (&matrix)[4][4])
{
	if(nullptr == my_matrix_address) return false;
	std::memcpy(&matrix, my_matrix_address, sizeof(float) * 16);
	return true;
}

void CGameMemory::ReadPlayerData()
{
	for(size_t index = 1; index < PlayerDataArray.size(); index++)
	{
		auto &playerData = PlayerDataArray[index];
		playerData.isActive = false;

		DWORD pPlayer = *(reinterpret_cast<DWORD *>(player_array_address + index * player_array_element_size));

		if(NULL != pPlayer)	
		{
			const float *pWorldPos = reinterpret_cast<const float *>(pPlayer + pos_offset);
			std::memcpy(&playerData.worldPos, pWorldPos, sizeof(float) * 3);

			playerData.health = *reinterpret_cast<const int *>(pPlayer + health_offset);
			playerData.team = *reinterpret_cast<const int *>(pPlayer + team_offset);
			playerData.type = *reinterpret_cast<const int *>(pPlayer + type_offset);
			try{ 
				playerData.name = nameMap.at(playerData.type).c_str(); 
			}
    		catch(std::out_of_range){ 
				playerData.name = invalid_name; 
			}

			pPlayer = *(reinterpret_cast<DWORD *>(player_array_address + index * player_array_element_size));
			if(NULL != pPlayer)
			{
				playerData.isActive = true;
			}
		}
	}
} 

void CGameMemory::ToggleModelBlueLine(bool &isActive)
{
	if(draw_model_address != nullptr)
	{
		if(isActive)
		{
			*draw_model_address = blue_line_model;
		}
		else
		{
			*draw_model_address = normal_model;
		}
	}
}

void CGameMemory::ToggleThirdPersonPerspective(bool &isActive)
{
	if(person_perspective_address != nullptr)
	{
		if(isActive)
		{
			*person_perspective_address = third_person;	
		}
		else
		{
			*person_perspective_address = first_person;
		}
	}
}

void CGameMemory::ToggleFootStep(bool &isActive)
{
	if(draw_footstep_address != nullptr)
	{
		if(isActive)
		{
			*draw_footstep_address = show_footstep;
		}
		else
		{
			*draw_footstep_address = hide_footstep;
		}
	}
}
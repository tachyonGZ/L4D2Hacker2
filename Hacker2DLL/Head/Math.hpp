#pragma once

#include <cmath>

#include "CMatrix.hpp"

namespace Math
{

inline const float GetWorldDistance(const float (&pos_1)[3], const float (&pos_2)[3])
{
	return std::sqrtf( 
	  std::powf(pos_1[0] - pos_2[0], 2.f)
	+ std::powf(pos_1[1] - pos_2[1], 2.f)
	+ std::powf(pos_1[2] - pos_2[2], 2.f)
	);
}

inline const float GetScreenDistance(const float (&pos_1)[2], const float (&pos_2)[2])
{
	return std::sqrtf( 
	  std::powf(pos_1[0] - pos_2[0], 2.f)
	+ std::powf(pos_1[1] - pos_2[1], 2.f)
	);
}

}
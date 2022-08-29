#pragma once

#define WIN32_LEAN_AND_MEAN

#include "cmath"

class CDirect3DMath
{
protected:

    const bool WorldToScreen(const float(&martix)[4][4], const float (&world_pos)[3], const D3DXVECTOR2 &wnd_rect, D3DXVECTOR2 &screen_pos)
    {
        // 裁剪坐标
        float clip_pos[4];
        clip_pos[3] = world_pos[0] * martix[3][0]
                    + world_pos[1] * martix[3][1]
                    + world_pos[2] * martix[3][2]
                    			   + martix[3][3];
        if (clip_pos[3] < 0.1f) return false;
        clip_pos[1] = world_pos[0] * martix[1][0]
                    + world_pos[1] * martix[1][1]
                    + world_pos[2] * martix[1][2]
                                   + martix[1][3];
        clip_pos[0] = world_pos[0] * martix[0][0]
                    + world_pos[1] * martix[0][1]
                    + world_pos[2] * martix[0][2]
                    			   + martix[0][3];

        // NDC坐标
        float NDC_pos[2];    
        NDC_pos[0] = clip_pos[0] / clip_pos[3];
        NDC_pos[1] = clip_pos[1] / clip_pos[3];

        // 屏幕坐标
        screen_pos.x = (NDC_pos[0] + wnd_rect.x * 0.5f) + (wnd_rect.x * 0.5f * NDC_pos[0]);
        screen_pos.y = (NDC_pos[1] + wnd_rect.y * 0.5f) - (wnd_rect.y * 0.5f * NDC_pos[1]);
		return true;
    }

    const float DistanceForPointToPoint(const float (&pointPos1)[3], float (&pointPos2)[3])
    {
		return static_cast<float>(
			std::sqrt(
				  std::pow(pointPos1[0] - pointPos2[0], 2)
				+ std::pow(pointPos1[1] - pointPos2[1], 2)
				+ std::pow(pointPos1[2] - pointPos2[2], 2)
			)
		);
	}
};
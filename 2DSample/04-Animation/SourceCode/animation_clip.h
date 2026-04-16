#pragma once
#include <string>
#include <vector>
#include <Vector/vector2.hpp>

/// @brief アニメーションクリップ (1アクション専用)
struct AnimationClip
{
	std::string			filePath;
	std::vector<int>	animHandle;
	int					frameNum;		// フレーム数
	Vector2				graphicSize;	// 画像全体のサイズ
};

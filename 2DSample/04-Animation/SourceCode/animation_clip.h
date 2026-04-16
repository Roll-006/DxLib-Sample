#pragma once
#include <string>
#include <vector>
#include <Vector/vector2.hpp>

/// @brief アニメーションクリップ (1アクション専用)
struct AnimationClip
{
	std::vector<int>	animHandle;			// アニメーションハンドル

	std::string			name;				// アニメーションの名前
	std::string			filePath;			// アニメーションのファイルパス
	Vector2				originGraphicSize;	// 元画像全体のサイズ
	int					keyframeNum;		// キーフレーム数
	float				playIntervalTime;	// 再生時間の間隔
	bool				isLoop;				// ループ再生をするかどうか
};

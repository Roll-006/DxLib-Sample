#pragma once
#include <math.hpp>

/// @brief アニメーションクリップ (1アクション専用)
struct AnimationClip
{
	AnimationClip(const std::string& name, const int keyframeNum, const float playIntervalTime, const bool isLoop, const bool canTransitionToSelf) : 
		graphic				(),
		name				(name),
		keyframeNum			(keyframeNum),
		playIntervalTime	(playIntervalTime),
		isLoop				(isLoop),
		canTransitionToSelf	(canTransitionToSelf)
	{ }

	AnimationClip() :
		graphic				(),
		name				(""),
		keyframeNum			(0),
		playIntervalTime	(0.0f),
		isLoop				(false),
		canTransitionToSelf	(false)
	{ }

	std::vector<std::shared_ptr<Graphic>> graphic;	// アニメーションシートを分割した画像を格納
	std::string	name;					// アニメーションの名前
	int			keyframeNum;			// キーフレーム数
	float		playIntervalTime;		// 再生時間の間隔
	bool		isLoop;					// ループ再生をするかどうか
	bool		canTransitionToSelf;	// 自身のアニメーションから遷移を許可するかどうか
};

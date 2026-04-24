#include <DxLib.h>
#include <unordered_map>
#include <Vector/vector2.hpp>
#include "time.h"
#include "transform.h"
#include "animator.h"

Animator::Animator(Transform& transform) :
	_playTimer		(0.0f),
	_keyframeIndex	(0),
	_currentAnimName(""),
	_transform		(transform)
{

}

void Animator::Update()
{
	PlayAnim();
}

void Animator::Draw() const
{
	const auto& clip = _clips.at(_currentAnimName);

	DrawRotaGraph3(
		static_cast<int>(_transform.position.x),
		static_cast<int>(_transform.position.y),
		static_cast<int>(clip.originGraphicSize.x / clip.keyframeNum * 0.5f),
		static_cast<int>(clip.originGraphicSize.y * 0.5f),
		static_cast<double>(_transform.scale.x),
		static_cast<double>(_transform.scale.y),
		_transform.rotation,
		clip.animHandle.at(_keyframeIndex),
		TRUE,
		FALSE,
		FALSE);
}

void Animator::LoadAnim(const AnimationClip& animClip)
{
	auto clip = animClip;

	// アニメーションハンドルのサイズを指定
	clip.animHandle.resize(clip.keyframeNum);

	// 画像を分割して読み込む
	// MEMO : 画像が別々の場合はサイトでの合成をおすすめします！ [https://web.save-editor.com/pic/picture_split_tool.html]
	auto result = LoadDivGraph(clip.filePath.c_str(), clip.keyframeNum, clip.keyframeNum, 1, clip.originGraphicSize.x / clip.keyframeNum, clip.originGraphicSize.y, clip.animHandle.data());

	_clips[clip.name] = clip;
}

void Animator::AttachAnim(const std::string& animName)
{
	if (_clips.contains(_currentAnimName))
	{
		const auto& clip = _clips.at(_currentAnimName);

		// 自身からの遷移を許可しない場合はアタッチを許可しない
		if (clip.name == animName && !clip.canTransitionToSelf) { return; }
	}

	_currentAnimName	= animName;
	_playTimer			= 0.0f;
	_keyframeIndex		= 0;
}

void Animator::PlayAnim()
{
	const auto& clip = _clips.at(_currentAnimName);

	// ループしないアニメーションの再生が終了していた場合、早期return
	if (!clip.isLoop && _keyframeIndex == clip.keyframeNum - 1) { return; }

	// 一定間隔で次のキーフレームに移行
	_playTimer += Time::GetInstance().GetDeltaTime();
	if (_playTimer >= clip.playIntervalTime)
	{
		_playTimer = 0.0f;

		// ループするアニメーションのキーフレームが最大値に到達したら、アニメーションをループ
		++_keyframeIndex;
		if (_keyframeIndex >= clip.keyframeNum)
		{
			if (clip.isLoop)
			{
				_keyframeIndex = 0;
			}
			else
			{
				_keyframeIndex = clip.keyframeNum - 1;
			}

			// 1行でも書ける！ (三項演算子)
			// _keyframeIndex = clip.isLoop ? 0 : clip.keyframeNum - 1;
		}
	}
}

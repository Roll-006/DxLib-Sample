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

Animator::~Animator()
{

}

void Animator::Update()
{
	PlayAnim();
}

void Animator::Draw() const
{
	DrawRotaGraph3(
		static_cast<int>(_transform.position.x),
		static_cast<int>(_transform.position.y),
		static_cast<int>(_clips.at(_currentAnimName).loadGraphicSize.x / _clips.at(_currentAnimName).keyframeNum * 0.5f),
		static_cast<int>(_clips.at(_currentAnimName).loadGraphicSize.y * 0.5f),
		static_cast<double>(_transform.scale.x),
		static_cast<double>(_transform.scale.y),
		_transform.rotation,
		_clips.at(_currentAnimName).animHandle.at(_keyframeIndex),
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
	auto result = LoadDivGraph(clip.filePath.c_str(), clip.keyframeNum, clip.keyframeNum, 1, clip.loadGraphicSize.x / clip.keyframeNum, clip.loadGraphicSize.y, clip.animHandle.data());

	_clips[clip.name] = clip;
}

void Animator::AttachAnim(const std::string& animName)
{
	_currentAnimName	= animName;
	_playTimer			= 0.0f;
	_keyframeIndex		= 0.0f;
}

void Animator::PlayAnim()
{
	// 一定間隔で次のキーフレームに以降
	_playTimer += Time::GetInstance().GetDeltaTime();
	if (_playTimer >= _clips.at(_currentAnimName).playIntervalTime)
	{
		_playTimer = 0.0f;

		// キーフレームが最大値に到達したら、アニメーションをループ
		++_keyframeIndex;
		if (_keyframeIndex >= _clips.at(_currentAnimName).keyframeNum)
		{
			_keyframeIndex = 0;
		}
	}
}

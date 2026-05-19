#include <unordered_map>
#include <math.hpp>
#include "time.h"
#include "transform.h"
#include "graphic.h"
#include "graphic_renderer.h"
#include "animator.h"

Animator::Animator(Transform& transform, GraphicRenderer& graphicRenderer) :
	_playTimer		(0.0f),
	_keyframeIndex	(0),
	_currentAnimName(""),
	_transform		(transform),
	_graphicRenderer(graphicRenderer)
{

}

void Animator::Update()
{
	PlayAnim();
}

void Animator::LoadAnim(const std::string& filePath, const AnimationClip& animClip)
{
	_clips[animClip.name] = animClip;
	auto& clip = _clips.at(animClip.name);

	// アニメーションハンドルのサイズを指定
	std::vector<int> animHandles(clip.keyframeNum);

	// アニメーションシートのハンドル及びサイズを取得
	const auto graphicHandle = LoadGraph(filePath.c_str());
	auto graphicSize = Vector2::Zero;
	GetGraphSizeF(graphicHandle, &graphicSize.x, &graphicSize.y);

	// 画像を分割して読み込む
	// MEMO : 画像が別々の場合はサイトでの合成をおすすめします！ [https://web.save-editor.com/pic/picture_split_tool.html]
	LoadDivGraph(filePath.c_str(), clip.keyframeNum, clip.keyframeNum, 1, static_cast<int>(graphicSize.x / clip.keyframeNum), static_cast<int>(graphicSize.y), animHandles.data());

	for (const auto animHnadle : animHandles)
	{
		clip.graphic.emplace_back(std::make_shared<Graphic>(animHnadle));
	}
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

	_graphicRenderer.SetGraphic(_clips.at(_currentAnimName).graphic.at(_keyframeIndex));
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

			// MEMO : 1行でも書ける！ (三項演算子)
			// _keyframeIndex = clip.isLoop ? 0 : clip.keyframeNum - 1;
		}

		// rendererに現在のキーフレームの画像を設定する
		_graphicRenderer.SetGraphic(clip.graphic.at(_keyframeIndex));
	}
}

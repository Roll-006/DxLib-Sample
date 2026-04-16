#include <DxLib.h>
#include <unordered_map>
#include "time.h"
#include "animator.h"

Animator::Animator() : 
	_playTimer(0.0f)
{

}

Animator::~Animator()
{

}

void Animator::PlayAnim()
{
	_playTimer += Time::GetInstance().GetDeltaTime();
	
}

void Animator::LoadAnim(AnimationClip& clip)
{
	clip.animHandle.resize(clip.frameNum);

	LoadDivGraph(clip.filePath.c_str(), clip.frameNum, clip.frameNum, 0, clip.graphicSize.x / clip.frameNum, clip.graphicSize.y, clip.animHandle.data());
}

void Animator::Draw() const
{
	//DrawRotaGraph3();
}

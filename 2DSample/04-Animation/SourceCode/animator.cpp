#include <DxLib.h>
#include <unordered_map>
#include "animator.h"

Animator::Animator()
{

}

Animator::~Animator()
{

}

void Animator::PlayAnim()
{

}

void Animator::LoadAnim(AnimationClip& clip)
{
	clip.animHandle.resize(clip.frameNum);

	LoadDivGraph(clip.filePath.c_str(), clip.frameNum, clip.frameNum, 0, clip.graphicSize.x / clip.frameNum, clip.graphicSize.y, clip.animHandle.data());
}

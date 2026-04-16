#pragma once
#include "animation_clip.h"

class Animator
{
public:
	Animator();
	~Animator();

	void PlayAnim();
	void LoadAnim(AnimationClip& clip);

private:
	std::unordered_map<int, AnimationClip> _clips;
};

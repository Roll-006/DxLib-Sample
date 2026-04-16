#pragma once
#include "animation_clip.h"

class Animator
{
public:
	Animator();
	~Animator();

	void PlayAnim();
	void LoadAnim(AnimationClip& clip);
	void Draw() const;

private:
	std::unordered_map<int, AnimationClip> _clips;
	float _playTimer;
};

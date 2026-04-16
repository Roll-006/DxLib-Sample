#pragma once
#include "animation_clip.h"

class Animator
{
public:
	Animator(Transform& transform);
	~Animator();

	void Update();
	void Draw() const;

	void LoadAnim(const AnimationClip& animClip);
	void AttachAnim(const std::string& animName);

private:
	void PlayAnim();

private:
	std::unordered_map<std::string, AnimationClip> _clips;
	float _playTimer;
	int _keyframeIndex;
	std::string _currentAnimName;
	Transform& _transform;
};

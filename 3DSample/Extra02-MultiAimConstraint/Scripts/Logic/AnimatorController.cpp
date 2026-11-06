#include "AnimatorController.h"
#include "../Core/time.h"

AnimatorController::AnimatorController() : 
    _blendRate(1.f)
{
    
}

void AnimatorController::Initialize(const ModelData& modelData)
{
    _modelData = modelData;
    AttachAnimation(_defaultAnimationName);
}

void AnimatorController::UpdateAnimation()
{
    PlayAnimation(_currentAnimation, true);
    PlayAnimation(_previousAnimation, false);
    BlendAnimation();
}

void AnimatorController::AttachAnimation(const std::string& attachAnimationName)
{
    if (_currentAnimation.animationName == attachAnimationName) return;
    if (_blendRate < 1.f) return;
    
    auto& attachClip = _animationClips.at(attachAnimationName); 
    
    if (_previousAnimation.attachIndex > 0)
    {
        DetachAnimation(_previousAnimation);
    }
        
    // Current ➡ Prev
    _previousAnimation = _currentAnimation;

    // Next ➡ Current
    _currentAnimation.animationName = attachAnimationName;
    _currentAnimation.attachIndex = MV1AttachAnim(_modelData.modelHandle, attachClip.animationIndex, attachClip.animationHandle, TRUE);
    attachClip.totalTime = MV1GetAttachAnimTotalTime(_modelData.modelHandle, _currentAnimation.attachIndex);
    
    _blendRate = _previousAnimation.attachIndex > -1 ? 0.f : 1.f;
}

void AnimatorController::DetachAnimation(AttachAnimationData& attachAnimation)
{
    if (attachAnimation.attachIndex < 0) return;
    
    MV1DetachAnim(_modelData.modelHandle, attachAnimation.attachIndex);
    attachAnimation.attachIndex = -1;
}

void AnimatorController::PlayAnimation(AttachAnimationData& attachAnimation, bool isCurrentAnimation) const
{
    if (attachAnimation.attachIndex < 0) return;

    const auto clip = _animationClips.at(attachAnimation.animationName);
    const auto currentBlendRate = isCurrentAnimation ? _blendRate : 1.f - _blendRate;
    const auto speed = clip.speed * Time::GetInstance().GetDeltaTime();

    attachAnimation.currentTime += speed;
    if (clip.isLooping && attachAnimation.currentTime > clip.totalTime)
    {
        attachAnimation.currentTime = 0.f;
    }
    
    MV1SetAttachAnimTime(_modelData.modelHandle, attachAnimation.attachIndex, attachAnimation.currentTime);
    MV1SetAttachAnimBlendRate(_modelData.modelHandle, attachAnimation.attachIndex, currentBlendRate);
}

void AnimatorController::BlendAnimation()
{
    if (!_animationClips.contains(_previousAnimation.animationName)) return;
    
    const auto transitionDuration = _animationClips.at(_currentAnimation.animationName).transitionDuration * Time::GetInstance().GetDeltaTime();
    _blendRate = std::min(_blendRate + transitionDuration, 1.f);
}
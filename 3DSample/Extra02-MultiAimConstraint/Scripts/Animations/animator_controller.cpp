#include "animator_controller.h"
#include "../Core/time.h"

using namespace Core;
using namespace Animations;
using namespace Model;

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
    Translate();
    PlayAnimation(_currentAnimation, true);
    PlayAnimation(_previousAnimation, false);
    BlendAnimation();
}

void AnimatorController::SetFloat(const std::string& name, float value)
{
    if (!_animationParameters.floats.contains(name)) return;
    _animationParameters.floats.at(name) = value;
}

void AnimatorController::SetBool(const std::string& name, bool value)
{
    if (!_animationParameters.booleans.contains(name)) return;
    _animationParameters.booleans.at(name) = value;
}

void AnimatorController::SetInteger(const std::string& name, bool value)
{
    if (!_animationParameters.integers.contains(name)) return;
    _animationParameters.integers.at(name) = value;
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

void AnimatorController::DetachAnimation(AttachAnimationData& attachAnimation) const
{
    if (attachAnimation.attachIndex < 0) return;
    
    MV1DetachAnim(_modelData.modelHandle, attachAnimation.attachIndex);
    attachAnimation.attachIndex = -1;
}

void AnimatorController::Translate()
{
    for (const auto& animationTransition : _animationTransitions)
    {
        if (animationTransition.originAnimationName != _currentAnimation.animationName) continue;

        if (!CheckFloatTransition(animationTransition)) continue;
        if (!CheckBooleanTransition(animationTransition)) continue;
        if (!CheckIntegerTransition(animationTransition)) continue;
                
        _currentTransition = animationTransition;
        AttachAnimation(_currentTransition.targetAnimationName);
        return;
    }
}

bool AnimatorController::CheckFloatTransition(const AnimationTransition& checkAnimation) const
{
    for (const auto& [name, floatTransition] : checkAnimation.floats)
    {
        if (!_animationParameters.floats.contains(name)) return false;
            
        switch (floatTransition.type)
        {
        case FloatTransitionType::kGreater:
            if (_animationParameters.floats.at(name) <= floatTransition.value) return false;
            break;
                
        case FloatTransitionType::kLess:
            if (_animationParameters.floats.at(name) >= floatTransition.value) return false;
            break;
        }
    }
    
    return true;
}

bool AnimatorController::CheckBooleanTransition(const AnimationTransition& checkAnimation) const
{
    for (const auto& [name, booleanTransition] : checkAnimation.booleans)
    {
        if (!_animationParameters.booleans.contains(name)) return false;
            
        const auto expected = booleanTransition.type == BooleanTransitionType::kTrue;
        if (_animationParameters.booleans.at(name) != expected) return false;
    }
    
    return true;
}

bool AnimatorController::CheckIntegerTransition(const AnimationTransition& checkAnimation) const
{
    for (const auto& [name, integerTransition] : checkAnimation.integers)
    {
        if (!_animationParameters.integers.contains(name)) return false;
            
        switch (integerTransition.type)
        {
        case IntegerTransitionType::kGreater:
            if (_animationParameters.integers.at(name) <= integerTransition.value) return false;
            break;
                
        case IntegerTransitionType::kLess:
            if (_animationParameters.integers.at(name) >= integerTransition.value) return false;
            break;
            
        case IntegerTransitionType::kEquals:
            if (_animationParameters.integers.at(name) != integerTransition.value) return false;
            break;
                
        case IntegerTransitionType::kNotEqual:
            if (_animationParameters.integers.at(name) == integerTransition.value) return false;
            break;
        }
    }
    
    return true;
}

void AnimatorController::PlayAnimation(AttachAnimationData& attachAnimation, bool isCurrentAnimation) const
{
    if (attachAnimation.attachIndex < 0) return;

    const auto clip = _animationClips.at(attachAnimation.animationName);
    const auto currentBlendRate = isCurrentAnimation ? _blendRate : 1.f - _blendRate;
    const auto speed = clip.speed * Time::I().GetDeltaTime();

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
    
    const auto transitionDuration = _currentTransition.transitionDuration * Time::I().GetDeltaTime();
    _blendRate = std::min(_blendRate + transitionDuration, 1.f);
}
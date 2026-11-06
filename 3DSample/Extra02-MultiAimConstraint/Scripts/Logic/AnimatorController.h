#pragma once
#include "../References/model_data.h"
#include "../References/animation_clip.h"
#include "../References/attach_animation_data.h"

class AnimatorController
{
public:
    AnimatorController();
    
    void Initialize(const ModelData& modelData);
    void UpdateAnimation();
    
    // TODO: 簡易的アタッチ
    void AttachAnimation(const std::string& attachAnimationName);
    
private:
    void DetachAnimation(AttachAnimationData& attachAnimation);
    void PlayAnimation(AttachAnimationData& attachAnimation, bool isCurrentAnimation) const;
    void BlendAnimation();

private:
    ModelData _modelData;
    std::string _defaultAnimationName;
    AttachAnimationData _currentAnimation;
    AttachAnimationData _previousAnimation;
    float _blendRate;
    std::unordered_map<std::string, AnimationClip> _animationClips;
    
    friend void from_json	(const nlohmann::json& json, AnimatorController& animatorController);
    friend void to_json		(nlohmann::json& json, const AnimatorController& animatorController);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, AnimatorController& animatorController)
{
    json.at("defaultAnimationName").get_to(animatorController._defaultAnimationName);
    json.at("animationClips").      get_to(animatorController._animationClips);
}

inline void to_json(nlohmann::json& json, const AnimatorController& animatorController)
{
    json =
    {
        { "defaultAnimationName",   animatorController._defaultAnimationName },
        { "animationClips",         animatorController._animationClips },
    };
}
#pragma endregion
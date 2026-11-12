#pragma once
#include "../Model/model_data.h"
#include "animation_clip.h"
#include "attach_animation_data.h"
#include "animation_parameters.h"
#include "animation_transition.h"

namespace DxLibSample::Animations
{
    class AnimatorController
    {
    public:
        AnimatorController();
    
        void Initialize(const Model::ModelData& modelData);
        void UpdateAnimation();
    
        void SetFloat(const std::string& name, float value);
        void SetBool(const std::string& name, bool value);
        void SetInteger(const std::string& name, bool value);
    
        AnimationParameters GetAnimationParameters() const { return _animationParameters; }

    private:
        void AttachAnimation(const std::string& attachAnimationName);
        void DetachAnimation(AttachAnimationData& attachAnimation) const;
        void Translate();
        bool CheckFloatTransition(const AnimationTransition& checkAnimation) const;
        bool CheckBooleanTransition(const AnimationTransition& checkAnimation) const;
        bool CheckIntegerTransition(const AnimationTransition& checkAnimation) const;
        void PlayAnimation(AttachAnimationData& attachAnimation, bool isCurrentAnimation) const;
        void BlendAnimation();

    private:
        Model::ModelData _modelData;
        std::string _defaultAnimationName;
        AttachAnimationData _currentAnimation;
        AttachAnimationData _previousAnimation;
        float _blendRate;
        AnimationParameters _animationParameters;
        std::vector<AnimationTransition> _animationTransitions;
        AnimationTransition _currentTransition;
        std::unordered_map<std::string, AnimationClip> _animationClips;
    
        friend void from_json	(const nlohmann::json& json, AnimatorController& animatorController);
        friend void to_json		(nlohmann::json& json, const AnimatorController& animatorController);
    };

    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, AnimatorController& animatorController)
    {
        json.at("defaultAnimationName").get_to(animatorController._defaultAnimationName);
        json.at("animationParameters"). get_to(animatorController._animationParameters);
        json.at("animationTransitions").get_to(animatorController._animationTransitions);
        json.at("animationClips").      get_to(animatorController._animationClips);
    }

    inline void to_json(nlohmann::json& json, const AnimatorController& animatorController)
    {
        json =
        {
            { "defaultAnimationName",   animatorController._defaultAnimationName },
            { "animationParameters",    animatorController._animationParameters },
            { "animationTransitions",   animatorController._animationTransitions },
            { "animationClips",         animatorController._animationClips },
        };
    }
    #pragma endregion
}
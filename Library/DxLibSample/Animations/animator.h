#pragma once
#include "../component.h"
#include "../Model/model_data.h"
#include "animator_controller.h"

namespace DxLibSample::Animations
{
    class Animator final : public Component
    {
    public:
        Animator();

        void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
        void Start()		override;
        void Update()       override;
        void LateUpdate()   override;

        void Deserialize(const nlohmann::json& json) override;
    
        void SetFloat(const std::string& name, float value);
        void SetBool(const std::string& name, bool value);
        void SetInteger(const std::string& name, int value);
        void SetTrigger(const std::string& name);
        void ResetTrigger(const std::string& name);
    
        [[nodiscard]] Model::ModelData GetModelData() const { return _modelData; }
    
    private:
        Model::ModelData _modelData;
        bool _isApplyRootMotion;
        std::string _animationRootName;
        int _animationRootIndex;
        AnimatorController _animatorController;
        std::weak_ptr<Transform> _transform;

        friend void from_json	(const nlohmann::json& json, Animator& animator);
        friend void to_json		(nlohmann::json& json, const Animator& animator);
    };

    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, Animator& animator)
    {
        json.at("enabled").             get_to(animator._enabled);
        json.at("isApplyRootMotion").   get_to(animator._isApplyRootMotion);
        json.at("animationRootName").   get_to(animator._animationRootName);
        json.at("animatorController").  get_to(animator._animatorController);
    }

    inline void to_json(nlohmann::json& json, const Animator& animator)
    {
        json =
        {
            { "enabled",            animator._enabled },
            { "isApplyRootMotion",  animator._isApplyRootMotion },
            { "animationRootName",  animator._animationRootName },
            { "animatorController", animator._animatorController },
        };
    }
    #pragma endregion
}
#pragma once
#include "../Core/component.h"
#include "../References/model_data.h"
#include "../Logic/AnimatorController.h"

class Animator final : public Component
{
public:
    Animator();
    ~Animator() override;

    void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
    void Update()       override;
    void LateUpdate()   override;
    void Render() const override;

    void Deserialize(const nlohmann::json& json) override;
    
    ModelData GetModelData() const { return _modelData; }
    
private:
    ModelData _modelData;
    AnimatorController _animatorController;

    friend void from_json	(const nlohmann::json& json, Animator& animator);
    friend void to_json		(nlohmann::json& json, const Animator& animator);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, Animator& animator)
{
    json.at("enabled").             get_to(animator._enabled);
    json.at("animatorController").  get_to(animator._animatorController);
}

inline void to_json(nlohmann::json& json, const Animator& animator)
{
    json =
    {
        { "enabled",            animator._enabled },
        { "animatorController", animator._animatorController },
    };
}
#pragma endregion
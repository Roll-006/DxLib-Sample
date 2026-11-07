#pragma once
#include "rig_constraint.h"
#include "../../Core/component.h"

namespace Core
{
    class GameObject;
}

namespace Animations::Rigging
{
    class Rig final : public Core::Component
    {
    public:
        Rig();

        void Initialize(const std::shared_ptr<Core::GameObject>& gameObject) override;
        void Start()		override;
        void Update()       override;
        void LateUpdate()   override;
        void Render() const override;

        void Deserialize(const nlohmann::json& json) override;
        
        [[nodiscard]] float GetWeight() const { return _weight; }
        
        void SetWeight(float weight);

    private:
        float _weight;
        std::vector<std::shared_ptr<RigConstraint>> _rigConstraints;
    
        friend void from_json	(const nlohmann::json& json, Rig& rig);
        friend void to_json		(nlohmann::json& json, const Rig& rig);
    };

    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, Rig& rig)
    {
        json.at("enabled").	get_to(rig._enabled);
        json.at("weight").	get_to(rig._weight);
    }

    inline void to_json(nlohmann::json& json, const Rig& rig)
    {
        json =
        {
            { "enabled",	rig._enabled },
            { "weight",	    rig._weight },
        };
    }
    #pragma endregion
}
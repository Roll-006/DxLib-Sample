#pragma once
#include "../Core/component.h"

namespace Core
{
    class GameObject;
}

namespace Animations
{
    class Animator;
}

namespace Animations::Rigging
{
    class Rig;
}

namespace Components
{
    class Shooter final : public Core::Component
    {
    public:
        Shooter();

        void Initialize(const std::shared_ptr<Core::GameObject>& gameObject) override;
        void Start()		override;
        void Update()       override;
        void LateUpdate()   override;
        void Render() const override;

        void Deserialize(const nlohmann::json& json) override;
    
    private:
        static constexpr float kAimFollowSpeed = 6.f;
        
        bool _isAiming;
        std::shared_ptr<Animations::Animator> _animator;
        std::shared_ptr<Animations::Rigging::Rig> _rig;
        float _aimWeight;

        friend void from_json	(const nlohmann::json& json, Shooter& shooter);
        friend void to_json		(nlohmann::json& json, const Shooter& shooter);
    };

    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, Shooter& shooter)
    {
        json.at("enabled").get_to(shooter._enabled);
    }

    inline void to_json(nlohmann::json& json, const Shooter& shooter)
    {
        json =
        {
            { "enabled", shooter._enabled },
        };
    }
    #pragma endregion
}
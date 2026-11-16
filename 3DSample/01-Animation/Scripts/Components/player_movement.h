#pragma once
#include <DxLibSample/component.h>

namespace DxLibSample
{
    class GameObject;
    
}

namespace DxLibSample::Animations
{
    class Animator;
    
}

namespace DxLibSample::Animations::Rigging
{
    class Rig;
}

namespace Components
{
    class PlayerMovement final : public DxLibSample::Component
    {
    public:
        PlayerMovement();

        void Initialize(const std::shared_ptr<DxLibSample::GameObject>& gameObject) override;
        void Start()		override;
        void Update()       override;
        void LateUpdate()   override;

        void Deserialize(const nlohmann::json& json) override;
        
    private:
        void Move();
        void Sprint();
        void Jump() const;
        void Rotate() const;
    
    private:
        static constexpr float kAimFollowSpeed = 6.f;
        
        bool _isMoving;
        bool _isSprinting;
        Vector3 _moveDirection;
        std::shared_ptr<DxLibSample::Transform> _cameraTransform;
        std::weak_ptr<DxLibSample::Transform> _transform;
        std::shared_ptr<DxLibSample::Animations::Animator> _animator;

        friend void from_json	(const nlohmann::json& json, PlayerMovement& shooter);
        friend void to_json		(nlohmann::json& json, const PlayerMovement& shooter);
    };

    #pragma region from / to JSON
    inline void from_json(const nlohmann::json& json, PlayerMovement& shooter)
    {
        json.at("enabled").get_to(shooter._enabled);
    }

    inline void to_json(nlohmann::json& json, const PlayerMovement& shooter)
    {
        json =
        {
            { "enabled", shooter._enabled },
        };
    }
    #pragma endregion
}
#include <json_loader.hpp>
#include <math.hpp>
#include <DxLibSample/transform.h>
#include <DxLibSample/component_factory.h>
#include "player_movement.h"
#include <DxLibSample/Animations/animator.h>
#include <DxLibSample/game_time.h>
#include <DxLibSample/Input/keyboard.h>
#include <DxLibSample/scene.h>

using namespace DxLibSample;
using namespace DxLibSample::Input;
using namespace DxLibSample::Animations;
using namespace DxLibSample::Animations::Rigging;
using namespace Components;

namespace
{
    const bool registered = []()
    {
        ComponentFactory::Register("PlayerMovement", [](GameObject& obj) { return obj.AddComponent<PlayerMovement>(); });
        return true;
    }();
}

PlayerMovement::PlayerMovement() : 
    _isMoving(false)
{
    
}

void PlayerMovement::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
    _gameObject = gameObject;
    _transform = GetTransform();
    _animator = GetComponent<Animator>();
}

void PlayerMovement::Start()
{
    _cameraTransform = _gameObject.lock()->GetScene()->Find("MainCamera");
}

void PlayerMovement::Update()
{
    if (!_enabled) return;
    
    Move();
    Sprint();
    Jump();
    Rotate();
}

void PlayerMovement::LateUpdate()
{
    if (!_enabled) return;
}

void PlayerMovement::Deserialize(const nlohmann::json& json)
{
    from_json(json, *this);
}

void PlayerMovement::Move()
{
    const auto keyboard = Keyboard::I();

    auto inputAxis = Vector2::Zero;
    if (keyboard.IsPressed(KEY_INPUT_W)) inputAxis.y += 1.f;
    if (keyboard.IsPressed(KEY_INPUT_S)) inputAxis.y -= 1.f;
    if (keyboard.IsPressed(KEY_INPUT_A)) inputAxis.x -= 1.f;
    if (keyboard.IsPressed(KEY_INPUT_D)) inputAxis.x += 1.f;

    auto cameraForward = _cameraTransform->GetForward();
    cameraForward.y = 0.f;
    cameraForward.Normalize();

    auto cameraRight = _cameraTransform->GetRight();
    cameraRight.y = 0.f;
    cameraRight.Normalize();

    auto targetDirection = cameraRight * inputAxis.x + cameraForward * inputAxis.y;
    targetDirection.Normalize();
    _moveDirection = MoveTowards(_moveDirection, targetDirection, 5.f * GameTime::I().GetDeltaTime());
    
    _isMoving = _moveDirection.LengthSquared() > Math::kEpsilon || targetDirection.LengthSquared() > Math::kEpsilon;
    
    _animator->SetBool("IsMoving", _isMoving);
}

void PlayerMovement::Sprint()
{
    if (!_isMoving)
    {
        _isSprinting = false;
        return;
    }
    
    if (Keyboard::I().WasPressedThisFrame(KEY_INPUT_LSHIFT))
    {
        _isSprinting = !_isSprinting;
    }
    
    _animator->SetBool("IsSprinting", _isSprinting);
}

void PlayerMovement::Jump() const
{
    if (Keyboard::I().WasPressedThisFrame(KEY_INPUT_SPACE))
    {
        _animator->SetTrigger("Jump");
    }
}

void PlayerMovement::Rotate() const
{
    if (!_isMoving) return;

    const auto transform = _transform.lock();
    const auto yaw = atan2f(_moveDirection.x, _moveDirection.z);
    const auto target = Quaternion::CreateFromAxisAngle(Vector3::Up, yaw);

    const auto result = Quaternion::Slerp(transform->GetLocalRotation(), target, 5.f * GameTime::I().GetDeltaTime());
    transform->SetLocalRotation(result);
}
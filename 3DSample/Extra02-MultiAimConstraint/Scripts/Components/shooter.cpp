#include <json_loader.hpp>
#include <math.hpp>
#include "../Core/transform.h"
#include "../Core/component_factory.h"
#include "shooter.h"
#include "../Animations/animator.h"
#include "../Animations/Rigging/rig.h"
#include "../Core/time.h"
#include "../Input/keyboard.h"

using namespace Core;
using namespace Input;
using namespace Animations;
using namespace Animations::Rigging;
using namespace Components;

namespace
{
    const bool registered = []()
    {
        ComponentFactory::Register("Shooter", [](GameObject& obj) { return obj.AddComponent<Shooter>(); });
        return true;
    }();
}

Shooter::Shooter() : 
    _isAiming(false)
{
    
}

void Shooter::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
    _gameObject = gameObject;
    _animator = GetComponent<Animator>();
}

void Shooter::Start()
{
    _rig = GetComponentInChildren<Rig>();
}

void Shooter::Update()
{
    if (!_enabled) return;
    
    if (Keyboard::I().WasPressedThisFrame(KEY_INPUT_SPACE)) _isAiming = !_isAiming;
    
    _animator->SetBool("IsAiming", _isAiming);
    
    const auto targetWeight = _isAiming ? 1.f : 0.f;
    _aimWeight = std::clamp(std::lerp(_aimWeight, targetWeight,  kAimFollowSpeed * Time::I().GetDeltaTime()), 0.f, 1.f);
    _rig->SetWeight(_aimWeight);
}

void Shooter::LateUpdate()
{
    if (!_enabled) return;
}

void Shooter::Render() const
{
    if (!_enabled) return;
    
    DrawFormatString(0, 120, 0xffffff, "SPACE : Idle / Aim");
}

void Shooter::Deserialize(const nlohmann::json& json)
{
    from_json(json, *this);
}
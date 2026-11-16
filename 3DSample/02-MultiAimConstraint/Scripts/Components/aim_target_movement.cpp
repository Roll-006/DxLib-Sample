#include <json_loader.hpp>
#include <math.hpp>
#include <DxLibSample/transform.h>
#include <DxLibSample/component_factory.h>
#include "aim_target_movement.h"
#include <DxLibSample/Input/keyboard.h>
#include <DxLibSample/game_time.h>
#include <DxLibSample/scene.h>

using namespace DxLibSample;
using namespace DxLibSample::Input;
using namespace Components;

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("AimTargetMovement", [](GameObject& obj) { return obj.AddComponent<AimTargetMovement>(); });
		return true;
	}();
}

AimTargetMovement::AimTargetMovement() : 
	_moveSpeed(0.f)
{
	
}

void AimTargetMovement::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;
	_transform = GetTransform();
}

void AimTargetMovement::Start()
{
	_cameraTransform = _gameObject.lock()->GetScene()->Find("MainCamera");
}

void AimTargetMovement::Update()
{
	if (!_enabled) return;

	Move();
	
	DrawSphere3D(ToDxLibVector(_transform.lock()->GetWorldPosition()), 0.5f, 8, 0xffffff, 0xffffff, TRUE);

	DrawFormatString(0,   0, 0xffffff, "→ : 右へ移動");
	DrawFormatString(0,  20, 0xffffff, "← : 左へ移動");
	DrawFormatString(0,  40, 0xffffff, "↑ : 上へ移動");
	DrawFormatString(0,  60, 0xffffff, "↓ : 下へ移動");
	DrawFormatString(0,  80, 0xffffff, "SHIFT + ↑ : 奥へ移動");
	DrawFormatString(0, 100, 0xffffff, "SHIFT + ↓ : 手前へ移動");
}

void AimTargetMovement::LateUpdate()
{
	if (!_enabled) return;
}

void AimTargetMovement::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}

void AimTargetMovement::Move() const
{
	const auto keyboard = Keyboard::I();
	
	auto inputAxis = Vector3::Zero;
	if (keyboard.IsPressed(KEY_INPUT_RIGHT)) inputAxis.x += 1.f;
	if (keyboard.IsPressed(KEY_INPUT_LEFT)) inputAxis.x -= 1.f;
	if (keyboard.IsPressed(KEY_INPUT_UP) && keyboard.IsPressed(KEY_INPUT_LSHIFT)) inputAxis.z += 1.f;
	else if (keyboard.IsPressed(KEY_INPUT_UP)) inputAxis.y += 1.f;
	if (keyboard.IsPressed(KEY_INPUT_DOWN) && keyboard.IsPressed(KEY_INPUT_LSHIFT)) inputAxis.z -= 1.f;
	else if (keyboard.IsPressed(KEY_INPUT_DOWN)) inputAxis.y -= 1.f;
	
	auto cameraForward = _cameraTransform->GetForward();
	cameraForward.y = 0.f;
	cameraForward.Normalize();

	auto cameraRight = _cameraTransform->GetRight();
	cameraRight.y = 0.f;
	cameraRight.Normalize();

	const auto moveDirection = cameraRight * inputAxis.x + cameraForward * inputAxis.z + Vector3::Up * inputAxis.y;
	const auto velocity = moveDirection * _moveSpeed * GameTime::I().GetDeltaTime();
	const auto transform = _transform.lock();
	transform->SetLocalPosition(transform->GetLocalPosition() + velocity);
}
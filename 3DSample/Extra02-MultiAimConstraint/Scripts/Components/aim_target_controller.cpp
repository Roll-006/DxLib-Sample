#include <string>
#include <JSON/json_loader.hpp>
#include <Math/math.hpp>
#include "transform.h"
#include "../Core/component_factory.h"
#include "../Core/time.h"
#include "../System/keyboard.h"
#include "aim_target_controller.h"

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("AimTargetController", [](GameObject& obj) { return obj.AddComponent<AimTargetController>(); });
		return true;
	}();
}

AimTargetController::AimTargetController() : 
	_moveSpeed(0.0f)
{

}

void AimTargetController::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;
	_transform	= GetTransform();
}

void AimTargetController::Update()
{
	if (!_enabled) { return; }

	Move();
}

void AimTargetController::LateUpdate()
{
	if (!_enabled) { return; }
}

void AimTargetController::Render() const
{
	if (!_enabled) { return; }

	auto transform = _transform.lock();
	DrawSphere3D(transform->GetWorldPosition(), 0.5f, 8, 0xffffff, 0xffffff, TRUE);

	DrawFormatString(0,   0, 0xffffff, "→ : 右へ移動");
	DrawFormatString(0,  20, 0xffffff, "← : 左へ移動");
	DrawFormatString(0,  40, 0xffffff, "↑ : 上へ移動");
	DrawFormatString(0,  60, 0xffffff, "↓ : 下へ移動");
	DrawFormatString(0,  80, 0xffffff, "SHIFT + ↑ : 奥へ移動");
	DrawFormatString(0, 100, 0xffffff, "SHIFT + ↓ : 手前へ移動");
}

void AimTargetController::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}

void AimTargetController::Move()
{
	// ターゲットを移動
	auto moveDir = math::Vector3(0.0f, 0.0f, 0.0f);
	const auto keyboard = Keyboard::GetInstance();
	if (keyboard.IsPressed(KEY_INPUT_LEFT))
	{
		moveDir.x -= 1;
	}
	if (keyboard.IsPressed(KEY_INPUT_RIGHT))
	{
		moveDir.x += 1;
	}
	if (keyboard.IsPressed(KEY_INPUT_LSHIFT) && keyboard.IsPressed(KEY_INPUT_DOWN))
	{
		moveDir.z -= 1;
	}
	else if (keyboard.IsPressed(KEY_INPUT_DOWN))
	{
		moveDir.y -= 1;
	}
	if (keyboard.IsPressed(KEY_INPUT_LSHIFT) && keyboard.IsPressed(KEY_INPUT_UP))
	{
		moveDir.z += 1;
	}
	else if (keyboard.IsPressed(KEY_INPUT_UP))
	{
		moveDir.y += 1;
	}

	// 座標を移動
	const auto transform	= _transform.lock();
	const auto velocity		= moveDir.LoadToSIMD() * _moveSpeed * Time::GetInstance().GetDeltaTime();
	const auto pos			= transform->GetLocalPosition().LoadToSIMD() + velocity;
	transform->SetLocalPosition(math::Vector3::StoreFromSIMD(pos));
}

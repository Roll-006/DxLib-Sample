#include "aim_target_movement.h"

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
	_transform	= GetTransform();
}

void AimTargetMovement::Update()
{
	if (!_enabled) return;

	Move();
}

void AimTargetMovement::LateUpdate()
{
	if (!_enabled) return;
}

void AimTargetMovement::Render() const
{
	if (!_enabled) return;

	auto transform = _transform.lock();
	DrawSphere3D(ToDxLibVector(transform->GetWorldPosition()), 0.5f, 8, 0xffffff, 0xffffff, TRUE);

	DrawFormatString(0,   0, 0xffffff, "→ : 右へ移動");
	DrawFormatString(0,  20, 0xffffff, "← : 左へ移動");
	DrawFormatString(0,  40, 0xffffff, "↑ : 上へ移動");
	DrawFormatString(0,  60, 0xffffff, "↓ : 下へ移動");
	DrawFormatString(0,  80, 0xffffff, "SHIFT + ↑ : 奥へ移動");
	DrawFormatString(0, 100, 0xffffff, "SHIFT + ↓ : 手前へ移動");
}

void AimTargetMovement::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}

void AimTargetMovement::Move() const
{
	// ターゲットを移動
	auto moveDirection = Vector3::Zero;
	const auto keyboard = Keyboard::GetInstance();
	if (keyboard.IsPressed(KEY_INPUT_LEFT))
	{
		moveDirection.x -= 1;
	}
	if (keyboard.IsPressed(KEY_INPUT_RIGHT))
	{
		moveDirection.x += 1;
	}
	if (keyboard.IsPressed(KEY_INPUT_LSHIFT) && keyboard.IsPressed(KEY_INPUT_DOWN))
	{
		moveDirection.z -= 1;
	}
	else if (keyboard.IsPressed(KEY_INPUT_DOWN))
	{
		moveDirection.y -= 1;
	}
	if (keyboard.IsPressed(KEY_INPUT_LSHIFT) && keyboard.IsPressed(KEY_INPUT_UP))
	{
		moveDirection.z += 1;
	}
	else if (keyboard.IsPressed(KEY_INPUT_UP))
	{
		moveDirection.y += 1;
	}

	// 座標を移動
	const auto transform = _transform.lock();
	const auto velocity = moveDirection * _moveSpeed * Time::GetInstance().GetDeltaTime();
	transform->SetLocalPosition(transform->GetLocalPosition() + velocity);
}
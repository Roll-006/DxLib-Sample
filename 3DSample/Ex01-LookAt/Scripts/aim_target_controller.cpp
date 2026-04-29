#include <array>
#include <DxLib.h>
#include <Vector/vector3.hpp>
#include <Matrix/matrix_4x4.hpp>
#include <JSON/json_loader.hpp>
#include <string>
#include "math.h"
#include "time.h"
#include "keyboard.h"
#include "transform.h"
#include "component_factory.h"
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

AimTargetController::~AimTargetController()
{

}

void AimTargetController::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;
	_transform = GetComponent<Transform>();
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
	DrawSphere3D(transform->GetPosition(), 0.5f, 8, 0xffffff, 0xffffff, TRUE);
	
}

void AimTargetController::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}

void AimTargetController::Move()
{
	// ターゲットを移動
	auto moveDir = Vector3::GetZero();
	const auto keyboard = Keyboard::GetInstance();
	if (keyboard.IsPressed(KEY_INPUT_LEFT))	{ moveDir.x -= 1; }
	if (keyboard.IsPressed(KEY_INPUT_RIGHT)){ moveDir.x += 1; }
	if (keyboard.IsPressed(KEY_INPUT_DOWN))	{ moveDir.y -= 1; }
	if (keyboard.IsPressed(KEY_INPUT_UP))	{ moveDir.y += 1; }
	if (keyboard.IsPressed(KEY_INPUT_S))	{ moveDir.z -= 1; }
	if (keyboard.IsPressed(KEY_INPUT_W))	{ moveDir.z += 1; }
	moveDir.Normalize();

	// 座標を移動
	auto transform = _transform.lock();
	transform->SetPosition(transform->GetPosition() + moveDir * _moveSpeed * Time::GetInstance().GetDeltaTime());
}

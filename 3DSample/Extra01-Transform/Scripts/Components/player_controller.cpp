#include <DxLib.h>
#include <Vector/vector2.hpp>
#include <Vector/vector3.hpp>
#include <Matrix/matrix_4x4.hpp>
#include <JSON/json_loader.hpp>
#include <string>
#include "../Core/math.h"
#include "transform.h"
#include "../Core/component_factory.h"
#include "../Core/time.h"
#include "../System/keyboard.h"
#include "player_controller.h"

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("PlayerController", [](GameObject& obj) { return obj.AddComponent<PlayerController>(); });
		return true;
	}();
}

PlayerController::PlayerController() : 
	_moveDir	(Vector3::GetZero()),
	_moveSpeed	(0.0f),
	_moveT		(0.0f)
{

}

PlayerController::~PlayerController()
{

}

void PlayerController::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;
	_transform	= GetTransform();

	_mainCameraTransform = gameObject->Find("MainCamera")->GetTransform();
}

void PlayerController::Update()
{
	if (!_enabled) { return; }

	Move();
}

void PlayerController::LateUpdate()
{
	if (!_enabled) { return; }
}

void PlayerController::Render() const
{
	if (!_enabled) { return; }
}

void PlayerController::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}

void PlayerController::Move()
{
	const auto mainCameraTransform = _mainCameraTransform.lock();

	auto forward = Vector2(mainCameraTransform->GetForward().x, mainCameraTransform->GetForward().z);
	forward.Normalize();

	auto right = Vector2(mainCameraTransform->GetRight().x, mainCameraTransform->GetRight().z);
	right.Normalize();

	const auto keyboard = Keyboard::GetInstance();
	auto inputAxis = Vector2::GetZero();
	if (keyboard.IsPressed(KEY_INPUT_W)) { inputAxis += forward; }
	if (keyboard.IsPressed(KEY_INPUT_S)) { inputAxis -= forward; }
	if (keyboard.IsPressed(KEY_INPUT_D)) { inputAxis += right; }
	if (keyboard.IsPressed(KEY_INPUT_A)) { inputAxis -= right; }
	inputAxis.Normalize();

	_moveDir = Vector3::GetLerp(_moveDir, Vector3(inputAxis.x, 0.0f, inputAxis.y), _moveT);

	const auto transform = _transform.lock();
	const auto localPos = transform->GetLocalPosition();
	transform->SetLocalPosition(localPos + _moveDir * _moveSpeed * Time::GetInstance().GetDeltaTime());
}

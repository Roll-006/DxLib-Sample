#include <string>
#include <json_loader.hpp>
#include <math.hpp>
#include "transform.h"
#include "../Core/component_factory.h"
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
	_moveT				(0.0f),
	_moveDir			(0.0f, 0.0f, 1.0f),
	_isMoving			(false),
	_mainCameraTransform()
{

}

void PlayerController::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;

	_mainCameraTransform = gameObject->Find("MainCamera")->GetTransform();
}

void PlayerController::Update()
{
	if (!_enabled) { return; }

	Control();
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

void PlayerController::Control()
{
	// 移動方向をカメラから取得
	const auto mainCameraTransform = _mainCameraTransform.lock();

	auto forward = Vector2(mainCameraTransform->GetForward().x, mainCameraTransform->GetForward().z);
	forward.Normalize();
	
	auto right = Vector2(mainCameraTransform->GetRight().x, mainCameraTransform->GetRight().z);
	right.Normalize();

	// 入力方向を計算
	const auto keyboard = Keyboard::GetInstance();
	auto inputAxis = Vector2::Zero;
	if (keyboard.IsPressed(KEY_INPUT_W)) { inputAxis += forward; }
	if (keyboard.IsPressed(KEY_INPUT_S)) { inputAxis -= forward; }
	if (keyboard.IsPressed(KEY_INPUT_D)) { inputAxis += right; }
	if (keyboard.IsPressed(KEY_INPUT_A)) { inputAxis -= right; }
	inputAxis.Normalize();

	// 移動判定
	_isMoving = inputAxis.Length() > math::kEpsilon;

	// 移動方向を計算
	const auto projectInputAxis = Vector3(inputAxis.x, 0.0f, inputAxis.y);
	_moveDir = Vector3::Lerp(_moveDir, projectInputAxis, _moveT);
}

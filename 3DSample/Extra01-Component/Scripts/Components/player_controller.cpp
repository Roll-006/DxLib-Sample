#include <string>
#include <JSON/json_loader.hpp>
#include <Math/math.hpp>
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

	UpdateControl();
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

void PlayerController::UpdateControl()
{
	// 移動方向をカメラから取得
	const auto mainCameraTransform = _mainCameraTransform.lock();

	auto forward = math::Vector2(mainCameraTransform->GetForward().x, mainCameraTransform->GetForward().z).LoadToSIMD();
	DirectX::XMVector2Normalize(forward);
	
	auto right = math::Vector2(mainCameraTransform->GetRight().x, mainCameraTransform->GetRight().z).LoadToSIMD();
	DirectX::XMVector2Normalize(right);

	// 入力方向を計算
	const auto keyboard = Keyboard::GetInstance();
	auto inputAxis = DirectX::XMVectorZero();
	if (keyboard.IsPressed(KEY_INPUT_W)) { inputAxis = DirectX::XMVectorAdd		(inputAxis, forward); }
	if (keyboard.IsPressed(KEY_INPUT_S)) { inputAxis = DirectX::XMVectorSubtract(inputAxis, forward); }
	if (keyboard.IsPressed(KEY_INPUT_D)) { inputAxis = DirectX::XMVectorAdd		(inputAxis, right); }
	if (keyboard.IsPressed(KEY_INPUT_A)) { inputAxis = DirectX::XMVectorSubtract(inputAxis, right); }
	DirectX::XMVector2Normalize(inputAxis);

	// 移動判定
	_isMoving = DirectX::XMVectorGetX(DirectX::XMVector3Length(inputAxis)) > math::kEpsilon;

	// 移動方向を計算
	const auto projectInputAxis = DirectX::XMVectorSet(DirectX::XMVectorGetX(inputAxis), 0.0f, DirectX::XMVectorGetY(inputAxis), 0.0f);
	_moveDir = math::Vector3::StoreFromSIMD(DirectX::XMVectorLerp(_moveDir.LoadToSIMD(), projectInputAxis, _moveT));
}

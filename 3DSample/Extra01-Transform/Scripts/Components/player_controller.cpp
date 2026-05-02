#include <DxLib.h>
#include <Vector/vector3.hpp>
#include <Matrix/matrix_4x4.hpp>
#include <JSON/json_loader.hpp>
#include <string>
#include "../Core/math.h"
#include "transform.h"
#include "../Core/component_factory.h"
#include "player_controller.h"

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("PlayerController", [](GameObject& obj) { return obj.AddComponent<PlayerController>(); });
		return true;
	}();
}

PlayerController::PlayerController()
{

}

PlayerController::~PlayerController()
{

}

void PlayerController::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;
	_transform	= GetTransform();
}

void PlayerController::Update()
{
	if (!_enabled) { return; }
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

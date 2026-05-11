#include <string>
#include <json_loader.hpp>
#include <math.hpp>
#include "transform.h"
#include "../Core/time.h"
#include "../Core/component_factory.h"
#include "player_controller.h"
#include "player_mover.h"

namespace
{
	const bool registered = []()
		{
			ComponentFactory::Register("PlayerMover", [](GameObject& obj) { return obj.AddComponent<PlayerMover>(); });
			return true;
		}();
}

PlayerMover::PlayerMover() : 
	_moveSpeed			(0.0f),
	_playerController	(),
	_transform			()
{

}

void PlayerMover::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;
	_transform	= GetTransform();

	_playerController = GetComponent<PlayerController>();
}

void PlayerMover::Update()
{
	if (!_enabled) { return; }

	Move();
}

void PlayerMover::LateUpdate()
{
	if (!_enabled) { return; }
}

void PlayerMover::Render() const
{
	if (!_enabled) { return; }
}

void PlayerMover::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}

void PlayerMover::Move()
{
	const auto controller	= _playerController.lock();
	const auto moveDir		= controller->GetMoveDir();

	// 座標を更新
	const auto transform	= _transform.lock();
	const auto velocity		= moveDir * _moveSpeed * Time::GetInstance().GetDeltaTime();
	transform->SetLocalPosition(transform->GetLocalPosition() + velocity);

	// 姿勢を更新
	if (controller->IsMoving()) { transform->LookAt(moveDir); }
}

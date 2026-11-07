#include <json_loader.hpp>
#include <math.hpp>
#include "../Core/transform.h"
#include "../Core/component_factory.h"
#include "camera_applier.h"

using namespace Core;
using namespace Components;

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("CameraApplier", [](GameObject& obj) { return obj.AddComponent<CameraApplier>(); });
		return true;
	}();
}

CameraApplier::CameraApplier()
{
	// 仮
	SetCameraNearFar(1.f, 100.f);
	SetupCamera_Perspective(60.f * Math::kDeg2Rad);
}

void CameraApplier::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;
	_transform = GetTransform();
}

void CameraApplier::Start()
{
	
}

void CameraApplier::Update()
{
	if (!_enabled) return;
}

void CameraApplier::LateUpdate()
{
	if (!_enabled) return;

	// カメラの位置及び姿勢を設定
	const auto transform = _transform.lock();
	SetCameraPositionAndTargetAndUpVec(ToDxLibVector(transform->GetWorldPosition()), ToDxLibVector(transform->GetForward()), ToDxLibVector(transform->GetUp()));
}

void CameraApplier::Render() const
{
	if (!_enabled) return;
}

void CameraApplier::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}
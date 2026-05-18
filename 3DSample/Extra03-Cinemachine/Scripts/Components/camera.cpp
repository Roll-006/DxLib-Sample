#include <string>
#include <json_loader.hpp>
#include <math.hpp>
#include "transform.h"
#include "../Core/component_factory.h"
#include "camera.h"

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("Camera", [](GameObject& obj) { return obj.AddComponent<Camera>(); });
		return true;
	}();
}

Camera::Camera() : 
	_projection	(),
	_rendering	(),
	_transform	()
{

}

void Camera::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;
	_transform	= GetTransform();
}

void Camera::Update()
{
	if (!_enabled) { return; }

	// 仮
	SetCameraNearFar(_projection.clippingPlanesNear, _projection.clippingPlanesFar);
	SetupCamera_Perspective(_projection.fieldOfView * math::kDeg2Rad);
}

void Camera::LateUpdate()
{
	if (!_enabled) { return; }

	// 仮
	// カメラの位置及び姿勢を設定
	const auto transform = _transform.lock();
	SetCameraPositionAndTargetAndUpVec(ToDxLibVector(transform->GetWorldPosition()), ToDxLibVector(transform->GetForward()), ToDxLibVector(transform->GetUp()));
}

void Camera::Render() const
{
	if (!_enabled) { return; }
}

void Camera::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}

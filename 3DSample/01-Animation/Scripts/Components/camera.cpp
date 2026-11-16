#include <json_loader.hpp>
#include <math.hpp>
#include <DxLibSample/transform.h>
#include <DxLibSample/component_factory.h>
#include "camera.h"
#include <DxLibSample/scene.h>

using namespace DxLibSample;
using namespace Components;

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("Camera", [](GameObject& obj) { return obj.AddComponent<Camera>(); });
		return true;
	}();
}

Camera::Camera() :
	_near	(1.f),
	_far	(1000.f),
	_fov	(60.f)
{
	
}

void Camera::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;
	_transform = GetTransform();
	
	SetCameraNearFar(_near, _far);
	SetupCamera_Perspective(_fov * Math::kDeg2Rad);
	_transform.lock()->SetWorldEulerAngles(_eulerAngles);
}

void Camera::Start()
{
	_target = _gameObject.lock()->GetScene()->Find(_targetName);
}

void Camera::Update()
{
	if (!_enabled) return;
}

void Camera::LateUpdate()
{
	if (!_enabled) return;
	
	_transform.lock()->SetWorldPosition(_target->GetWorldPosition() + _offset);
	
	const auto transform = _transform.lock();
	const auto invScaleMat = Matrix::CreateScale(1.f / transform->GetWorldScale());
	const auto worldMat = invScaleMat * transform->GetWorldMatrix();
	const auto view = ToDxLibMatrix(worldMat.Invert());
	
	SetCameraViewMatrix(view);
}

void Camera::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}
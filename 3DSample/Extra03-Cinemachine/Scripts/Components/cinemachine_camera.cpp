#include <string>
#include <json_loader.hpp>
#include <math.hpp>
#include "transform.h"
#include "../Core/component_factory.h"
#include "cinemachine_camera.h"

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("CinemachineCamera", [](GameObject& obj) { return obj.AddComponent<CinemachineCamera>(); });
		return true;
	}();
}

CinemachineCamera::CinemachineCamera()
{

}

void CinemachineCamera::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;
	_transform	= GetTransform();
}

void CinemachineCamera::Update()
{
	if (!_enabled) { return; }
}

void CinemachineCamera::LateUpdate()
{
	if (!_enabled) { return; }
}

void CinemachineCamera::Render() const
{
	if (!_enabled) { return; }
}

void CinemachineCamera::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}

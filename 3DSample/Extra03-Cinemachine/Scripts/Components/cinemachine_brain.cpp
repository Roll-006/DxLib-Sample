#include <string>
#include <json_loader.hpp>
#include <math.hpp>
#include "transform.h"
#include "../Core/component_factory.h"
#include "../System/keyboard.h"
#include "cinemachine_brain.h"

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("CinemachineBrain", [](GameObject& obj) { return obj.AddComponent<CinemachineBrain>(); });
		return true;
	}();
}

CinemachineBrain::CinemachineBrain() : 
	_blendTime			(0.0f),
	_blendTimer			(0.0f),
	_mainCameraTransform()
{
	
}

void CinemachineBrain::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject = gameObject;

	_mainCameraTransform = gameObject->Find("MainCamera")->GetTransform();
}

void CinemachineBrain::Update()
{
	if (!_enabled) { return; }
}

void CinemachineBrain::LateUpdate()
{
	if (!_enabled) { return; }
}

void CinemachineBrain::Render() const
{
	if (!_enabled) { return; }
}

void CinemachineBrain::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}

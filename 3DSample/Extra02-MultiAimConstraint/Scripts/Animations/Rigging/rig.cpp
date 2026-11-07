#include <math.hpp>
#include "../../Core/transform.h"
#include "../../Core/component_factory.h"
#include "../../Model/model_renderer.h"
#include "rig.h"

using namespace Core;
using namespace Animations::Rigging;

namespace
{
	const bool registered = []()
	{
		ComponentFactory::Register("Rig", [](GameObject& obj) { return obj.AddComponent<Rig>(); });
		return true;
	}();
}

Rig::Rig() : 
	_weight(1.f)
{
	
}

void Rig::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
	_gameObject	= gameObject;
}

void Rig::Start()
{
	_rigConstraints = GetComponentsInChildren<RigConstraint>();
}

void Rig::Update()
{
	if (!_enabled) return;
}

void Rig::LateUpdate()
{
	if (!_enabled) return;
}

void Rig::Render() const
{
	if (!_enabled) return;
}

void Rig::Deserialize(const nlohmann::json& json)
{
	from_json(json, *this);
}

void Rig::SetWeight(float weight)
{
	_weight = std::clamp(weight, 0.f, 1.f);
	
	for (const auto& rigConstraint : _rigConstraints)
	{
		rigConstraint->SetRigWeight(_weight);
	}
}
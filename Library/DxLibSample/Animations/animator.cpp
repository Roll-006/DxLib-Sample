#include <json_loader.hpp>
#include <math.hpp>
#include "../transform.h"
#include "../component_factory.h"
#include "animator.h"
#include "../Model/model_renderer.h"

using namespace DxLibSample;
using namespace DxLibSample::Animations;
using namespace DxLibSample::Model;

namespace
{
    const bool registered = []()
    {
        ComponentFactory::Register("Animator", [](GameObject& obj) { return obj.AddComponent<Animator>(); });
        return true;
    }();
}

Animator::Animator()
{
    
}

void Animator::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
    _gameObject = gameObject;
    _transform = GetComponent<Transform>();
}

void Animator::Start()
{
    _modelData = GetComponentInChildren<ModelRenderer>()->GetModelData();
    _animatorController.Initialize(_modelData);
    _animationRootIndex = MV1SearchFrame(_modelData.modelHandle, _animationRootName.c_str());
}

void Animator::Update()
{
    if (!_enabled) return;
    
    _animatorController.UpdateAnimation();
    _animatorController.ApplyRootMotion(_isApplyRootMotion, _animationRootIndex, _transform.lock());
}

void Animator::LateUpdate()
{
    if (!_enabled) return;
}

void Animator::Deserialize(const nlohmann::json& json)
{
    from_json(json, *this);
}

void Animator::SetFloat(const std::string& name, float value)
{
    _animatorController.SetFloat(name, value);
}

void Animator::SetBool(const std::string& name, bool value)
{
    _animatorController.SetBool(name, value);
}

void Animator::SetInteger(const std::string& name, int value)
{
    _animatorController.SetInteger(name, value);
}

void Animator::SetTrigger(const std::string& name)
{
    _animatorController.SetTrigger(name);
}

void Animator::ResetTrigger(const std::string& name)
{
    _animatorController.ResetTrigger(name);
}
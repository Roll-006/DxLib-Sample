#include <json_loader.hpp>
#include <math.hpp>
#include "transform.h"
#include "../Core/component_factory.h"
#include "animator.h"
#include "model_renderer.h"

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

Animator::~Animator()
{
    
}

void Animator::Initialize(const std::shared_ptr<GameObject>& gameObject)
{
    _gameObject = gameObject;
    _modelData = GetComponent<ModelRenderer>()->GetModelData();
    
    _animatorController.Initialize(_modelData);
}

void Animator::Update()
{
    if (!_enabled) return;
    
    _animatorController.UpdateAnimation();
}

void Animator::LateUpdate()
{
    if (!_enabled) return;
}

void Animator::Render() const
{
    if (!_enabled) return;
}

void Animator::Deserialize(const nlohmann::json& json)
{
    from_json(json, *this);
}
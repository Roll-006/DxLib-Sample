#include <json_loader.hpp>
#include <math.hpp>
#include "component_factory.h"
#include "transform.h"
#include "scene.h"
#include "game_object.h"

using namespace DxLibSample;

GameObject::GameObject(const std::string& jsonPath) :
	_isActive(true)
{
	// Transformは必須で追加
	AddComponent<Transform>();

	nlohmann::json json;
	if (JsonLoader::Load(jsonPath, json))
	{
		from_json(json, *this);

		// コンポーネントを生成
		for (const auto& componentJson : json.at("components"))
		{
			const auto type = componentJson.at("type").get<std::string>();
			auto component = ComponentFactory::Create(type, *this);

			if (!component) { continue; }
			
			component->Deserialize(componentJson);
		}
	}
}

void GameObject::Initialize(const std::shared_ptr<Scene>& scene)
{
	_scene = scene;

	for (const auto& component : _components)
	{
		component->Initialize(shared_from_this());
	}
}

void GameObject::Start() const
{
	for (const auto& component : _components)
	{
		component->Start();
	}
}

void GameObject::Update() const
{
	if (!_isActive) return;

	for (const auto& component : _components)
	{
		component->Update();
	}
}

void GameObject::LateUpdate() const
{
	if (!_isActive) return;

	for (const auto& component : _components)
	{
		component->LateUpdate();
	}
}

void GameObject::Render() const
{
	if (!_isActive) return;

	for (const auto& component : _components)
	{
		component->Render();
	}
}

std::shared_ptr<Transform> GameObject::Find(const std::string& name) const
{
	return _scene.lock()->Find(name);
}

std::shared_ptr<Transform> GameObject::GetTransform() const
{
	return GetComponent<Transform>();
}

std::shared_ptr<Scene> GameObject::GetScene() const
{
	return _scene.lock();
}

std::vector<std::shared_ptr<GameObject>> GameObject::GetChildrenGameObject() const
{
	std::vector<std::shared_ptr<GameObject>> gameObjects = {};

	const auto transform = GetTransform();
	if (!transform) return gameObjects;

	for (const auto& child : transform->GetChildren())
	{
		if (const auto object = child->GetGameObject()) gameObjects.emplace_back(object);
	}

	return gameObjects;
}
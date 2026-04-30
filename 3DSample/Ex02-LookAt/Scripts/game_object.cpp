#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "math.h"
#include "component_factory.h"
#include "transform.h"
#include "scene.h"
#include "game_object.h"

GameObject::GameObject(const std::string& jsonPath) :
	_name		(""),
	_tag		(""),
	_isActive	(true),
	_owenerScene()
{
	nlohmann::json json;
	if (json_loader::Load(jsonPath, json))
	{
		from_json(json, *this);

		// コンポーネントを生成
		for (const auto& componentJson : json.at("Components"))
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
	_owenerScene = scene;

	for (const auto& component : _components)
	{
		component->Initialize(shared_from_this());
	}
}

void GameObject::Update()
{
	if (!_isActive) { return; }

	for (const auto& component : _components)
	{
		component->Update();
	}
}

void GameObject::LateUpdate()
{
	if (!_isActive) { return; }

	for (const auto& component : _components)
	{
		component->LateUpdate();
	}
}

void GameObject::Render() const
{
	if (!_isActive) { return; }

	for (const auto& component : _components)
	{
		component->Render();
	}
}

/// <summary>
/// 同一シーン上のオブジェクトを検索
/// </summary>
/// <param name="name">オブジェクト名</param>
/// <returns>見つかったオブジェクトのTransform。見つからなかった場合はnullptr</returns>
std::shared_ptr<Transform> GameObject::Find(const std::string& name)
{
	return _owenerScene.lock()->Find(name);
}

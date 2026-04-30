#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "math.h"
#include "component_factory.h"
#include "game_object_factory.h"
#include "transform.h"
#include "game_object.h"
#include "scene.h"

Scene::Scene(const std::string& jsonPath) : 
	_name		(""),
	_jsonPath	(jsonPath),
	_objects	()
{

}

void Scene::Initialize()
{
	nlohmann::json json;
	if (json_loader::Load(_jsonPath, json))
	{
		from_json(json, *this);
		LoadGameObjects(json.at("objects"));
	}

	for (const auto& object : _objects)
	{
		object->Initialize(shared_from_this());
	}
}

void Scene::Update()
{
	for (const auto& object : _objects)
	{
		object->Update();
	}
}

void Scene::LateUpdate()
{
	for (const auto& object : _objects)
	{
		object->LateUpdate();
	}
}

void Scene::Render() const
{
	for (const auto& object : _objects)
	{
		object->Render();
	}
}

std::shared_ptr<Transform> Scene::Find(const std::string& name)
{
	for (const auto& object : _objects)
	{
		if (object->GetName() == name)
		{
			return object->GetComponent<Transform>();
		}
	}

	return nullptr;
}

std::vector<std::shared_ptr<Transform>> Scene::LoadGameObjects(const nlohmann::json& childrenJson)
{
	std::vector<std::shared_ptr<Transform>> childTransform;

	for (const auto& objectJson : childrenJson)
	{
		const auto name			= objectJson.at("name").get<std::string>();
		const auto gameObject	= GameObjectFactory::Create(name, *this);
		const auto transform	= gameObject->GetComponent<Transform>();

		// 子階層も全て読み込む
		for (const auto& child : LoadGameObjects(objectJson.at("children")))
		{
			child->SetParent(transform);
		}

		childTransform.emplace_back(transform);
	}

	return childTransform;
}

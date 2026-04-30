#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "math.h"
#include "component_factory.h"
#include "transform.h"
#include "game_object.h"
#include "scene.h"

Scene::Scene(/*const std::string& jsonPath*/)
{

}

void Scene::Initialize()
{
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

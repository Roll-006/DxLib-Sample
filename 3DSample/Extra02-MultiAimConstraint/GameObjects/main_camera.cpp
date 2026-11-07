#include <json_loader.hpp>
#include <math.hpp>
#include "../Scripts/Core/transform.h"
#include "../Scripts/Core/scene.h"
#include "../Scripts/Core/game_object_factory.h"
#include "main_camera.h"

using namespace Core;
using namespace GameObjects;

namespace
{
	const bool registered = []()
	{
		GameObjectFactory::Register("MainCamera", [](Scene& scene) { return scene.AddGameObject<MainCamera>(); });
		return true;
	}();
}

MainCamera::MainCamera() :
	GameObject("JSON/GameObjects/main_camera.json")
{

}
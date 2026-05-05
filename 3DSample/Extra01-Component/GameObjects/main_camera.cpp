#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Math/math.hpp>
#include "../Scripts/Components/transform.h"
#include "../Scripts/Core/scene.h"
#include "../Scripts/Core/game_object_factory.h"
#include "main_camera.h"

namespace
{
	const bool registered = []()
	{
		GameObjectFactory::Register("MainCamera", [](Scene& scene) { return scene.AddGameObject<MainCamera>(); });
		return true;
	}();
}

MainCamera::MainCamera() :
	GameObject("JSON/main_camera.json")
{

}

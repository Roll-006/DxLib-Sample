#include <json_loader.hpp>
#include <math.hpp>
#include <DxLibSample/transform.h>
#include <DxLibSample/scene.h>
#include <DxLibSample/game_object_factory.h>
#include "main_camera.h"

using namespace DxLibSample;
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
#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "../../Scripts/math.h"
#include "../../Scripts/transform.h"
#include "../../Scripts/scene.h"
#include "../../Scripts/game_object_factory.h"
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

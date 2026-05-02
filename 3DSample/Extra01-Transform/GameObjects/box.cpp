#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "../Scripts/Core/math.h"
#include "../Scripts/Components/transform.h"
#include "../Scripts/Core/scene.h"
#include "../Scripts/Core/game_object_factory.h"
#include "box.h"

namespace
{
	const bool registered = []()
	{
		GameObjectFactory::Register("Box", [](Scene& scene) { return scene.AddGameObject<Box>(); });
		return true;
	}();
}

Box::Box() :
	GameObject("JSON/box.json")
{

}

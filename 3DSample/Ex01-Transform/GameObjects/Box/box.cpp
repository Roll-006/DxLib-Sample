#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "../../Scripts/math.h"
#include "../../Scripts/transform.h"
#include "../../Scripts/scene.h"
#include "../../Scripts/game_object_factory.h"
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

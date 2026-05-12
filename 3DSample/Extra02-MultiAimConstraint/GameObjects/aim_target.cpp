#include <string>
#include <vector>
#include <json_loader.hpp>
#include <math.hpp>
#include "../Scripts/Components/transform.h"
#include "../Scripts/Core/scene.h"
#include "../Scripts/Core/game_object_factory.h"
#include "aim_target.h"

namespace
{
	const bool registered = []()
	{
		GameObjectFactory::Register("AimTarget", [](Scene& scene) { return scene.AddGameObject<AimTarget>(); });
		return true;
	}();
}

AimTarget::AimTarget() :
	GameObject("JSON/aim_target.json")
{

}

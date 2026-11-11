#include <json_loader.hpp>
#include <math.hpp>
#include "../../Scripts/Core/transform.h"
#include "../../Scripts/Core/scene.h"
#include "../../Scripts/Core/game_object_factory.h"
#include "aim_rig.h"

using namespace Core;
using namespace GameObjects::Player;

namespace
{
	const bool registered = []()
	{
		GameObjectFactory::Register("AimRig", [](Scene& scene) { return scene.AddGameObject<AimRig>(); });
		return true;
	}();
}

AimRig::AimRig() : 
	GameObject("JSON/GameObjects/Player/aim_rig.json")
{

}
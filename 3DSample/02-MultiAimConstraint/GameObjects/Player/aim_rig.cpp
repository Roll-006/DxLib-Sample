#include <json_loader.hpp>
#include <math.hpp>
#include <DxLibSample/transform.h>
#include <DxLibSample/scene.h>
#include <DxLibSample/game_object_factory.h>
#include "aim_rig.h"

using namespace DxLibSample;
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
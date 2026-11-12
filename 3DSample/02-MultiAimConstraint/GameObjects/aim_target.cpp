#include <json_loader.hpp>
#include <math.hpp>
#include <DxLibSample/transform.h>
#include <DxLibSample/scene.h>
#include <DxLibSample/game_object_factory.h>
#include "aim_target.h"

using namespace DxLibSample;
using namespace GameObjects;

namespace
{
	const bool registered = []()
	{
		GameObjectFactory::Register("AimTarget", [](Scene& scene) { return scene.AddGameObject<AimTarget>(); });
		return true;
	}();
}

AimTarget::AimTarget() :
	GameObject("JSON/GameObjects/aim_target.json")
{

}
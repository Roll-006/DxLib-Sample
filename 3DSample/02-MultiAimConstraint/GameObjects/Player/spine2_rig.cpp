#include <json_loader.hpp>
#include <math.hpp>
#include <DxLibSample/transform.h>
#include <DxLibSample/scene.h>
#include <DxLibSample/game_object_factory.h>
#include "spine2_rig.h"

using namespace DxLibSample;
using namespace GameObjects::Player;

namespace
{
	const bool registered = []()
	{
		GameObjectFactory::Register("Spine2Rig", [](Scene& scene) { return scene.AddGameObject<Spine2Rig>(); });
		return true;
	}();
}

Spine2Rig::Spine2Rig() : 
	GameObject("JSON/GameObjects/Player/spine2_rig.json")
{

}
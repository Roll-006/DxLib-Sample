#include <json_loader.hpp>
#include <math.hpp>
#include "../../Scripts/Core/transform.h"
#include "../../Scripts/Core/scene.h"
#include "../../Scripts/Core/game_object_factory.h"
#include "spine1_rig.h"

using namespace Core;
using namespace GameObjects::Player;

namespace
{
	const bool registered = []()
	{
		GameObjectFactory::Register("Spine1Rig", [](Scene& scene) { return scene.AddGameObject<Spine1Rig>(); });
		return true;
	}();
}

Spine1Rig::Spine1Rig() : 
	GameObject("JSON/GameObjects/Player/spine1_rig.json")
{

}
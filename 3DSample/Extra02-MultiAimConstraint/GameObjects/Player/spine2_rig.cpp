#include <json_loader.hpp>
#include <math.hpp>
#include "../../Scripts/Core/transform.h"
#include "../../Scripts/Core/scene.h"
#include "../../Scripts/Core/game_object_factory.h"
#include "spine2_rig.h"

using namespace Core;
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
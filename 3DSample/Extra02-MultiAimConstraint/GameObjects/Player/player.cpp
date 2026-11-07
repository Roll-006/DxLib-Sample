#include <json_loader.hpp>
#include <math.hpp>
#include "../../Scripts/Core/transform.h"
#include "../../Scripts/Core/scene.h"
#include "../../Scripts/Core/game_object_factory.h"
#include "player.h"

using namespace Core;
using namespace GameObjects::Player;

namespace
{
	const bool registered = []()
	{
		GameObjectFactory::Register("Player", [](Scene& scene) { return scene.AddGameObject<Player>(); });
		return true;
	}();
}

Player::Player() : 
	GameObject("JSON/GameObjects/player.json")
{

}
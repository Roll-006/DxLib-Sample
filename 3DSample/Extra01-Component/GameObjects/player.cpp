#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Math/math.hpp>
#include "../Scripts/Components/transform.h"
#include "../Scripts/Core/scene.h"
#include "../Scripts/Core/game_object_factory.h"
#include "player.h"

namespace
{
	const bool registered = []()
	{
		GameObjectFactory::Register("Player", [](Scene& scene) { return scene.AddGameObject<Player>(); });
		return true;
	}();
}

Player::Player() : 
	GameObject("JSON/player.json")
{

}

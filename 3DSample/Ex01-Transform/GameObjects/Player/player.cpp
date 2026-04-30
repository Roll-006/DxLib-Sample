#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "../../Scripts/math.h"
#include "../../Scripts/transform.h"
#include "../../Scripts/scene.h"
#include "../../Scripts/game_object_factory.h"
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

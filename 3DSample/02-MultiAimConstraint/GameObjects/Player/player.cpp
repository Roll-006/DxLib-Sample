#include <json_loader.hpp>
#include <math.hpp>
#include <DxLibSample/transform.h>
#include <DxLibSample/scene.h>
#include <DxLibSample/game_object_factory.h>
#include "player.h"

using namespace DxLibSample;
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
	GameObject("JSON/GameObjects/Player/player.json")
{

}
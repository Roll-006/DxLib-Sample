#include <DxLibSample/scene.h>
#include <DxLibSample/game_object_factory.h>
#include "skin.h"

using namespace DxLibSample;
using namespace GameObjects::Player;

namespace
{
	const bool registered = []()
	{
		GameObjectFactory::Register("Skin", [](Scene& scene) { return scene.AddGameObject<Skin>(); });
		return true;
	}();
}

Skin::Skin() : 
	GameObject("JSON/GameObjects/Player/skin.json")
{

}
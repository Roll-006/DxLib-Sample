#include <DxLibSample/scene.h>
#include <DxLibSample/game_object_factory.h>
#include "prototype_map.h"

using namespace DxLibSample;
using namespace GameObjects;

namespace
{
    const bool registered = []()
    {
        GameObjectFactory::Register("PrototypeMap", [](Scene& scene) { return scene.AddGameObject<PrototypeMap>(); });
        return true;
    }();
}

PrototypeMap::PrototypeMap() :
    GameObject("JSON/GameObjects/prototype_map.json")
{

}
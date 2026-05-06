#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Math/math.hpp>
#include "../Scripts/Core/component_factory.h"
#include "../Scripts/Components/transform.h"
#include "../Scripts/Core/game_object.h"
#include "demo_scene.h"

DemoScene::DemoScene() : 
	Scene("JSON/demo_scene.json")
{

}

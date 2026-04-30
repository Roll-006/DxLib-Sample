#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "../../Scripts/math.h"
#include "../../Scripts/component_factory.h"
#include "../../Scripts/transform.h"
#include "../../Scripts/game_object.h"
#include "demo_scene.h"

DemoScene::DemoScene() : 
	Scene("JSON/demo_scene.json")
{

}

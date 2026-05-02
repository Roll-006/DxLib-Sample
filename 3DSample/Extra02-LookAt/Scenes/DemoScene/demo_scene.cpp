#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "../../Scripts/math.h"
#include "../../Scripts/component_factory.h"
#include "../../Scripts/transform.h"
#include "../../Scripts/game_object.h"

// 仮
#include "../../GameObjects/AimTarget/aim_target.h"
#include "../../GameObjects/Player/player.h"
#include "../../GameObjects/MainCamera/main_camera.h"

#include "demo_scene.h"

DemoScene::DemoScene()
{
	// 仮
	_name = "DemoScene";
	_objects.emplace_back(std::make_shared<AimTarget>());
	_objects.emplace_back(std::make_shared<Player>());
	_objects.emplace_back(std::make_shared<MainCamera>());
}

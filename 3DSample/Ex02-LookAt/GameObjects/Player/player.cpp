#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "../../Scripts/math.h"
#include "../../Scripts/transform.h"
#include "player.h"

Player::Player() : 
	GameObject("JSON/player.json")
{

}

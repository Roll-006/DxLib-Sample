#include <string>
#include <vector>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "../../Scripts/math.h"
#include "../../Scripts/transform.h"
#include "main_camera.h"

MainCamera::MainCamera() :
	GameObject("JSON/main_camera.json")
{

}

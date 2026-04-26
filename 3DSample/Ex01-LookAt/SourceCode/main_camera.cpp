#include <DxLib.h>
#include <Vector/vector3.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "math.h"
#include "transform.h"
#include "main_camera.h"

MainCamera::MainCamera() : 
	_transform	(Transform())
{
	SetCameraNearFar(1.0f, 100.0f);
	SetupCamera_Perspective(60.0f * math::kDeg2Rad);

	_transform.SetPosition({ 0, 30, -15 });
}

void MainCamera::Update()
{
	// カメラの位置及び姿勢を設定
	SetCameraPositionAndTargetAndUpVec(_transform.GetPosition(), _transform.GetForward(), _transform.GetUp());
}

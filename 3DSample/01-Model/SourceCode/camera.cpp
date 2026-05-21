#include <math.hpp>
#include "transform.h"
#include "camera.h"

Camera::Camera() : 
	_transform(Transform())
{
	_transform.position = kFirstPosition;

	SetCameraNearFar(kNear, kFar);
	SetupCamera_Perspective(kFOV * math::kDeg2Rad);
}

void Camera::Update()
{
	SetCameraPositionAndTargetAndUpVec(ToDxLibVector(_transform.position), ToDxLibVector(_transform.GetForward()), ToDxLibVector(_transform.GetUp()));
}

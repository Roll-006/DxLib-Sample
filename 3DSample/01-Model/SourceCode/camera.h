#pragma once

class Camera
{
public:
	Camera();
	void Update();

private:
	static constexpr Vector3	kFirstPosition	= { 0.0f, 30.0f, -20.0f };
	static constexpr float		kNear			= 1.0f;
	static constexpr float		kFar			= 1000.0f;
	static constexpr float		kFOV			= 55.0f;

	Transform _transform;
};
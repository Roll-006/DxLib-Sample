#pragma once

class Player
{
public:
	Player();

	void Update();
	void Draw() const;

private:
	Transform _transform;
	Modeler _modeler;

	int _frameIndex;
	Vector3 _target;
};
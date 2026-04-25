#pragma once

class Player
{
public:
	Player();

	void Update();
	void Draw() const;

	AABB GetCollider() const { return _aabb; }

private:
	void Move();

private:
	static constexpr Vector2 kFirstPos	= { 500.0f, 500.0f };
	static constexpr Vector2 kScale		= { 10.0f, 10.0f };
	static constexpr float	 kSpeed		= 500.0f;

	Transform	_transform;
	Animator	_animator;
	AABB		_aabb;
};

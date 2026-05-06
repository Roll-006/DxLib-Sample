#pragma once

class Enemy
{
public:
	Enemy();

	void Update();
	void Draw() const;

	AABB GetCollider() const { return _aabb; }

private:
	static constexpr math::Vector2 kFirstPos	= { 1300.0f, 500.0f };
	static constexpr math::Vector2 kScale		= { 9.0f, 9.0f };

	Transform	_transform;
	Animator	_animator;
	AABB		_aabb;
};

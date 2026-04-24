#pragma once
#include <Vector/vector2.hpp>

class Player
{
public:
	Player();
	~Player();

	void Update();
	void Draw() const;

private:
	void Move();

private:
	static constexpr Vector2		kFirstPos	= { 500.0f, 500.0f };
	static constexpr float			kSpeed		= 500.0f;
	static constexpr int			kRadius		= 100;
	static constexpr unsigned int	kColor		= 0xffffff;

	Vector2 _pos = kFirstPos;
};

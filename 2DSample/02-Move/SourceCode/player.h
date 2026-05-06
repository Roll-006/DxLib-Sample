#pragma once

class Player
{
public:
	void Update();
	void Draw() const;

private:
	void Move();

private:
	static constexpr math::Vector2	kFirstPos	= { 100.0f, 100.0f };
	static constexpr float			kSpeed		= 5.0f;
	static constexpr int			kRadius		= 100;
	static constexpr unsigned int	kColor		= 0xffffff;

	math::Vector2 _pos = kFirstPos;
};

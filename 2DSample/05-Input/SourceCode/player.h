#pragma once

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
	static constexpr Vector2		kScale		= { 10.0f, 10.0f };
	static constexpr float			kSpeed		= 500.0f;
	static constexpr int			kRadius		= 100;
	static constexpr unsigned int	kColor		= 0xffffff;

	Transform _transform;
	Animator _animator;
};

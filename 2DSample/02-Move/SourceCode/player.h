#pragma once

class Player
{
public:
	void Update();
	void Render() const;

private:
	void Move();

private:
	static constexpr Vector2		kFirstPosition	= { 100.0f, 100.0f };
	static constexpr float			kSpeed			= 5.0f;
	static constexpr int			kRadius			= 100;
	static constexpr unsigned int	kColor			= 0xffffff;

	Vector2 _position = kFirstPosition;
};

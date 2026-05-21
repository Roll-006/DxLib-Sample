#pragma once

class Player
{
public:
	Player();
	void Update();
	void Render() const;

private:
	void Move();

	/// <summary>
	/// 弾を撃つ
	/// </summary>
	void Shoot();

private:
	static constexpr Vector2		kFirstPosition	= { 500.0f, 500.0f };
	static constexpr float			kSpeed			= 500.0f;
	static constexpr int			kRadius			= 50;
	static constexpr unsigned int	kColor			= 0xe33963;
	static constexpr int			kBulletNum		= 25;
	static constexpr float			kShootSpeed		= 1000.0f;
	static constexpr Vector2		kShootDirection	= { 1.0f, 0.0f };

	Vector2 _pos;
	std::vector<Bullet> _bullets;
};

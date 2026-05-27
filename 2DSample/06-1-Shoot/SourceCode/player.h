#pragma once

class Player
{
public:
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

<<<<<<< HEAD:2DSample/06-1-Shoot/SourceCode/player.h
	Vector2 _position;
	std::vector<Bullet> _bullets;
=======
	Vector2 _position = kFirstPosition;
	std::array<Bullet, kBulletNum> _bullets;	// 指定のサイズを指定して初期化
>>>>>>> f6fd91f0c5688c20eac593b66c33923a445441b1:2DSample/07-1-Shoot/SourceCode/player.h
};

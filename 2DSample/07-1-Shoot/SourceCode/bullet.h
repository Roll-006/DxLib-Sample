#pragma once

class Bullet
{
public:
	void Update();
	void Render() const;

	/// <summary>
	/// 撃たれる
	/// </summary>
	/// <param name="shotPos">撃った座標</param>
	/// <param name="moveDir">移動方向 (発射方向)</param>
	/// <param name="speed">移動速度</param>
	void OnShoot(const Vector2& shotPos, const Vector2& moveDir, const float speed);

	bool IsActive() const { return _isActive; }

private:
	void Move();

	/// <summary>
	/// スクリーンを離れた
	/// </summary>
	/// <returns>true : スクリーン外にいる, false : スクリーン内にいる</returns>
	bool IsOffScreen() const;

private:
	static constexpr int			kRadius	= 10;
	static constexpr unsigned int	kColor	= 0xffffff;

	Vector2 _pos;
	Vector2 _moveDir;
	float	_speed;
	bool	_isActive = false;
};

#include <math.hpp>
#include "window.h"
#include "time.h"
#include "bullet.h"

void Bullet::Update()
{
	// 非アクティブ(撃たれていない)の場合は更新しない
	if (!_isActive) { return; }

	Move();
	
	// スクリーンから離れたら、弾を消す
	if (IsOffScreen()) { _isActive = false; }
}

void Bullet::Render() const
{
	// 非アクティブ(撃たれていない)の場合は描画しない
	if (!_isActive) { return; }

	DrawCircle(static_cast<int>(_position.x), static_cast<int>(_position.y), kRadius, kColor);
}

void Bullet::OnShoot(const Vector2& shotPosition, const Vector2& moveDirection, const float speed)
{
	_position = shotPosition;
	_moveDirection = moveDirection;
	_speed = speed;

	// 安全のため移動方向を正規化する
	_moveDirection.Normalize();

	// 撃たれたらアクティブ状態にする
	_isActive = true;
}

void Bullet::Move()
{
	const auto velocity = _moveDirection * _speed * Time::GetInstance().GetDeltaTime();
	_position += velocity;
}

bool Bullet::IsOffScreen() const
{
	if (_position.x + kRadius < 0.0f)				{ return true; }	// スクリーン左に出た
	if (_position.x - kRadius > window::kSize.x)	{ return true; }	// スクリーン右に出た
	if (_position.y + kRadius < 0.0f)				{ return true; }	// スクリーン上に出た
	if (_position.y - kRadius > window::kSize.y)	{ return true; }	// スクリーン下に出た

	return false;
}

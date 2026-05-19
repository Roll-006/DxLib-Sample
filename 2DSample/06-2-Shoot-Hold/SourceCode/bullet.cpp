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

void Bullet::Draw() const
{
	// 非アクティブ(撃たれていない)の場合は描画しない
	if (!_isActive) { return; }

	DrawCircle(static_cast<int>(_pos.x), static_cast<int>(_pos.y), kRadius, kColor);
}

void Bullet::OnShoot(const Vector2& shotPos, const Vector2& moveDir, const float speed)
{
	_pos = shotPos;
	_moveDir = moveDir;
	_speed = speed;

	// 安全のため移動方向を正規化する
	_moveDir.Normalize();

	// 撃たれたらアクティブ状態にする
	_isActive = true;
}

void Bullet::Move()
{
	const auto velocity = _moveDir * _speed * Time::GetInstance().GetDeltaTime();
	_pos += velocity;
}

bool Bullet::IsOffScreen() const
{
	if (_pos.x + kRadius < 0.0f)			{ return true; }	// スクリーン左に出た
	if (_pos.x - kRadius > window::kSize.x) { return true; }	// スクリーン右に出た
	if (_pos.y + kRadius < 0.0f)			{ return true; }	// スクリーン上に出た
	if (_pos.y - kRadius > window::kSize.y)	{ return true; }	// スクリーン下に出た

	return false;
}

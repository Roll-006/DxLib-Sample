#include <math.hpp>
#include "time.h"
#include "keyboard.h"
#include "transform.h"
#include "player.h"

Player::Player() : 
	_transform(Transform())
{
	_transform.position = kFirstPosition;
}

void Player::Update()
{
	Move();
}

void Player::Render() const
{
	DrawCircle(static_cast<int>(_transform.position.x), static_cast<int>(_transform.position.y), kRadius, 0xffffff, TRUE);
}

void Player::Move()
{
	// このサンプルでは、DirectXMathのラップーライブラリを使用しています。
	// DXライブラリの演算は計算が不足しており不便なことが理由です。

	// 移動方向を初期化
	auto moveDirection = Vector2::Zero;

	// 移動方向を入力
	const auto keyboard = Keyboard::GetInstance();
	if (keyboard.IsPressed(KEY_INPUT_W)) { moveDirection.y -= 1.0f; }
	if (keyboard.IsPressed(KEY_INPUT_S)) { moveDirection.y += 1.0f; }
	if (keyboard.IsPressed(KEY_INPUT_A)) { moveDirection.x -= 1.0f; }
	if (keyboard.IsPressed(KEY_INPUT_D)) { moveDirection.x += 1.0f; }

	// ベクトルを正規化
	// MEMO : 右下に移動した場合[x = 1, y = 1]で√2の長さを移動してしまい、移動速度が上昇します。
	//        正規化(長さを1にする)を行い移動速度を常に一定に保ちます。
	moveDirection.Normalize();

	// velocity(移動方向・速度を持つベクトル)を計算
	// MEMO : デルタタイムを掛けてどの環境で動かしても速度が一定になるようにする。
	const auto velocity = moveDirection * kSpeed * Time::GetInstance().GetDeltaTime();

	// 座標を更新
	_transform.position += velocity;
}

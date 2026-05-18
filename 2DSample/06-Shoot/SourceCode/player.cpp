#include <vector>
#include <math.hpp>
#include "window.h"
#include "time.h"
#include "keyboard.h"
#include "mouse.h"
#include "bullet.h"
#include "player.h"

Player::Player() : 
	_pos	(kFirstPos),
	_bullets(kBulletNum)	// 指定のサイズを指定して初期化
{

}

void Player::Update()
{
	Move();
	Shoot();

	// すべての弾を更新する
	for (size_t i = 0; i < _bullets.size(); ++i)
	{
		_bullets.at(i).Update();
	}
}

void Player::Draw() const
{
	DrawCircle(static_cast<int>(_pos.x), static_cast<int>(_pos.y), kRadius, kColor);

	// すべての弾を描画する
	for (size_t i = 0; i < _bullets.size(); ++i)
	{
		_bullets.at(i).Draw();
	}
}

void Player::Move()
{
	// このサンプルでは、DirectXMathのラップーライブラリを使用しています。
	// DXライブラリの演算は計算が不足しており不便なことが理由です。

	// 移動方向を初期化
	auto moveDir = Vector2::Zero;

	// 移動方向を入力
	const auto keyboard = Keyboard::GetInstance();
	if (keyboard.IsPressed(KEY_INPUT_W)) { moveDir.y -= 1.0f; }
	if (keyboard.IsPressed(KEY_INPUT_S)) { moveDir.y += 1.0f; }
	if (keyboard.IsPressed(KEY_INPUT_A)) { moveDir.x -= 1.0f; }
	if (keyboard.IsPressed(KEY_INPUT_D)) { moveDir.x += 1.0f; }

	// ベクトルを正規化
	// MEMO : 右下に移動した場合[x = 1, y = 1]で√2の長さを移動してしまい、移動速度が上昇します。
	//        正規化(長さを1にする)を行い移動速度を常に一定に保ちます。
	moveDir.Normalize();

	// velocity(移動方向・速度を持つベクトル)を計算
	// MEMO : デルタタイムを掛けてどの環境で動かしても速度が一定になるようにする。
	const auto velocity = moveDir * kSpeed * Time::GetInstance().GetDeltaTime();

	// 座標を更新
	_pos += velocity;
}

void Player::Shoot()
{
	// 入力していなかったら撃つ処理は行わない
	// MEMO : 「早期return」という
	if (!Keyboard::GetInstance().WasPressedThisFrame(KEY_INPUT_SPACE)) { return; }
	
	for (size_t i = 0; i < _bullets.size(); ++i)
	{
		// アクティブ状態の弾は既に撃たれているため無視する
		if (_bullets.at(i).IsActive()) { continue; }
		
		// 弾を撃つ
		_bullets.at(i).OnShoot(_pos, kShootDir, kShootSpeed);

		printfDx("弾のindex : %2d\n", i);

		// 弾を撃ったため、for文から抜ける
		// ※breakを書かないとすべての弾を撃ってしまう
		break;
	}

	/*
	// MEMO : 「範囲for文」で下記のようにも書ける！
	for (auto& bullet : _bullets)
	{
		if (bullet.IsActive()) { continue; }
		
		bullet.OnShoot(_pos, kShootDir, kShootSpeed);
		break;		
	}
	*/
}

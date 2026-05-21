#include <vector>
#include <math.hpp>
#include "window.h"
#include "time.h"
#include "keyboard.h"
#include "mouse.h"
#include "bullet.h"
#include "player.h"

Player::Player() : 
	_pos				(kFirstPosition),
	_shootIntervalTimer	(kShootInterval),
	_bullets			(kBulletNum)		// 指定のサイズを指定して初期化
{

}

void Player::Update()
{
	Move();
	Shoot();

	DrawFormatString(0, 0, 0xffffff, "%f", _shootIntervalTimer);

	// すべての弾を更新する
	for (size_t i = 0; i < _bullets.size(); ++i)
	{
		_bullets.at(i).Update();
	}
}

void Player::Render() const
{
	DrawCircle(static_cast<int>(_pos.x), static_cast<int>(_pos.y), kRadius, kColor);

	// すべての弾を描画する
	for (size_t i = 0; i < _bullets.size(); ++i)
	{
		_bullets.at(i).Render();
	}
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
	_pos += velocity;
}

void Player::Shoot()
{
	_shootIntervalTimer += Time::GetInstance().GetDeltaTime();

	// 入力していない場合は撃たない
	if (!Keyboard::GetInstance().IsPressed(KEY_INPUT_SPACE)) { return; }
	
	// インターバルに到達していない場合は撃たない
	if (_shootIntervalTimer < kShootInterval) { return; }
	
	// タイマーリセット (再度タイマーを加算するために)
	_shootIntervalTimer = 0.0f;
	
	// 撃てる弾(非アクティブの弾)を探し、1発のみ撃つ
	for (size_t i = 0; i < _bullets.size(); ++i)
	{
		// アクティブ状態の弾は既に撃たれているため無視する
		if (_bullets.at(i).IsActive()) { continue; }
		
		// 弾を撃つ
		_bullets.at(i).OnShoot(_pos, kShootDirection, kShootSpeed);

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

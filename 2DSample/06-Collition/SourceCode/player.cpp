#include <math.hpp>
#include "time.h"
#include "keyboard.h"
#include "transform.h"
#include "animator.h"
#include "aabb.h"
#include "player.h"

Player::Player() :
	_transform(Transform()),
	_animator(Animator(_transform)),
	_aabb(AABB(_transform, Vector2(10.0f, 15.0f), Vector2(0.0f, 0.5f)))

{
	_transform.position = kFirstPos;
	_transform.scale = kScale;

	_animator.LoadAnim(AnimationClip({}, "Idle", "../../Assets/Animations/Player/Idle.png", Vector2(1008, 144), 7, 0.2f, true, false));
	_animator.LoadAnim(AnimationClip({}, "Run", "../../Assets/Animations/Player/Run.png", Vector2(1152, 144), 8, 0.2f, true, false));
	_animator.AttachAnim("Idle");
}

void Player::Update()
{
	Move();
	_animator.Update();
	_aabb.Update();
}

void Player::Draw() const
{
	_animator.Draw();
	_aabb.Draw();
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
	_transform.position += velocity;
}

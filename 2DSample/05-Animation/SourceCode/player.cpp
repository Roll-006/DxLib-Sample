#include <math.hpp>
#include "time.h"
#include "transform.h"
#include "graphic.h"
#include "graphic_renderer.h"
#include "animator.h"
#include "player.h"

Player::Player() : 
	_transform		(Transform()),
	_graphicRenderer(GraphicRenderer(_transform)),
	_animator		(Animator(_transform, _graphicRenderer))
{
	_transform.position = kFirstPosition;
	_transform.scale	= kScale;

	_animator.LoadAnim("../../Assets/Animations/Player/Idle.png", AnimationClip("Idle", 7, 0.2f, true, false));
	_animator.LoadAnim("../../Assets/Animations/Player/Run.png",  AnimationClip("Run",  8, 0.2f, true, false));
	_animator.AttachAnim("Idle");
}

void Player::Update()
{
	Move();
	_animator.Update();
}

void Player::Render() const
{
	_graphicRenderer.Render();
}

void Player::Move()
{
	// このサンプルでは、DirectXMathのラップーライブラリを使用しています。
	// DXライブラリの演算は計算が不足しており不便なことが理由です。

	// 移動方向を初期化
	auto moveDirection = Vector2::Zero;

	// 移動方向を入力
	if (CheckHitKey(KEY_INPUT_W)) { moveDirection.y -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_S)) { moveDirection.y += 1.0f; }
	if (CheckHitKey(KEY_INPUT_A)) { moveDirection.x -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_D)) { moveDirection.x += 1.0f; }

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

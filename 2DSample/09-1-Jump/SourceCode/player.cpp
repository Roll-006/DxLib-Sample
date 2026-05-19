#include <math.hpp>
#include "window.h"
#include "time.h"
#include "keyboard.h"
#include "physics.h"
#include "transform.h"
#include "graphic.h"
#include "graphic_renderer.h"
#include "animator.h"
#include "aabb.h"
#include "player.h"

Player::Player() :
	_transform		(Transform()),
	_isGrounded		(false),
	_graphicRenderer(GraphicRenderer(_transform)),
	_animator		(Animator(_transform, _graphicRenderer)),
	_aabb			(AABB(_transform, Vector2(10.0f, 15.0f), Vector2(0.0f, 0.5f)))

{
	_transform.position = kFirstPos;
	_transform.scale	= kScale;

	_animator.LoadAnim("../../Assets/Animations/Player/Idle.png", AnimationClip("Idle", 7, 0.2f, true, false));
	_animator.LoadAnim("../../Assets/Animations/Player/Run.png",  AnimationClip("Run",  8, 0.2f, true, false));
	_animator.AttachAnim("Idle");
}

void Player::Update()
{
	ApplyGravity();
	Move();
	Jump();
	UpdatePos();

	_animator.Update();
	_aabb.Update();

	UpdateGroundedState();
}

void Player::Draw() const
{
	_graphicRenderer.Draw();
	_aabb.Draw();
}

void Player::ApplyGravity()
{
	// 地面に立っている場合は重力を与えない
	if (_isGrounded) { return; }

	// 落下velocityに重力を加算する
	_fallVelocity.y += physics::kGravity * Time::GetInstance().GetDeltaTime();

	// 最大の重力加速度を設定
	if (_fallVelocity.y > physics::kMaxGravityAccel)
	{
		_fallVelocity.y = physics::kMaxGravityAccel;
	}
}

void Player::UpdateGroundedState()
{
	// 画面下にコライダーが衝突した場合、地面に着地したものとする
	// ※簡易的な衝突判定のため、押し戻しは行っていない
	if (_aabb.GetMax().y <= window::kSize.y) { return; }
	
	_fallVelocity.y = 0.0f;
	_isGrounded = true;
}

void Player::Move()
{
	// このサンプルでは、DirectXMathのラップーライブラリを使用しています。
	// DXライブラリの演算は計算が不足しており不便なことが理由です。

	// 移動方向を初期化
	auto moveDir = Vector2::Zero;

	// 移動方向を入力
	const auto keyboard = Keyboard::GetInstance();
	if (keyboard.IsPressed(KEY_INPUT_A)) { moveDir.x -= 1.0f; }
	if (keyboard.IsPressed(KEY_INPUT_D)) { moveDir.x += 1.0f; }

	// 移動velocity(移動方向・速度を持つベクトル)を計算
	_moveVelocity = moveDir * kSpeed;
}

void Player::Jump()
{
	// 地面に立っていない場合はジャンプさせない
	if (!_isGrounded) { return; }

	if (!Keyboard::GetInstance().WasPressedThisFrame(KEY_INPUT_SPACE)) { return; }
	
	// 落下velocityにジャンプ量を与える
	// MEMO : 初速度さえ与えれば、重力がかかるため自動的に落下していく
	_fallVelocity.y = kJumpPower;

	// 地面から離れる
	_isGrounded = false;
}

void Player::UpdatePos()
{
	const auto velocity = Vector2(_moveVelocity.x, _fallVelocity.y) * Time::GetInstance().GetDeltaTime();
	_transform.position += velocity;
}

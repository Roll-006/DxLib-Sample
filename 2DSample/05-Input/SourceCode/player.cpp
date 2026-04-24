#include <DxLib.h>
#include <Vector/vector2.hpp>
#include <Vector/vector2_int.hpp>
#include "time.h"
#include "keyboard.h"
#include "transform.h"
#include "animator.h"
#include "player.h"

Player::Player() : 
	_transform	(Transform()),
	_animator	(Animator(_transform))
{
	_transform.position = kFirstPos;
	_transform.scale	= kScale;

	_animator.LoadAnim(AnimationClip({}, "Idle", "Assets/Animations/Player/Idle.png", Vector2(1008, 144), 7, 0.2f, true, false));
	_animator.LoadAnim(AnimationClip({}, "Run",  "Assets/Animations/Player/Run.png",  Vector2(1152, 144), 8, 0.2f, true, false));
	_animator.AttachAnim("Idle");
}

void Player::Update()
{
	Move();
	_animator.Update();
}

void Player::Draw() const
{
	_animator.Draw();
}

void Player::Move()
{
	// ベクトルの計算では、「演算子のオーバーロード」を使用することをおすすめします！
	// 無駄な情報がなくなり読みやすいコードになります！

	// 移動方向を初期化
	auto moveDir = Vector2(0.0f, 0.0f);

	// 移動方向を入力
	if (Keyboard::GetInstance().IsPressed(KEY_INPUT_W)) { moveDir.y -= 1.0f; }
	if (Keyboard::GetInstance().IsPressed(KEY_INPUT_S)) { moveDir.y += 1.0f; }
	if (Keyboard::GetInstance().IsPressed(KEY_INPUT_A)) { moveDir.x -= 1.0f; }
	if (Keyboard::GetInstance().IsPressed(KEY_INPUT_D)) { moveDir.x += 1.0f; }

	// ベクトルを正規化
	// MEMO : 右下に移動した場合[x = 1, y = 1]で√2の長さを移動してしまい、移動速度が上昇します。
	//        正規化(長さを1にする)を行い移動速度を常に一定に保ちます。
	moveDir.Normalize();

	// velocity(移動方向・速度を持つベクトル)を計算
	const auto velocity = moveDir * kSpeed * Time::GetInstance().GetDeltaTime();

	// 座標を更新
	_transform.position += velocity;


	/*演算子のオーバーロードを使用しない場合 (_transform.positionはVECTOR型に変更する必要あり)
	auto moveDir = VECTOR(0.0f, 0.0f, 0.0f);

	// 移動方向を入力
	if (CheckHitKey(KEY_INPUT_W)) { moveDir.y -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_S)) { moveDir.y += 1.0f; }
	if (CheckHitKey(KEY_INPUT_A)) { moveDir.x -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_D)) { moveDir.x += 1.0f; }

	// ベクトルを正規化
	if (VSize(moveDir) != 0.0f)
	{
		moveDir = VNorm(moveDir);
	}

	// VScale : ベクトルの長さを変える
	const auto velicity = VScale(moveDir, kSpeed);

	// 座標を更新
	// VAdd : ベクトルの足し算を行う
	_transform.position = VAdd(_transform.position, velicity);
	*/
}

#include <DxLib.h>
#include "time.h"
#include "player.h"

Player::Player() : 
	_pos(kFirstPos)
{

}

Player::~Player()
{

}

void Player::Update()
{
	Move();
}

void Player::Draw() const
{
	DrawCircle(static_cast<int>(_pos.x), static_cast<int>(_pos.y), kRadius, kColor);
}

void Player::Move()
{
	// ベクトルの計算では、「演算子のオーバーロード」を使用することをおすすめします！
	// 無駄な情報がなくなり読みやすいコードになります！

	// 移動方向を初期化
	auto move_dir = Vector2(0.0f, 0.0f);

	// 移動方向を入力
	if (CheckHitKey(KEY_INPUT_W)) { move_dir.y -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_S)) { move_dir.y += 1.0f; }
	if (CheckHitKey(KEY_INPUT_A)) { move_dir.x -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_D)) { move_dir.x += 1.0f; }

	// ベクトルを正規化
	// MEMO : 右下に移動した場合[x = 1, y = 1]で√2の長さを移動してしまい、移動速度が上昇します。
	//        正規化(長さを1にする)を行い移動速度を常に一定に保ちます。
	move_dir.Normalize();

	// velocity(移動方向・速度を持つベクトル)を計算
	const auto velocity = move_dir * kSpeed * Time::GetInstance().GetDeltaTime();

	// 座標を更新
	_pos += velocity;


	/*演算子のオーバーロードを使用しない場合 (_posはVECTOR型に変更する必要あり)
	auto move_dir = VECTOR(0.0f, 0.0f, 0.0f);

	// 移動方向を入力
	if (CheckHitKey(KEY_INPUT_W)) { move_dir.y -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_S)) { move_dir.y += 1.0f; }
	if (CheckHitKey(KEY_INPUT_A)) { move_dir.x -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_D)) { move_dir.x += 1.0f; }

	// ベクトルを正規化
	if (VSize(move_dir) != 0.0f)
	{
		move_dir = VNorm(move_dir);
	}

	// VScale	: ベクトルの長さを変える
	const auto velicity = VScale(move_dir, kSpeed);

	// 座標を更新
	// VAdd : ベクトルの足し算を行う
	_pos = VAdd(_pos, velicity);
	*/
}

#include <DxLib.h>
#include <string>
#include <array>
#include <Vector/vector3.hpp>
#include <Matrix/matrix_4x4.hpp>
#include "time.h"
#include "keyboard.h"
#include "transform.h"
#include "modeler.h"
#include "player.h"

Player::Player() : 
	_transform	(Transform()),
	_modeler	(Modeler("../../Assets/Models/Swat/mesh.mv1", _transform)), 
	_frameIndex	(MV1SearchFrame(_modeler.GetModelHandle(), "mixamorig:Neck1")),
	_target		(0, 15, 7)
{
	_transform.SetScale(0.1f);
}

void Player::Update()
{
	// TODO : ボーンの進行方向とupを指定できるようにする必要あり
	
	const auto modelHandle = _modeler.GetModelHandle();

	// 回転させるボーンの行列を取得
	auto frameWorldMat = static_cast<Matrix4x4>(MV1GetFrameLocalWorldMatrix(modelHandle, _frameIndex));

	// ターゲットを移動
	auto moveDir = Vector3::GetZero();
	const auto keyboard = Keyboard::GetInstance();
	if (keyboard.IsPressed(KEY_INPUT_LEFT))	{ moveDir.x -= 1; }
	if (keyboard.IsPressed(KEY_INPUT_RIGHT)){ moveDir.x += 1; }
	if (keyboard.IsPressed(KEY_INPUT_DOWN))	{ moveDir.y -= 1; }
	if (keyboard.IsPressed(KEY_INPUT_UP))	{ moveDir.y += 1; }
	if (keyboard.IsPressed(KEY_INPUT_W))	{ moveDir.z += 1; }
	if (keyboard.IsPressed(KEY_INPUT_S))	{ moveDir.z -= 1; }
	moveDir.Normalize();
	_target += moveDir * 20.0f * Time::GetInstance().GetDeltaTime();

	// 対象の方向を見る
	frameWorldMat = Matrix4x4::LookAt(frameWorldMat.GetTranslation(), _target, Vector3::GetUp());

	// ローカル行列に変換
	const auto parentWorldRMat	= static_cast<Matrix4x4>(MV1GetFrameLocalWorldMatrix(modelHandle, MV1GetFrameParent(modelHandle, _frameIndex))).GetRMatrix();
	const auto frameLocalRMat	= frameWorldMat.GetRMatrix() * MInverse(parentWorldRMat);

	// ローカル行列に回転行列を設定
	auto frameLocalMat = static_cast<Matrix4x4>(MV1GetFrameLocalMatrix(modelHandle, _frameIndex));
	frameLocalMat.SetRotation(frameLocalRMat);

	// ローカル行列として適用
	MV1SetFrameUserLocalMatrix(modelHandle, _frameIndex, frameLocalMat);
}

void Player::Draw() const
{
	_modeler.Draw();

	const auto frameWorldMat = static_cast<Matrix4x4>(MV1GetFrameLocalWorldMatrix(_modeler.GetModelHandle(), _frameIndex));
	DrawFormatString(0,   0, 0xffffff, "↑ : 上へ移動");
	DrawFormatString(0,  20, 0xffffff, "↓ : 下へ移動");
	DrawFormatString(0,  40, 0xffffff, "→ : 右へ移動");
	DrawFormatString(0,  60, 0xffffff, "← : 左へ移動");
	DrawFormatString(0,  80, 0xffffff, "W  : 奥へ移動");
	DrawFormatString(0, 100, 0xffffff, "D  : 手前へ移動");
	DrawFormatString(0, 120, 0xffffff, "ターゲット座標 : %f, %f, %f", _target.x, _target.y, _target.z);
	DrawSphere3D(_target, 0.5f, 8, 0xffffff, 0xffffff, TRUE);
	DrawLine3D(frameWorldMat.GetTranslation(), frameWorldMat.GetTranslation() + frameWorldMat.GetRight()	* 10, GetColor(255,   0,   0));
	DrawLine3D(frameWorldMat.GetTranslation(), frameWorldMat.GetTranslation() + frameWorldMat.GetUp()		* 10, GetColor(  0, 255,   0));
	DrawLine3D(frameWorldMat.GetTranslation(), frameWorldMat.GetTranslation() + frameWorldMat.GetForward()	* 10, GetColor(  0,   0, 255));
}

#include <DxLib.h>
#include <Vector/vector2.hpp>
#include <Vector/vector2_int.hpp>
#include "time.h"
#include "keyboard.h"
#include "mouse.h"
#include "transform.h"
#include "animator.h"
#include "player.h"
#include "game.h"

int Game::Run() const
{
	if (auto setup_result = SetUpDxLib(); setup_result != EXIT_SUCCESS)
	{
		return setup_result;
	}

	auto player = Player();

	while (ShouldRun())
	{
		Time	::GetInstance().Update();
		Keyboard::GetInstance().Update();
		Mouse	::GetInstance().Update();

		player.Update();
		player.Draw();

		DrawFormatString(0,   0, 0xffffff, "1を長押し : %d",					Keyboard::GetInstance().IsPressed(KEY_INPUT_1));
		DrawFormatString(0,  20, 0xffffff, "2をこのフレームに入力した : %d",	Keyboard::GetInstance().WasPressedThisFrame(KEY_INPUT_2));
		DrawFormatString(0,  40, 0xffffff, "3をこのフレームで離した : %d",		Keyboard::GetInstance().WasReleasedThisFrame(KEY_INPUT_3));
		DrawFormatString(0,  60, 0xffffff, "4の入力カウント : %d",				Keyboard::GetInstance().GetInputCount(KEY_INPUT_4));
		DrawFormatString(0,  80, 0xffffff, "マウスの移動量 : %d, %d",			Mouse::GetInstance().GetDelta().x, Mouse::GetInstance().GetDelta().y);
		DrawCircle(Mouse::GetInstance().GetPos().x, Mouse::GetInstance().GetPos().y, 5, 0xffffff);

		Time::GetInstance().CapFPS();

		// 裏画面を表画面に反映
		ScreenFlip();
	}
}

int Game::SetUpDxLib() const
{
	SetGraphMode(1920, 1080, 32);						// 解像度とカラービット数を設定
	ChangeWindowMode(FALSE);							// ウィンドウモードを設定 (true : ウィンドウ, false : フルスクリーン)
	if (DxLib_Init() == -1) { return EXIT_FAILURE; }	// DXライブラリを初期化 (0 : 成功, -1 : 失敗)
	SetDrawScreen(DX_SCREEN_BACK);						// 描画先画面を裏画面に設定
	SetMainWindowText("Sample");						// ウインドウのタイトルを設定する

	return EXIT_SUCCESS;
}

bool Game::ShouldRun() const
{
	if (ProcessMessage()  != 0)			{ return false; }		// ウインドウメッセージの処理を行う
	if (ClearDrawScreen() != 0)			{ return false; }		// 画面に描画されたものを削除する
	if (CheckHitKey(KEY_INPUT_ESCAPE))	{ return false; }		// ESCAPEが入力された場合は終了する

	return true;
}

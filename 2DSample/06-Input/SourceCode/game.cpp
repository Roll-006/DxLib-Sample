#include <math.hpp>
#include "window.h"
#include "time.h"
#include "keyboard.h"
#include "mouse.h"
#include "transform.h"
#include "graphic.h"
#include "graphic_renderer.h"
#include "animator.h"
#include "player.h"
#include "game.h"

int Game::Run() const
{
	if (auto setupResult = SetUpDxLib(); setupResult != EXIT_SUCCESS)
	{
		return setupResult;
	}

	auto player = Player();

	while (ShouldRun())
	{
		Time	::GetInstance().Update();
		Keyboard::GetInstance().Update();
		Mouse	::GetInstance().Update();

		player.Update();
		player.Render();

		const auto keyboard = Keyboard	::GetInstance();
		const auto mouse	= Mouse		::GetInstance();
		DrawFormatString(0,   0, 0xffffff, "１を長押しした　　　　　　 : %d",		keyboard.IsPressed(KEY_INPUT_1));
		DrawFormatString(0,  20, 0xffffff, "２をこのフレームに入力した : %d",		keyboard.WasPressedThisFrame(KEY_INPUT_2));
		DrawFormatString(0,  40, 0xffffff, "３をこのフレームで離した　 : %d",		keyboard.WasReleasedThisFrame(KEY_INPUT_3));
		DrawFormatString(0,  60, 0xffffff, "４の入力カウント　　　　　 : %d",		keyboard.GetInputCount(KEY_INPUT_4));
		DrawFormatString(0,  80, 0xffffff, "左クリックを長押しした　　 : %d",		mouse.IsPressed(MOUSE_INPUT_LEFT));
		DrawFormatString(0, 100, 0xffffff, "マウスの位置　　　　　　　 : %f, %f",	mouse.GetPosition().x, mouse.GetPosition().y);
		DrawFormatString(0, 120, 0xffffff, "マウスの移動量　　　　　　 : %f, %f",	mouse.GetDelta().x, mouse.GetDelta().y);
		DrawFormatString(0, 140, 0xffffff, "マウスホイールの移動量　　 : %f, %f",	mouse.GetScroll().x, mouse.GetScroll().y);

		Time::GetInstance().CapFPS();

		// 裏画面を表画面に反映
		ScreenFlip();
	}

	return EXIT_SUCCESS;
}

int Game::SetUpDxLib() const
{
	SetGraphMode(static_cast<int>(window::kSize.x), static_cast<int>(window::kSize.y), window::kColorBitDepth);		// 解像度とカラービット数を設定
	ChangeWindowMode(FALSE);							// ウィンドウモードを設定 (true : ウィンドウ, false : フルスクリーン)
	if (DxLib_Init() == -1) { return EXIT_FAILURE; }	// DXライブラリを初期化 (0 : 成功, -1 : 失敗)
	SetDrawScreen(DX_SCREEN_BACK);						// 描画先画面を裏画面に設定
	SetMainWindowText("Sample");						// ウインドウのタイトルを設定する

	return EXIT_SUCCESS;
}

bool Game::ShouldRun() const
{
	if (ProcessMessage()  != 0) { return false; }		// ウインドウメッセージの処理を行う
	if (ClearDrawScreen() != 0)	{ return false; }		// 画面に描画されたものを削除する
	if (Keyboard::GetInstance().WasPressedThisFrame(KEY_INPUT_ESCAPE)) { return false; }	// ESCAPEが入力された場合は終了する

	return true;
}

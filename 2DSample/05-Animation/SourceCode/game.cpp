#include <math.hpp>
#include "window.h"
#include "time.h"
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
		Time::GetInstance().Update();

		player.Update();
		player.Render();

		Time::GetInstance().CapFPS();

		// 裏画面を表画面に反映
		ScreenFlip();
	}

	return EXIT_SUCCESS;
}

int Game::SetUpDxLib() const
{
	SetGraphMode(static_cast<int>(window::kSize.x), static_cast<int>(window::kSize.y), window::kColorBitDepth);		// 解像度とカラービット数を設定
	ChangeWindowMode(FALSE);								// ウィンドウモードを設定 (true : ウィンドウ, false : フルスクリーン)
	if (DxLib_Init() == -1) { return EXIT_FAILURE; }		// DXライブラリを初期化 (0 : 成功, -1 : 失敗)
	SetDrawScreen(DX_SCREEN_BACK);							// 描画先画面を裏画面に設定
	SetMainWindowText("Sample");							// ウインドウのタイトルを設定する

	return EXIT_SUCCESS;
}

bool Game::ShouldRun() const
{
	if (ProcessMessage()  != 0)			{ return false; }	// ウインドウメッセージの処理を行う
	if (ClearDrawScreen() != 0)			{ return false; }	// 画面に描画されたものを削除する
	if (CheckHitKey(KEY_INPUT_ESCAPE))	{ return false; }	// ESCAPEが入力された場合は終了する

	return true;
}

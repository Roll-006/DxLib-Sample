#include <DxLib.h>
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
		player.Update();
		player.Draw();

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

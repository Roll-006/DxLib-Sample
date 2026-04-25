#include <DxLib.h>
#include "game.h"

int Game::Run() const
{
	if (auto setup_result = SetUpDxLib(); setup_result != EXIT_SUCCESS)
	{
		return setup_result;
	}

	while (ShouldRun())
	{
		// 四角形を描画
		// x1, y1			: 左上座標
		// x2, y2			: 右下座標
		// Color			: 色 (16進数[0xffffff]もしくはGetColor関数[GetColor(255, 255, 255)])
		// FillFlag			: 四角形を塗りつぶすかどうか
		// LineThickness	: 枠の太さ (初期値は1)
		DrawBox(200, 200, 400, 400, GetColor(219, 91, 75), TRUE);

		// 三角形を描画
		// x1, y1			: 頂点1
		// x2, y2			: 頂点1
		// x3, y3			: 頂点2
		// Color			: 色 (16進数[0xffffff]もしくはGetColor関数[GetColor(255, 255, 255)])
		// FillFlag			: 四角形を塗りつぶすかどうか
		DrawTriangle(300, 600, 200, 700, 400, 800, 0x52d158, TRUE);

		// 円を描画
		// x, y				: 中心座標
		// r				: 半径
		// Color			: 色 (16進数[0xffffff]もしくはGetColor関数[GetColor(255, 255, 255)])
		// FillFlag			: 四角形を塗りつぶすかどうか
		// LineThickness	: 枠の太さ (初期値は1)
		DrawCircle(1000, 500, 300, 0x41d6d9, FALSE, 5);

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

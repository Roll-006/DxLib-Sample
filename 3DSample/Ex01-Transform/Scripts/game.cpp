#include <string>
#include <vector>
#include <DxLib.h>
#include <JSON/json_loader.hpp>
#include <Matrix/matrix_4x4.hpp>
#include <Vector/vector2.hpp>
#include <Vector/vector2_int.hpp>
#include "math.h"
#include "time.h"
#include "keyboard.h"
#include "mouse.h"
#include "transform.h"
#include "../GameObjects/AimTarget/aim_target.h"
#include "../GameObjects/Player/player.h"
#include "../GameObjects/MainCamera/main_camera.h"
#include "../Scenes/DemoScene/demo_scene.h"
#include "game.h"

int Game::Run() const
{
	if (auto setupResult = SetUpDxLib(); setupResult != EXIT_SUCCESS)
	{
		return setupResult;
	}

	auto demoScene = std::make_shared<DemoScene>();
	demoScene->Initialize();

	while (ShouldRun())
	{
		Time	::GetInstance().Update();
		Keyboard::GetInstance().Update();
		Mouse	::GetInstance().Update();

		demoScene->Update();
		demoScene->LateUpdate();
		demoScene->Render();

		Time::GetInstance().CapFPS();

		// 裏画面を表画面に反映
		ScreenFlip();
	}

	return EXIT_SUCCESS;
}

int Game::SetUpDxLib() const
{
	SetGraphMode(1920, 1080, 32);						// 解像度とカラービット数を設定
	ChangeWindowMode(FALSE);							// ウィンドウモードを設定 (true : ウィンドウ, false : フルスクリーン)
	if (DxLib_Init() == -1) { return EXIT_FAILURE; }	// DXライブラリを初期化 (0 : 成功, -1 : 失敗)
	SetDrawScreen(DX_SCREEN_BACK);						// 描画先画面を裏画面に設定
	SetMainWindowText("Sample");						// ウインドウのタイトルを設定する

	SetUseZBuffer3D(TRUE);								// Zバッファを使用する
	SetWriteZBuffer3D(TRUE);							// Zバッファに書き込む

	return EXIT_SUCCESS;
}

bool Game::ShouldRun() const
{
	if (ProcessMessage()  != 0) { return false; }		// ウインドウメッセージの処理を行う
	if (ClearDrawScreen() != 0)	{ return false; }		// 画面に描画されたものを削除する
	if (Keyboard::GetInstance().WasPressedThisFrame(KEY_INPUT_ESCAPE)) { return false; }	// ESCAPEが入力された場合は終了する

	return true;
}

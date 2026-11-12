#include <json_loader.hpp>
#include <math.hpp>
#include <DxLibSample/game_time.h>
#include <DxLibSample/screen.h>
#include <DxLibSample/Input/keyboard.h>
#include <DxLibSample/Input/mouse.h>
#include "../Scenes/demo_scene.h"
#include "game.h"

using namespace DxLibSample;
using namespace Scenes;

int Game::Run() const
{
	if (auto setupResult = SetUpDxLib(); setupResult != EXIT_SUCCESS) return setupResult;
	
	auto demoScene = std::make_shared<DemoScene>();
	demoScene->Initialize();
	demoScene->Start();

	while (ShouldRun())
	{
		GameTime::I().Update();
		Input::Keyboard::I().Update();
		Input::Mouse::I().Update();
		Screen::I().UpdateScreenMode();

		demoScene->Update();
		demoScene->LateUpdate();
		demoScene->Render();

		GameTime::I().CapFPS();

		// 裏画面を表画面に反映
		ScreenFlip();
	}

	return EXIT_SUCCESS;
}

int Game::SetUpDxLib() const
{
	SetGraphMode(static_cast<int>(Screen::I().GetScreenSize().x), static_cast<int>(Screen::I().GetScreenSize().y), Screen::I().kColorBitDepth);	// 解像度とカラービット数を設定
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);	// ウィンドウモードを設定した際にグラフィックシステムをリセットするかを設定
	ChangeWindowMode(TRUE);							// ウィンドウモードを設定 (true : ウィンドウ, false : フルスクリーン)
	if (DxLib_Init() == -1) return EXIT_FAILURE;		// DXライブラリを初期化 (0 : 成功, -1 : 失敗)
	SetDrawScreen(DX_SCREEN_BACK);						// 描画先画面を裏画面に設定
	SetMainWindowText("Sample");						// ウインドウのタイトルを設定する

	SetUseZBuffer3D(TRUE);							// Zバッファを使用する
	SetWriteZBuffer3D(TRUE);						// Zバッファに書き込む

	return EXIT_SUCCESS;
}

bool Game::ShouldRun() const
{
	if (ProcessMessage()  != 0) return false;		// ウインドウメッセージの処理を行う
	if (ClearDrawScreen() != 0) return false;		// 画面に描画されたものを削除する
	if (Input::Keyboard::I().WasPressedThisFrame(KEY_INPUT_ESCAPE)) return false;	// ESCAPEが入力された場合は終了する

	return true;
}
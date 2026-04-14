/*
「Sample-03　デルタタイム」

内容 : デルタタイムを実装し、全てのPCで同じゲームスピードを保ちます
*/

#include <DxLib.h>
#include <Vector/vector_3d.hpp>

// 関数プロトタイプ宣言
void SetUpGameSystem();
const bool ShouldRun();
void Run();

// 定数
constexpr float kFPS			= 60.0f;
constexpr float kMaxWaitTimeMs	= 1000;
constexpr float kMs				= 1000.0f;		// ミリ秒
constexpr float kUs				= 1000000.0f;	// マイクロ秒

/// <summary>
/// メイン関数
/// </summary>
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	SetUpGameSystem();
	Run();

    return 0;
}

/// <summary>
/// ゲームの設定を行う
/// </summary>
void SetUpGameSystem()
{
	SetGraphMode(1920, 1080, 32);						// 解像度とカラービット数を設定
	ChangeWindowMode(FALSE);							// ウィンドウモードを設定 (true : ウィンドウ, false : フルスクリーン)
	if (DxLib_Init() == -1) { exit(EXIT_FAILURE); }		// DXライブラリを初期化 (0 : 成功, -1 : 失敗)
	SetDrawScreen(DX_SCREEN_BACK);						// 描画先画面を裏画面に設定
	SetMainWindowText("Sample");						// ウインドウのタイトルを設定する
}

/// <summary>
/// ゲームループを続けるかを判定
/// </summary>
/// <returns>true : 継続, false : 終了</returns>
const bool ShouldRun()
{
	if (ProcessMessage()  != 0) { return false; }		// ウインドウメッセージの処理を行う
	if (ClearDrawScreen() != 0) { return false; }		// 画面に描画されたものを削除する

	return true;
}

/// <summary>
/// ゲームループ処理を行う
/// </summary>
void Run()
{
	auto pos = VGet(200.0f, 200.0f, 0.0f);

	auto firstTimeUs	= GetNowHiPerformanceCount();
	auto currentTimeUs	= firstTimeUs;
	auto prevTimeUs		= firstTimeUs;
	auto frameCount		= 1;
	auto averageFps		= 0.0f;
	auto deltaTime		= 0.0f;
	auto elapsedTime	= 0.0f;

	while (ShouldRun())
	{
		// デルタタイム計算
		{
			currentTimeUs	= GetNowHiPerformanceCount();			// Windowsが起動してからの経過時間を取得
			deltaTime		= (currentTimeUs - prevTimeUs) / kUs;	// 1フレーム前との差分(デルタタイム)を計算 (マイクロ秒で割って単位を秒にする)
			prevTimeUs		= currentTimeUs;						// 現在の値を保存

			// フレームカウントが目的のFPSに到達した場合、カウントをリセットする
			if (frameCount == kFPS)
			{
				// 表示のため、平均FPSを計算する
				const auto totalFrameTime	= static_cast<float>(currentTimeUs - firstTimeUs);
				const auto calcAverage		= totalFrameTime / kFPS;
				averageFps = kUs / calcAverage;

				// カウントリセット
				firstTimeUs = GetNowHiPerformanceCount();
				frameCount	= 1;
			}
			else
			{
				++frameCount;
			}

			// 経過時間を取得
			// MEMO : デルタタイムの加算はそのまま経過時間になる
			elapsedTime += deltaTime;
		}


		// プレイヤーの移動
		{
			// 移動方向を初期化
			auto moveDir = VGet(0.0f, 0.0f, 0.0f);

			// 移動方向を入力
			if (CheckHitKey(KEY_INPUT_W)) { moveDir.y -= 1; }
			if (CheckHitKey(KEY_INPUT_S)) { moveDir.y += 1; }
			if (CheckHitKey(KEY_INPUT_A)) { moveDir.x -= 1; }
			if (CheckHitKey(KEY_INPUT_D)) { moveDir.x += 1; }

			// ベクトルを正規化
			moveDir = v3d::GetNormalizedV(moveDir);

			// 座標を更新
			const auto velocity = moveDir * 500.0f * deltaTime;
			pos += velocity;
		}


		// 描画
		DrawCircle(pos.x, pos.y, 100, 0xffffff, TRUE);

		// 経過時間を表示
		DrawFormatString(0,  0, 0xffffff, "FPS : %f", averageFps);
		DrawFormatString(0, 20, 0xffffff, "デルタタイム : %f", deltaTime);
		DrawFormatString(0, 40, 0xffffff, "経過時間 : %f", elapsedTime);


		// 待機時間計算
		{
			// 理想の時間と実際の時間の差分を取得し、時間を停止させる
			const auto idealTimeUs	= kUs / kFPS * frameCount;
			const auto actualTimeUs	= currentTimeUs - firstTimeUs;
			const auto waitTimeMs	= (idealTimeUs - actualTimeUs) / kMs;

			// 指定の時間動作を停止
			if (waitTimeMs > 0 && waitTimeMs <= kMaxWaitTimeMs)
			{
				WaitTimer(static_cast<int>(waitTimeMs));
			}
		}

		// 裏画面を表画面に反映
		ScreenFlip();
	}
}

/*
「Sample-02　移動」

内容	：ベクトルを用いてキャラクターを操作します！
ポイント：正規化を使用して、斜め移動で移動が速くなる問題を解決する
*/

#include <DxLib.h>
#include <Vector/vector_3d.hpp>

// 関数プロトタイプ宣言
void SetUpGameSystem();
const bool ShouldRun();
void Run();

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
	auto pos = VGet(100.0f, 100.0f, 0.0f);
	auto speed = 5.0f;

	while (ShouldRun())
	{
		// 移動方向を初期化
		auto move_dir = VGet(0.0f, 0.0f, 0.0f);

		// 移動方向を入力
		if (CheckHitKey(KEY_INPUT_W)) { move_dir.y -= 1; }
		if (CheckHitKey(KEY_INPUT_S)) { move_dir.y += 1; }
		if (CheckHitKey(KEY_INPUT_A)) { move_dir.x -= 1; }
		if (CheckHitKey(KEY_INPUT_D)) { move_dir.x += 1; }

		// ベクトルを正規化
		// MEMO : 右下に移動した場合[x = 1, y = 1]で√2の長さを移動してしまい、移動速度が上昇します。
		//        そのため、正規化(長さを1にする)を行い移動速度を常に一定に保ちます。
		move_dir = v3d::GetNormalizedV(move_dir);

		// 座標を更新
		// VAdd		: ベクトルの足し算を行う
		// VScale	: ベクトルの長さを変える
		pos = VAdd(pos, VScale(move_dir, speed));

		// MEMO : 通常は[pos = VAdd(pos, VScale(move_dir, speed));]と書く必要があるが、
		//        演算子のオーバーロードを使用してint型のような計算が可能です。
		// pos += move_dir * speed;

		// 描画
		DrawCircle(pos.x, pos.y, 100, 0xffffff, TRUE);

		// 裏画面を表画面に反映
		ScreenFlip();
	}
}

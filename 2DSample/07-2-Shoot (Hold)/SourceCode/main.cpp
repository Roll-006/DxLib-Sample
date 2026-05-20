/*
「長押しで弾を撃つ」

内容     : 長押しで弾を撃つ処理を作成します！
ポイント : 発射する間隔(インターバル)を追加して実装します！
*/

#include <DxLib.h>
#include "game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	auto game = Game();

	return game.Run();
}

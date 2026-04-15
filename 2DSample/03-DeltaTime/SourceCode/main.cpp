/*
「Sample-02　移動」

内容	：ベクトルを用いてキャラクターを操作します！
ポイント：正規化を使用して、斜め移動で移動が速くなる問題を解決する
*/

#include <DxLib.h>
#include "game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	auto game = Game();

	return game.Run();
}

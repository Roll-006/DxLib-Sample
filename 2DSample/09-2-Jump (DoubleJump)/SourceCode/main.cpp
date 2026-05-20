/*
「二段ジャンプ」

内容     : 二段ジャンプを実装します！
ポイント : ジャンプの回数を指定できるようにして、汎用的なコードにします！
*/

#include <DxLib.h>
#include "game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	auto game = Game();

	return game.Run();
}

/*
「ジャンプ」

内容     : 基本的な動作であるジャンプを実装します！
ポイント : 重力を実装して、放物線を描きながら落下するほうにします！
*/

#include <DxLib.h>
#include "game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	auto game = Game();

	return game.Run();
}

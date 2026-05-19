/*
「Sample-06　弾を撃つ」

内容     : 弾を撃つ処理を作成します！
ポイント : std::vectorで弾を管理し、大量の弾を扱いやすくします！
*/

#include <DxLib.h>
#include "game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	auto game = Game();

	return game.Run();
}

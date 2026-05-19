/*
「Sample-05　入力」

内容	 : 入力用のクラスを作成して入力を便利にする！
ポイント : DxLibの関数を活用して、長押しや単押しなどを区別できるようにします！
*/

#include <DxLib.h>
#include "game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	auto game = Game();

	return game.Run();
}

/*
「Sample-01　描画」

内容 : DxLibの関数を使用して図形の描画を行います！
*/

#include <DxLib.h>
#include "game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	auto game = Game();
	
    return game.Run();
}

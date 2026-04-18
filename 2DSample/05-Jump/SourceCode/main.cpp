/*
「Sample-04　アニメーション」

内容	 : アニメーションでキャラクターを動かします！
ポイント : 画像の読み込みや描画の方法を知っていきましょう！
*/

#include <DxLib.h>
#include "game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	auto game = Game();

	return game.Run();
}

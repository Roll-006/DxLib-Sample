#include <array>
#include <DxLib.h>
#include "keyboard.h"

Keyboard::Keyboard() :
	_state(0)
{

}

void Keyboard::Update()
{
	// すべてのキーの入力状態を保存する
	GetHitKeyStateAllEx(_state.data());
}

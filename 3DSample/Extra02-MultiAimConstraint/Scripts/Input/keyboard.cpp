#include <array>
#include <DxLib.h>
#include "keyboard.h"

using namespace Input;

Keyboard::Keyboard()
{
	
}

void Keyboard::Update()
{
	// すべてのキーの入力状態を保存する
	GetHitKeyStateAllEx(_state.data());
}
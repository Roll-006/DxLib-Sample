#include <array>
#include <DxLib.h>
#include <Vector/vector2.hpp>
#include <Vector/vector2_int.hpp>
#include "input.h"

Input::Input() :
	_keyState		(0),
	_currentMousePos(0, 0),
	_prevMousePos	(0, 0),
	_mouseDelta		(0, 0)
{
	GetMousePoint(&_currentMousePos.x, &_currentMousePos.y);
	_prevMousePos = _currentMousePos;
}

Input::~Input()
{

}

void Input::Update()
{
	_prevMousePos = _currentMousePos;

	// すべてのキーの入力状態を保存する
	GetHitKeyStateAllEx(_keyState.data());

	// 現在のマウス座標を取得
	GetMousePoint(&_currentMousePos.x, &_currentMousePos.y);

	// 移動量を計算
	_mouseDelta = _currentMousePos - _prevMousePos;
}

InputStateKind Input::GetKeyState(int keyCode) const
{
	// GetHitKeyStateAllEx
	// 0 : 押されていない, 1 : 押された１回め, 2以上 : 押され続けている回数, - 1 : 押されて離された１回め, - 2以下 : 押されて離されてからの回数

	const auto resultCount = _keyState.at(keyCode);

	if (resultCount == 1)
	{
		return InputStateKind::kSingle;
	}
	else if (resultCount >= 2)
	{
		return InputStateKind::kHold;
	}
	else if (resultCount == -1)
	{
		return InputStateKind::kPrev;
	}

	return InputStateKind::kNone;
}

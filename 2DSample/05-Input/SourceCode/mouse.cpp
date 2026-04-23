#include <array>
#include <DxLib.h>
#include <Vector/vector2.hpp>
#include <Vector/vector2_int.hpp>
#include "mouse.h"

Mouse::Mouse() :
	_state		(0),
	_currentPos(0, 0),
	_prevPos	(0, 0),
	_delta		(0, 0),
	_scroll	(0.0f, 0.0f)
{
	GetMousePoint(&_currentPos.x, &_currentPos.y);
	_prevPos = _currentPos;
}

Mouse::~Mouse()
{
	
}

void Mouse::Update()
{
	_prevPos = _currentPos;

	// 現在のマウス座標を取得
	GetMousePoint(&_currentPos.x, &_currentPos.y);

	// マウスホイールの移動量を取得
	_scroll = { GetMouseWheelRotVolF(), GetMouseHWheelRotVolF() };

	// 移動量を計算
	_delta = _currentPos - _prevPos;
}

int Mouse::GetIndex(uint8_t mouseButton) const
{
	for (int i = 0; i < 8; ++i)
	{
		if (mouseButton == (1 << i)) { return i; }
	}

	return -1;
}

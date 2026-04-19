#include <array>
#include <DxLib.h>
#include <Vector/vector2_int.hpp>
#include "mouse.h"

Mouse::Mouse() :
	_currentMousePos(0, 0),
	_prevMousePos	(0, 0),
	_mouseDelta		(0, 0)
{
	GetMousePoint(&_currentMousePos.x, &_currentMousePos.y);
	_prevMousePos = _currentMousePos;
}

Mouse::~Mouse()
{

}

void Mouse::Update()
{
	_prevMousePos = _currentMousePos;

	// 現在のマウス座標を取得
	GetMousePoint(&_currentMousePos.x, &_currentMousePos.y);

	// 移動量を計算
	_mouseDelta = _currentMousePos - _prevMousePos;
}
